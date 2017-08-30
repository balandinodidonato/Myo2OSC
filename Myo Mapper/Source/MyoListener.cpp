#include "MyoListener.h"
#include <array>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Classes that inherit from myo::DeviceListener can be used to receive events from Myo devices. DeviceListener
// provides several virtual functions for handling different kinds of events. If you do not override an event, the
// default behavior is to do nothing.
MyoListener::MyoListener()
:   onArm (false),
    isUnlocked (false),
    orientation(),
    emgRaw(),
    emgScaled(),
    acceleration (0, 0, 0),
    yaw (0),
    pitch (0),
    roll (0),
    Pose ("null")
{
}

void MyoListener::onConnect (myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion)
{
    knownMyos.push_back (myo);
    myoData.resize (knownMyos.size());
    myo->setStreamEmg (myo::Myo::streamEmgEnabled);
    myo->unlock (myo::Myo::unlockHold);
}

// onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
void MyoListener::onUnpair (myo::Myo* myo, uint64_t timestamp)
{
    // We've lost a Myo.
    // Let's clean up some leftover state.
    roll = 0.0f;
    pitch = 0.0f;
    yaw = 0.0f;
    onArm = false;
    isUnlocked = false;
    emgRaw.fill (0);
    emgScaled.fill (0);
    Pose = "null";
}

void MyoListener::onPair (myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion)
{
    // We've added a Myo.
    // Let's initialise the.
    roll = 0.0f;
    pitch = 0.0f;
    yaw = 0.0f;
    onArm = false;
    isUnlocked = false;
    emgRaw.fill (0);
    emgScaled.fill (0);
    Pose = "null";
    
    knownMyos.push_back (myo);
    myoData.resize (knownMyos.size());
    myo->setStreamEmg (myo::Myo::streamEmgEnabled);
    myo->unlock (myo::Myo::unlockHold);
}

// onOrientationData() is called whenever the Myo device provides its current orientation, which is represented
// as a unit quaternion.
void MyoListener::onOrientationData (myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
{
    using std::atan2;
    using std::asin;
    using std::sqrt;
    using std::max;
    using std::min;
    // Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
    roll = atan2 (2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
                       1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
    pitch = asin (max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
    yaw = atan2 (2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
                      1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
    
    
    int myoID = getMyoID (myo);
    if (myoID == -1) return;
    
    myoData[myoID].orientationRaw.x = yaw;
    myoData[myoID].orientationRaw.y = pitch;
    myoData[myoID].orientationRaw.z = roll;
    myoData[myoID].quaternion[0] = quat.x();
    myoData[myoID].quaternion[1] = quat.y();
    myoData[myoID].quaternion[2] = quat.z();
    myoData[myoID].quaternion[3] = quat.w();
}

void MyoListener::onAccelerometerData (myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel)
{
    int myoID = getMyoID (myo);
    if (myoID == -1) return;
   
    myoData[myoID].acceleration.x = accel.x();
    myoData[myoID].acceleration.y = accel.y();
    myoData[myoID].acceleration.z = accel.z();

    myoData[myoID].accelerationScaled.x = (accel.x() + 16) * 0.03125;
    myoData[myoID].accelerationScaled.y = (accel.y() + 16) * 0.03125;
    myoData[myoID].accelerationScaled.z = (accel.z() + 16) * 0.03125;
   
    accWfL.set3DValue (myoData[myoID].acceleration);
    myoData[myoID].accelerationWfL = accWfL.get3DValue();
    
    accScaledWfL.set3DValue (myoData[myoID].accelerationScaled);
    myoData[myoID].accelerationScaledWfL = accScaledWfL.get3DValue();
}

void MyoListener::onGyroscopeData (myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro)
{

    int myoID = getMyoID (myo);
    if (myoID == -1) return;
   
    myoData[myoID].gyro.x = gyro.x();
    myoData[myoID].gyro.y = gyro.y();
    myoData[myoID].gyro.z = gyro.z();

    myoData[myoID].gyroScaled.x = (gyro.x() + 2000) * 0.00025;
    myoData[myoID].gyroScaled.y = (gyro.y() + 2000) * 0.00025;
    myoData[myoID].gyroScaled.z = (gyro.z() + 2000) * 0.00025;
   
    gyroWfL.set3DValue (myoData[myoID].gyro);
    myoData[myoID].gyroWfL = gyroWfL.get3DValue();
    
    gyroScaledWfL.set3DValue (myoData[myoID].gyroScaled);
    myoData[myoID].gyroScaledWfL = gyroScaledWfL.get3DValue();
    
}


void MyoListener::onPose (myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
{
    int poseID = 0;
    int myoID = getMyoID (myo);
    if (myoID == -1) return;
  
    if (pose == myo::Pose::unknown) poseID = -1;
    if (pose == myo::Pose::rest) poseID = 0;
    if (pose == myo::Pose::fist) poseID = 1;
    if (pose == myo::Pose::fingersSpread) poseID = 2;
    if (pose == myo::Pose::waveIn) poseID = 3;
    if (pose == myo::Pose::waveOut) poseID = 4;
    if (pose == myo::Pose::doubleTap) poseID = 5;

    myoData[myoID].pose = pose.toString();
    myoData[myoID].poseID = poseID;
}

// onArmSync() is called whenever Myo has recognized a Sync Gesture after someone has put it on their
// arm. This lets Myo know which arm it's on and which way it's facing.
void MyoListener::onArmSync (myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
                              myo::WarmupState warmupState)
{
    onArm = true;
    whichArm = arm;
}

void MyoListener::onEmgData (myo::Myo* myo, uint64_t timestamp, const int8_t* emg)
{
    int myoID = getMyoID (myo);
    if (myoID == -1) return;
    
    
    emgSum = 0; // reset value for AVG
    
    for (size_t i = 0; i < 8; ++i) {
    myoData[myoID].emgRaw[i] = emg[i];
    myoData[myoID].emgScaled[i] = (emg[i] + 127) * 0.003921568627;
    myoData[myoID].emgScaledAbs[i] = std::abs(emg[i] * 0.0078125);
    emgSum = emgSum + myoData[myoID].emgScaledAbs[i];
    }
    
    myoData[myoID].mav = emgSum * 0.125;
   
    mavWfL.setValue (myoData[myoID].mav);
    myoData[myoID].mavWfL = mavWfL.getValue();
}

// onArmUnsync() is called whenever Myo has detected that it was moved from a stable position on a person's arm after
// it recognized the arm. Typically this happens when someone takes Myo off of their arm, but it can also happen
// when Myo is moved around on the arm.
void MyoListener::onArmUnsync (myo::Myo* myo, uint64_t timestamp)
{
    onArm = false;
}

// onUnlock() is called whenever Myo has become unlocked, and will start delivering pose events.
void MyoListener::onUnlock (myo::Myo* myo, uint64_t timestamp)
{
    isUnlocked = true;
}

// onLock() is called whenever Myo has become locked. No pose events will be sent until the Myo is unlocked again.
void MyoListener::onLock (myo::Myo* myo, uint64_t timestamp)
{
    isUnlocked = false;
}

std::vector<MyoData> MyoListener::getMyoData() const
{
    return myoData;
}

int MyoListener::getMyoID (myo::Myo* myo)
{
    for (unsigned int i = 1; i < knownMyos.size(); ++i)
    {
        if (knownMyos[i] == myo)
        {
            return i;
        }
    }

    return -1;
}
