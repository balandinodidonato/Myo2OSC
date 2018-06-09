#include "../JuceLibraryCode/JuceHeader.h"
#include "../DataStreaming/OSC.h"
#include "../Application.h"
#include "../UI/Windows/Rescale.h"

OSC::OSC()
:   myoDataIn {"Yaw", "Pitch", "Roll", "Mav"},
    action {"vibrate", "calibrate", "reverse", "inMin", "inMax", "outMin", "outMax"}
{
    receivePort = Application::receivePort;
    
    for (int i = 1; i < 5; ++i) // id
    {
        String I = String(i);
        
        receiver.addListener (this, "/myo" + I + "/" + action[0]);
        
        for (int y = 0; y < 4; ++y) // myo data
        {
            for (int z = 0; z < 7; ++z) //action
            {
                receiver.addListener (this, "/myo" + I + "/" + myoDataIn[y] + "/" + action[z]);
            }
        }
    }
}

OSC::~OSC()
{
}

bool OSC::connectSender (String mainHostAddress, int mainPort, String mlHostAddress, int mlOscPort)
{
    mlOscSender.connect(mlHostAddress, mlOscPort);
    
    if (mainOscSender.connect (mainHostAddress, mainPort) == false)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "ML OSC Sender",
                                          "Myo Mapper could not connect to UDP port " + (String)mlOscPort + ".",
                                          "OK");
        return false;
    }

    if (mainOscSender.connect (mainHostAddress, mainPort) == false)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "OSC Sender",
                                          "Myo Mapper could not connect to UDP port " + (String)mainPort + ".",
                                          "OK");
        return false;
    }
    return true;
}
void OSC::disconnectSender()
{
    mainOscSender.disconnect();
    mlOscSender.disconnect();
}

void OSC::bufferOsc (MyoData &myoData)
{
    String id = String (Application::selectedMyo);
 
    orScaled.x = (myoData.orientationRaw.x + PI) / (2 * PI); // for passing variable to orientationScaled data to sender
    orScaled.y = (myoData.orientationRaw.y + PI) / (2 * PI); // for passing variable to orientationScaled data to sender
    orScaled.z = (myoData.orientationRaw.z + PI) / (2 * PI); // for passing variable to orientationScaled data to sender
    
    auto tree = Application::getApp().getOscStreamingTree();
    if (tree.getChildWithName("OrData").getChildWithName("OrQuaternion").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/orientation/quaternion");
        oscOut.addFloat32 ((float) myoData.quaternion[0]);
        oscOut.addFloat32 ((float) myoData.quaternion[1]);
        oscOut.addFloat32 ((float) myoData.quaternion[2]);
        oscOut.addFloat32 ((float) myoData.quaternion[3]);
        oscOutBuffer.push_back (oscOut);
    }
    if (tree.getChildWithName("OrData").getChildWithName("OrRaw").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/orientation/raw");
        oscOut.addFloat32 ((float) myoData.orientationRaw.x);
        oscOut.addFloat32 ((float) myoData.orientationRaw.y);
        oscOut.addFloat32 ((float) myoData.orientationRaw.z);
        
        oscOutBuffer.push_back (oscOut);
    }
    if (tree.getChildWithName("OrData").getChildWithName("OrScaled").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/orientation/scaled");
        oscOut.addFloat32 ((float) myoData.orientationScaled.x);
        oscOut.addFloat32 ((float) myoData.orientationScaled.y);
        oscOut.addFloat32 ((float) myoData.orientationScaled.z);
        
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("OrData").getChildWithName("OrVelocity").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/orientation/velocity");
        oscOut.addFloat32 ((float) myoData.orientationScaledFod.x);
        oscOut.addFloat32 ((float) myoData.orientationScaledFod.y);
        oscOut.addFloat32 ((float) myoData.orientationScaledFod.z);
        oscOutBuffer.push_back (oscOut);
    }
    if (tree.getChildWithName("OrData").getChildWithName("OrAccel").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/orientation/acceleration");
        oscOut.addFloat32 ((float) myoData.orientationScaledSod.x);
        oscOut.addFloat32 ((float) myoData.orientationScaledSod.y);
        oscOut.addFloat32 ((float) myoData.orientationScaledSod.z);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("AccData").getChildWithName("AccRaw").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/acceleration/raw");
        oscOut.addFloat32 ((float) myoData.acc.x);
        oscOut.addFloat32 ((float) myoData.acc.y);
        oscOut.addFloat32 ((float) myoData.acc.x);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("AccData").getChildWithName("AccRaw").getChildWithName("AccRawFod").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/acceleration/raw/fod");
        oscOut.addFloat32 ((float) myoData.accFod.x);
        oscOut.addFloat32 ((float) myoData.accFod.y);
        oscOut.addFloat32 ((float) myoData.accFod.z);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("AccData").getChildWithName("AccScaled").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/acceleration/scaled");
        oscOut.addFloat32 ((float) myoData.accScaled.x);
        oscOut.addFloat32 ((float) myoData.accScaled.y);
        oscOut.addFloat32 ((float) myoData.accScaled.z);
        oscOutBuffer.push_back (oscOut);
    }
    
    
    if (tree.getChildWithName("AccData").getChildWithName("AccScaled").getChildWithName("AccScaledFod").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/acceleration/scaled/fod");
        oscOut.addFloat32 ((float) myoData.accScaledFod.x);
        oscOut.addFloat32 ((float) myoData.accScaledFod.y);
        oscOut.addFloat32 ((float) myoData.accScaledFod.z);
        oscOutBuffer.push_back (oscOut);
    }
    if (tree.getChildWithName("AccData").getChildWithName("AccScaled").getChildWithName("AccScaledFod").getChildWithName("AccScaledFodMavg").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/acceleration/scaled/fod/mavg");
        oscOut.addFloat32 ((float) myoData.accScaledFodMavg.x);
        oscOut.addFloat32 ((float) myoData.accScaledFodMavg.y);
        oscOut.addFloat32 ((float) myoData.accScaledFodMavg.z);
        oscOutBuffer.push_back (oscOut);
    }
    if (tree.getChildWithName("GyroData").getChildWithName("GyroRaw").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/gyro/raw");
        oscOut.addFloat32 ((float) myoData.gyro.x);
        oscOut.addFloat32 ((float) myoData.gyro.y);
        oscOut.addFloat32 ((float) myoData.gyro.z);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("GyroData").getChildWithName("GyroRaw").getChildWithName("GyroRawFod").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/gyro/raw/fod");
        oscOut.addFloat32 ((float) myoData.gyroFod.x);
        oscOut.addFloat32 ((float) myoData.gyroFod.y);
        oscOut.addFloat32 ((float) myoData.gyroFod.z);
        oscOutBuffer.push_back (oscOut);
    }
    if (tree.getChildWithName("GyroData").getChildWithName("GyroScaled").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/gyro/scaled");
        oscOut.addFloat32 ((float) myoData.gyroScaled.x);
        oscOut.addFloat32 ((float) myoData.gyroScaled.y);
        oscOut.addFloat32 ((float) myoData.gyroScaled.z);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("GyroData").getChildWithName("GyroScaled").getChildWithName("GyroScaledAbs").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/gyro/scaled/abs");
        oscOut.addFloat32 ((float) myoData.gyroScaledAbs.x);
        oscOut.addFloat32 ((float) myoData.gyroScaledAbs.y);
        oscOut.addFloat32 ((float) myoData.gyroScaledAbs.z);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("GyroData").getChildWithName("GyroScaled").getChildWithName("GyroScaledFod").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/gyro/scaled/fod");
        oscOut.addFloat32 ((float) myoData.gyroScaledFod.x);
        oscOut.addFloat32 ((float) myoData.gyroScaledFod.y);
        oscOut.addFloat32 ((float) myoData.gyroScaledFod.z);
        oscOutBuffer.push_back (oscOut);
    }
    if (tree.getChildWithName("GyroData").getChildWithName("GyroScaled").getChildWithName("GyroScaledFod").getChildWithName("GyroScaledFodMavg").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/gyro/scaled/fod/mavg");
        oscOut.addFloat32 ((float) myoData.gyroScaledFodMavg.x);
        oscOut.addFloat32 ((float) myoData.gyroScaledFodMavg.y);
        oscOut.addFloat32 ((float) myoData.gyroScaledFodMavg.z);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("EmgData").getChildWithName("EmgRaw").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/raw");
        oscOut.addInt32 ((int) myoData.emgRaw[0]);
        oscOut.addInt32 ((int) myoData.emgRaw[1]);
        oscOut.addInt32 ((int) myoData.emgRaw[2]);
        oscOut.addInt32 ((int) myoData.emgRaw[3]);
        oscOut.addInt32 ((int) myoData.emgRaw[4]);
        oscOut.addInt32 ((int) myoData.emgRaw[5]);
        oscOut.addInt32 ((int) myoData.emgRaw[6]);
        oscOut.addInt32 ((int) myoData.emgRaw[7]);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("EmgData").getChildWithName("EmgRaw").getChildWithName("EmgRawMavg").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/raw/mavg");
        oscOut.addInt32 ((int) myoData.emgRawMavg[0]);
        oscOut.addInt32 ((int) myoData.emgRawMavg[1]);
        oscOut.addInt32 ((int) myoData.emgRawMavg[2]);
        oscOut.addInt32 ((int) myoData.emgRawMavg[3]);
        oscOut.addInt32 ((int) myoData.emgRawMavg[4]);
        oscOut.addInt32 ((int) myoData.emgRawMavg[5]);
        oscOut.addInt32 ((int) myoData.emgRawMavg[6]);
        oscOut.addInt32 ((int) myoData.emgRawMavg[7]);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("EmgData").getChildWithName("EmgRaw").getChildWithName("EmgRawZcr").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/raw/zcr");
        oscOut.addInt32 ((int) myoData.emgZeroCross[0]);
        oscOut.addInt32 ((int) myoData.emgZeroCross[1]);
        oscOut.addInt32 ((int) myoData.emgZeroCross[2]);
        oscOut.addInt32 ((int) myoData.emgZeroCross[3]);
        oscOut.addInt32 ((int) myoData.emgZeroCross[4]);
        oscOut.addInt32 ((int) myoData.emgZeroCross[5]);
        oscOut.addInt32 ((int) myoData.emgZeroCross[6]);
        oscOut.addInt32 ((int) myoData.emgZeroCross[7]);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("EmgData").getChildWithName("EmgRaw").getChildWithName("EmgRawZcr").getChildWithName("EmgRawZcrMavg").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/raw/zcr/mavg");
        oscOut.addInt32 ((int) myoData.emgZeroCrossMavg[0]);
        oscOut.addInt32 ((int) myoData.emgZeroCrossMavg[1]);
        oscOut.addInt32 ((int) myoData.emgZeroCrossMavg[2]);
        oscOut.addInt32 ((int) myoData.emgZeroCrossMavg[3]);
        oscOut.addInt32 ((int) myoData.emgZeroCrossMavg[4]);
        oscOut.addInt32 ((int) myoData.emgZeroCrossMavg[5]);
        oscOut.addInt32 ((int) myoData.emgZeroCrossMavg[6]);
        oscOut.addInt32 ((int) myoData.emgZeroCrossMavg[7]);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/scaled");
        oscOut.addFloat32 ((float) myoData.emgScaled[0]);
        oscOut.addFloat32 ((float) myoData.emgScaled[1]);
        oscOut.addFloat32 ((float) myoData.emgScaled[2]);
        oscOut.addFloat32 ((float) myoData.emgScaled[3]);
        oscOut.addFloat32 ((float) myoData.emgScaled[4]);
        oscOut.addFloat32 ((float) myoData.emgScaled[5]);
        oscOut.addFloat32 ((float) myoData.emgScaled[6]);
        oscOut.addFloat32 ((float) myoData.emgScaled[7]);
        oscOutBuffer.push_back (oscOut);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/scaled/abs");
        oscOut.addFloat32 ((float) myoData.emgScaledAbs[0]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbs[1]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbs[2]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbs[3]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbs[4]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbs[5]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbs[6]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbs[7]);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMin").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/scaled/abs/min");
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMin[0]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMin[1]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMin[2]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMin[3]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMin[4]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMin[5]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMin[6]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMin[7]);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMax").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/scaled/abs/max");
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMax[0]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMax[1]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMax[2]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMax[3]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMax[4]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMax[5]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMax[6]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMax[7]);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsFod").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/scaled/abs/fod");
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFod[0]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFod[1]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFod[2]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFod[3]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFod[4]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFod[5]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFod[6]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFod[7]);
        oscOutBuffer.push_back (oscOut);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsFod").getChildWithName("EmgScaledAbsFodMavg").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/scaled/abs/fod/mavg");
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFodMavg[0]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFodMavg[1]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFodMavg[2]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFodMavg[3]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFodMavg[4]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFodMavg[5]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFodMavg[6]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsFodMavg[7]);
        oscOutBuffer.push_back (oscOut);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMavg").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/scaled/abs/mavg");
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMavg[0]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMavg[1]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMavg[2]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMavg[3]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMavg[4]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMavg[5]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMavg[6]);
        oscOut.addFloat32 ((float) myoData.emgScaledAbsMavg[7]);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMav").getChildWithName("EmgScaledAbsMavMavg").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/scaled/abs/mav/mavg");
        oscOut.addFloat32 ((float) myoData.emgMav);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMav").getChildWithName("EmgScaledAbsMavMin").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/scaled/abs/mav/min");
        oscOut.addFloat32 ((float) myoData.emgMavMin);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMav").getChildWithName("EmgScaledAbsMavMax").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/scaled/abs/mav/max");
        oscOut.addFloat32 ((float) myoData.emgMavMax);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMav").getChildWithName("EmgScaledAbsMavFod").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/scaled/abs/mav/fod");
        oscOut.addFloat32 ((float) myoData.mavFod);
        oscOutBuffer.push_back (oscOut);
    }
    
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMav").getChildWithName("EmgScaledAbsMavFod").getChildWithName("EmgScaledAbsMavFodMavg").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/emg/scaled/abs/mav/fod/mavg");
        oscOut.addFloat32 ((float) myoData.mavFodMavg);
        oscOutBuffer.push_back (oscOut);
    }
    
    
    if (tree.getChildWithName("EmgData").getChildWithName("HandPose").getPropertyAsValue ("oscOut", 0) == true)
    {
        OSCMessage oscOut = OSCMessage ("/myo" + id + "/pose");
        oscOut.addInt32 (myoData.poseID);
        oscOut.addString (myoData.pose);
        oscOutBuffer.push_back (oscOut);
    }

    //========================================================
    //                 Data to Wekinator
    //========================================================
    OSCMessage oscToWekinator = OSCMessage ("/myo" + id);

    if (tree.getChildWithName("OrData").getChildWithName("OrQuaternion").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.quaternion[0]);
        oscToWekinator.addFloat32 ((float) myoData.quaternion[1]);
        oscToWekinator.addFloat32 ((float) myoData.quaternion[2]);
        oscToWekinator.addFloat32 ((float) myoData.quaternion[3]);
    }
    if (tree.getChildWithName("OrData").getChildWithName("OrRaw").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.orientationRaw.x);
        oscToWekinator.addFloat32 ((float) myoData.orientationRaw.y);
        oscToWekinator.addFloat32 ((float) myoData.orientationRaw.z);
    }
    if (tree.getChildWithName("OrData").getChildWithName("OrScaled").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.orientationScaled.x);
        oscToWekinator.addFloat32 ((float) myoData.orientationScaled.y);
        oscToWekinator.addFloat32 ((float) myoData.orientationScaled.z);
    }
    if (tree.getChildWithName("OrData").getChildWithName("OrVelocity").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.orientationScaledFod.x);
        oscToWekinator.addFloat32 ((float) myoData.orientationScaledFod.y);
        oscToWekinator.addFloat32 ((float) myoData.orientationScaledFod.z);
    }
    if (tree.getChildWithName("OrData").getChildWithName("OrAccel").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.orientationScaledSod.x);
        oscToWekinator.addFloat32 ((float) myoData.orientationScaledSod.y);
        oscToWekinator.addFloat32 ((float) myoData.orientationScaledSod.z);
    }
    if (tree.getChildWithName("AccData").getChildWithName("AccRaw").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.acc.x);
        oscToWekinator.addFloat32 ((float) myoData.acc.y);
        oscToWekinator.addFloat32 ((float) myoData.acc.x);
    }
    if (tree.getChildWithName("AccData").getChildWithName("AccRaw").getChildWithName("AccRawFod").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.accFod.x);
        oscToWekinator.addFloat32 ((float) myoData.accFod.y);
        oscToWekinator.addFloat32 ((float) myoData.accFod.z);
    }
    if (tree.getChildWithName("AccData").getChildWithName("AccScaled").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.accScaled.x);
        oscToWekinator.addFloat32 ((float) myoData.accScaled.y);
        oscToWekinator.addFloat32 ((float) myoData.accScaled.z);
    }
    if (tree.getChildWithName("AccData").getChildWithName("AccScaled").getChildWithName("AccScaledFod").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.accScaledFod.x);
        oscToWekinator.addFloat32 ((float) myoData.accScaledFod.y);
        oscToWekinator.addFloat32 ((float) myoData.accScaledFod.z);
    }
    if (tree.getChildWithName("AccData").getChildWithName("AccScaled").getChildWithName("AccScaledFod").getChildWithName("AccScaledFodMavg").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.accScaledFodMavg.x);
        oscToWekinator.addFloat32 ((float) myoData.accScaledFodMavg.y);
        oscToWekinator.addFloat32 ((float) myoData.accScaledFodMavg.z);
    }
    
    if (tree.getChildWithName("GyroData").getChildWithName("GyroRaw").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.gyro.x);
        oscToWekinator.addFloat32 ((float) myoData.gyro.y);
        oscToWekinator.addFloat32 ((float) myoData.gyro.z);
    }
    if (tree.getChildWithName("GyroData").getChildWithName("GyroRaw").getChildWithName("GyroRawFod").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.gyroFod.x);
        oscToWekinator.addFloat32 ((float) myoData.gyroFod.y);
        oscToWekinator.addFloat32 ((float) myoData.gyroFod.z);
    }
    if (tree.getChildWithName("GyroData").getChildWithName("GyroScaled").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.gyroScaled.x);
        oscToWekinator.addFloat32 ((float) myoData.gyroScaled.y);
        oscToWekinator.addFloat32 ((float) myoData.gyroScaled.z);
    }
    if (tree.getChildWithName("GyroData").getChildWithName("GyroScaled").getChildWithName("GyroScaledAbs").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.gyroScaledAbs.x);
        oscToWekinator.addFloat32 ((float) myoData.gyroScaledAbs.y);
        oscToWekinator.addFloat32 ((float) myoData.gyroScaledAbs.z);
    }
    if (tree.getChildWithName("GyroData").getChildWithName("GyroScaled").getChildWithName("GyroScaledFod").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.gyroScaledFod.x);
        oscToWekinator.addFloat32 ((float) myoData.gyroScaledFod.y);
        oscToWekinator.addFloat32 ((float) myoData.gyroScaledFod.z);
    }
    if (tree.getChildWithName("GyroData").getChildWithName("GyroScaled").getChildWithName("GyroScaledFod").getChildWithName("GyroScaledFodMavg").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.gyroScaledFodMavg.x);
        oscToWekinator.addFloat32 ((float) myoData.gyroScaledFodMavg.y);
        oscToWekinator.addFloat32 ((float) myoData.gyroScaledFodMavg.z);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgRaw").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addInt32 ((int) myoData.emgRaw[0]);
        oscToWekinator.addInt32 ((int) myoData.emgRaw[1]);
        oscToWekinator.addInt32 ((int) myoData.emgRaw[2]);
        oscToWekinator.addInt32 ((int) myoData.emgRaw[3]);
        oscToWekinator.addInt32 ((int) myoData.emgRaw[4]);
        oscToWekinator.addInt32 ((int) myoData.emgRaw[5]);
        oscToWekinator.addInt32 ((int) myoData.emgRaw[6]);
        oscToWekinator.addInt32 ((int) myoData.emgRaw[7]);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgRaw").getChildWithName("EmgRawMavg").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addInt32 ((int) myoData.emgRawMavg[0]);
        oscToWekinator.addInt32 ((int) myoData.emgRawMavg[1]);
        oscToWekinator.addInt32 ((int) myoData.emgRawMavg[2]);
        oscToWekinator.addInt32 ((int) myoData.emgRawMavg[3]);
        oscToWekinator.addInt32 ((int) myoData.emgRawMavg[4]);
        oscToWekinator.addInt32 ((int) myoData.emgRawMavg[5]);
        oscToWekinator.addInt32 ((int) myoData.emgRawMavg[6]);
        oscToWekinator.addInt32 ((int) myoData.emgRawMavg[7]);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgRaw").getChildWithName("EmgRawZcr").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addInt32 ((int) myoData.emgZeroCross[0]);
        oscToWekinator.addInt32 ((int) myoData.emgZeroCross[1]);
        oscToWekinator.addInt32 ((int) myoData.emgZeroCross[2]);
        oscToWekinator.addInt32 ((int) myoData.emgZeroCross[3]);
        oscToWekinator.addInt32 ((int) myoData.emgZeroCross[4]);
        oscToWekinator.addInt32 ((int) myoData.emgZeroCross[5]);
        oscToWekinator.addInt32 ((int) myoData.emgZeroCross[6]);
        oscToWekinator.addInt32 ((int) myoData.emgZeroCross[7]);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgRaw").getChildWithName("EmgRawZcr").getChildWithName("EmgRawZcrMavg").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addInt32 ((int) myoData.emgZeroCrossMavg[0]);
        oscToWekinator.addInt32 ((int) myoData.emgZeroCrossMavg[1]);
        oscToWekinator.addInt32 ((int) myoData.emgZeroCrossMavg[2]);
        oscToWekinator.addInt32 ((int) myoData.emgZeroCrossMavg[3]);
        oscToWekinator.addInt32 ((int) myoData.emgZeroCrossMavg[4]);
        oscToWekinator.addInt32 ((int) myoData.emgZeroCrossMavg[5]);
        oscToWekinator.addInt32 ((int) myoData.emgZeroCrossMavg[6]);
        oscToWekinator.addInt32 ((int) myoData.emgZeroCrossMavg[7]);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.emgScaled[0]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaled[1]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaled[2]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaled[3]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaled[4]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaled[5]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaled[6]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaled[7]);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbs[0]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbs[1]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbs[2]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbs[3]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbs[4]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbs[5]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbs[6]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbs[7]);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMin").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMin[0]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMin[1]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMin[2]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMin[3]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMin[4]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMin[5]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMin[6]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMin[7]);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMax").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMax[0]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMax[1]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMax[2]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMax[3]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMax[4]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMax[5]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMax[6]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMax[7]);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsFod").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFod[0]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFod[1]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFod[2]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFod[3]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFod[4]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFod[5]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFod[6]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFod[7]);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsFod").getChildWithName("EmgScaledAbsFodMavg").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFodMavg[0]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFodMavg[1]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFodMavg[2]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFodMavg[3]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFodMavg[4]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFodMavg[5]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFodMavg[6]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsFodMavg[7]);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMavg").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMavg[0]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMavg[1]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMavg[2]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMavg[3]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMavg[4]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMavg[5]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMavg[6]);
        oscToWekinator.addFloat32 ((float) myoData.emgScaledAbsMavg[7]);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMav").getChildWithName("EmgScaledAbsMavMavg").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.emgMav);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMav").getChildWithName("EmgScaledAbsMavMin").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.emgMavMin);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMav").getChildWithName("EmgScaledAbsMavMax").getPropertyAsValue ("oscOut", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.emgMavMax);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMav").getChildWithName("EmgScaledAbsMavFod").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.mavFod);
    }
    if (tree.getChildWithName("EmgData").getChildWithName("EmgScaled").getChildWithName("EmgScaledAbs").getChildWithName("EmgScaledAbsMav").getChildWithName("EmgScaledAbsMavFod").getChildWithName("EmgScaledAbsMavFodMavg").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32 ((float) myoData.mavFodMavg);
    }
    
    if (tree.getChildWithName("EmgData").getChildWithName("HandPose").getPropertyAsValue ("oscToWekinator", 0) == true)
    {
        oscToWekinator.addFloat32((float)myoData.poseID);
    }
    
    oscToWekiBuffer.push_back (oscToWekinator);
}

void OSC::sendOsc ()
{
    for (int i = 0; i < oscOutBuffer.size(); ++i)
    {
        mainOscSender.send (oscOutBuffer.at(i));
    }
    for (int i = 0; i < oscToWekiBuffer.size(); ++i)
    {
        mlOscSender.send (oscToWekiBuffer.at(i));
    }
    oscOutBuffer.clear();
    oscToWekiBuffer.clear();
}

bool OSC::connectReceiver (int port)
{
    if (receiver.connect (port) == false)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "OSC Receiver",
                                          "Myo Mapper could not connect to UDP port " + (String)Application::receivePort + ".",
                                          "OK");
        return false;
    }
    return true;
}

void OSC::disconnectReceiver()
{
    receiver.disconnect();
}

void OSC::oscMessageReceived (const OSCMessage& oscIn)
{
    auto Id = Application::getApp().getOscSettingsTree().getChildWithName("SelectedMyo").getProperty ("myoId").toString();
    auto tree = Application::getApp().getOscSettingsTree().getChildWithName("DataScaling");
    
    // ---------------- Vibrate
    
    if (oscIn.getAddressPattern() == "/myo" + Id + "/vibrate")
        if (oscIn.size() == 1 && oscIn[0].isString())
        {
          //  auto vibrationType =  oscIn[0].getString();
         //   myoManager.vibrate (vibrationType);
        }
    
    for (int i = 0; i < 4; ++i) // myoDataIn
    {
        // ---------------- Calibrate
        
        if (oscIn.getAddressPattern() == "/myo" + Id + "/" + myoDataIn[i] + "/" + action[1])
        {
            if (oscIn.size() == 1 && oscIn[0].isString())
            {
                if (oscIn[0].getString() == "calibrate")
                {
                    if(myoDataIn[i] == "Yaw")
                    {
                        Application::getApp().getMyoDataScalingTree().getChildWithName(myoDataIn[i]+"Scaling").setProperty("offset", orScaled.x, 0);
                        
                        Application::getApp().getMyoDataScalingTree().getChildWithName(myoDataIn[i]+"Scaling").setProperty("test", 1, 0);
                    }
                    if(myoDataIn[i] == "Pitch")
                    {
                        Application::getApp().getMyoDataScalingTree().getChildWithName(myoDataIn[i]+"Scaling").setProperty("offset", orScaled.y, 0);
                        
                        Application::getApp().getMyoDataScalingTree().getChildWithName(myoDataIn[i]+"Scaling").setProperty("test", 1, 0);
                    }
                    if(myoDataIn[i] == "Roll")
                    {
                        Application::getApp().getMyoDataScalingTree().getChildWithName(myoDataIn[i]+"Scaling").setProperty("offset", orScaled.z, 0);
                        Application::getApp().getMyoDataScalingTree().getChildWithName(myoDataIn[i]+"Scaling").setProperty("test", 1, 0);
                    }
                }
            }
        }
        
        // ---------------- Reverse
        if (oscIn.getAddressPattern() == "/myo" + Id + "/" + myoDataIn[i] + "/" + action[2])
        {
            if (oscIn.size() == 1 && oscIn[0].isInt32())
            {
                if (oscIn[0].getInt32() == 1)
                {
                    Application::getApp().getMyoDataScalingTree().getChildWithName(myoDataIn[i]+"Scaling").setProperty("reverse", true, 0);
                }
                else if (oscIn[0].getInt32() == 0)
                {
                    Application::getApp().getMyoDataScalingTree().getChildWithName(myoDataIn[i]+"Scaling").setProperty("reverse", false, 0);
                }
            }
            else if (oscIn.size() == 1 && oscIn[0].isFloat32())
            {
                if (oscIn[0].getFloat32() == 1)
                {
                    Application::getApp().getMyoDataScalingTree().getChildWithName(myoDataIn[i]+"Scaling").setProperty("reverse", true, 0);
                }
                else if (oscIn[0].getFloat32() == 0)
                {
                    Application::getApp().getMyoDataScalingTree().getChildWithName(myoDataIn[i]+"Scaling").setProperty("reverse", false, 0);
                    Application::getApp().getMyoDataScalingTree().getChildWithName(myoDataIn[i]+"Scaling").getProperty("inMax");
                }
            }
            map[i][4] = true;
        }
        
        for (int y = 3; y < 7; ++y) // action
        {
            // ---------------- Set MinMax Scaling
            if (oscIn.getAddressPattern() == "/myo" + Id + "/" + myoDataIn[i] + "/" + action[y])
            {
                if (oscIn.size() == 1)
                {
                    auto tree = Application::getApp().getMyoDataScalingTree().getChildWithName(myoDataIn[i]+"Scaling");
                    float inMax = tree.getProperty("inMax");
                    float inMin = tree.getProperty("inMin");
                    
                    if (oscIn[0].isInt32())
                    {
                        if(y == 3 && oscIn[0].getInt32() >= inMax)
                        {
                            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                              "Error",
                                                              "Input minimum cannot be equal or greater than input maximum");
                        }
                        else if(y == 4 && oscIn[0].getInt32() <= inMin)
                        {
                            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                              "Error",
                                                              "Input maximum cannot be equal or less than input minimum");
                        }
                        else
                            Application::getApp().getMyoDataScalingTree().getChildWithName(myoDataIn[i]+"Scaling").setProperty(action[y], oscIn[0].getInt32(), 0);
                        
                    }
                    else if (oscIn[0].isFloat32())
                    {
                        if(y == 3 && oscIn[0].getFloat32() >= inMax)
                        {
                            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                              "Error",
                                                              "Input minimum cannot be equal or greater than input maximum");
                        }
                        else if(y == 4 && oscIn[0].getFloat32() <= inMin)
                        {
                            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                              "Error",
                                                              "Input maximum cannot be equal or less than input minimum");
                        }
                        else
                            Application::getApp().getMyoDataScalingTree().getChildWithName(myoDataIn[i]+"Scaling").setProperty(action[y], oscIn[0].getFloat32(), 0);
                    }
                }
            }
        }
    }
}

