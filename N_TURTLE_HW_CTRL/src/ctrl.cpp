#include "ctrl.h"

/* set RTOS parameters */
unsigned long prevTime = 0; // init prev time with 0

// configure encoder L & R
volatile long encoderLeftCount = 0;
volatile long encoderRightCount = 0;

/* glopal var to take pwm for motors */
std_msgs::Float32MultiArray inputPwm;

/* subscriber cb for motor speed */
void callback(const std_msgs::Float32MultiArray &velMsg)
{
    inputPwm = velMsg;
}
// sub to "/AgribotVelWpid" topic for pwm data
ros::Subscriber<std_msgs::Float32MultiArray> sub("/N_TurtleVel", &callback);

// global var to take encoder values
std_msgs::Float32MultiArray msg;

// pub for angular velocity for both motors
ros::Publisher pub("/encoder", &msg);

/* IMU SETUP*/
sensor_msgs::Imu imu_msg;
ros::Publisher imu_pub("/imu/data_raw", &imu_msg);
int sequence = 0;
// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU;

// init for motor & encoder
void ctrlInit(void)
{
    // sub & pub
    nh.advertise(pub);
    nh.advertise(imu_pub);
    nh.subscribe(sub);

    msg.data_length = 2;
    msg.data = new float[msg.data_length];

    // motor init
    pinMode(pinDirLeft1, OUTPUT);
    pinMode(pinDirRight1, OUTPUT);
    pinMode(pinPwmLeft1, OUTPUT);
    pinMode(pinPwmRight1, OUTPUT);
    analogWrite(pinPwmLeft1, 0);
    analogWrite(pinPwmRight1, 0);
    // encoder pin setups
    pinMode(encoderLeftA, INPUT_PULLUP);
    pinMode(encoderRightA, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(encoderLeftA), updateEncoderLeft, RISING);
    attachInterrupt(digitalPinToInterrupt(encoderRightA), updateEncoderRight, RISING);

    Wire.begin();
    if (!IMU.setup(0x68))
    { // change to your own address
      //     // while (1)
      //     // {
      //         // Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      //     //     delay(5000);
      //     // }
    }
    // calibrate anytime you want to
    IMU.verbose(true);
    IMU.calibrateAccelGyro();
    IMU.calibrateMag();
    IMU.verbose(false);
}
void robotMove(void)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {                                         // Read the position in an atomic block to avoid a potential misread if the interrupt coincides with this code running
        unsigned long currentTime = millis(); // get the current time
        if (currentTime - prevTime > interval)
        { // check if enough time has elapsed since the last interrupt
            // meassuring angular velocity for left & right motors
            double motorLeftSpeed = double((encoderLeftCount / PPR) * (2 * PI) * (1000.0 / interval));    // Revs per second to radians per second
            double motorRightSpeed = -double((encoderRightCount / PPR) * (2 * PI) * (1000.0 / interval)); // Revs per second to radians per second

            // convert linear velocity data to pwm in case no pid
            // for front engines
            setSpeed(mypwm(inputPwm.data[1]), pinDirRight1, pinPwmRight1); // set right Motor with input speed.
            setSpeed(mypwm(inputPwm.data[0]), pinDirLeft1, pinPwmLeft1);   // set left Motor with input speed.
            /* IMU SETUP */
            // Header
            imu_msg.header.seq = sequence++;
            imu_msg.header.stamp = nh.now();
            imu_msg.header.frame_id = "imu_link";

            // Linear Acceleration
            imu_msg.linear_acceleration.x = IMU.getLinearAccX();
            imu_msg.linear_acceleration.y = 0; // IMU.getAccY();
            imu_msg.linear_acceleration.z = 0; // IMU.getLinearAccZ();

            // Angular Velocity
            imu_msg.angular_velocity.x = 0; // IMU.getGyroX();
            imu_msg.angular_velocity.y = 0; // IMU.getGyroY();
            imu_msg.angular_velocity.z = IMU.getGyroZ();

            // Orientation
            imu_msg.orientation.x = 0;
            imu_msg.orientation.y = 0;
            imu_msg.orientation.z = 0;
            imu_msg.orientation.w = IMU.getQuaternionW();
            imu_pub.publish(&imu_msg);

            // give ENCODER feedback to be published
            msg.data[0] = motorLeftSpeed;
            msg.data[1] = motorRightSpeed;
            pub.publish(&msg); // publishing encoder angular velocity msgs

            // reset counts to 0
            encoderLeftCount = 0;
            encoderRightCount = 0;
            prevTime = currentTime;
        }
    }
}
// set motor speed
void setSpeed(int pwm, int dirPin, int PwmPin)
{
    if (pwm > 0)
    {
        digitalWrite(dirPin, LOW);
    }
    else if (pwm < 0)
    {
        digitalWrite(dirPin, HIGH);
    }
    analogWrite(PwmPin, abs(pwm));
}
// convert speed to motor effort//////////////
int mypwm(float x)
{
    int m_pwm = round((x * 255) / 2);
    return m_pwm;
}
// update left encoder
void updateEncoderLeft(void)
{
    if (digitalRead(encoderLeftB) > 0)
    {
        encoderLeftCount++;
    }
    else
    {
        encoderLeftCount--;
    }
}
// update Right encoder
void updateEncoderRight(void)
{
    if (digitalRead(encoderRightB) > 0)
    {
        encoderRightCount++;
    }
    else
    {
        encoderRightCount--;
    }
}
