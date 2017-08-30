#ifndef OSC_H_INCLUDED
#define OSC_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class OSC   :   public Component,
                private OSCReceiver,
                private OSCReceiver::ListenerWithOSCAddress<OSCReceiver::MessageLoopCallback>

{
public:
    OSC();
    ~OSC();
    
    void setSender (String HostAddress, int Port);
    void setReceiver (int Port);
    void sendOSC (int id,
                 std::array<int8_t, 8> emgRaw,
                 std::array<float, 8> emgScaled,
                 std::array<float, 8> emgScaledAbs,
                 std::array<float, 4> quaternion,
                 float mav,
                 float mavWl,
                 Vector3D<float> gyro,
                 Vector3D<float> gyroWfL,
                 Vector3D<float> gyroScaled,
                 Vector3D<float> gyroScaledWfL,
                 Vector3D<float> acceleration,
                 Vector3D<float> accelerationWfL,
                 Vector3D<float> accelerationScaled,
                 Vector3D<float> accelerationScaledWfL,
                 Vector3D<float> orientationRaw,
                 Vector3D<float> orientationScaled,
                 Vector3D<float> orientationWfL,
                 
                 String pose,
                 int poseID);
    
    void connectSender();
    void connectReceiver();
    void disconnectSender();
    void disconnectReceiver();
    
    void oscMessageReceived (const OSCMessage& message) override;
    void getOSCMessage (const OSCMessage& message, String myoData);
    
    void setMyoIdReceiver (int ID);
    
    bool vibrate;
    String vibrationType;

    bool map[4][5];

    bool rescaleMinTest;
    int rescaleMIN;
    
    bool rescaleMaxTest;
    int rescaleMAX;
    float value;
    
    bool reverseStatus;

private:
    
    OSCSender sender;
    OSCReceiver receiver;
    
    int oscPortSender;
    int oscPortReceiver;
    String hostAddress;
    
    bool enableOSCvalue;
    
    bool oscConnectionSender;
    bool oscConnectionReceiver;
    
    String Id;
    
    String myoDataIn[4];
    String action[5];
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSC)
};


#endif  // OSC_H_INCLUDED
