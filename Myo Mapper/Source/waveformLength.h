#ifndef __JUCE_HEADER_7BB4186462AAD392__
#define __JUCE_HEADER_7BB4186462AAD392__

#include "../JuceLibraryCode/JuceHeader.h"

class WaveformLength    :   public Component
{
public:
    WaveformLength ();
    ~WaveformLength();

    void setValue (float value);
    float getValue();
    Vector3D<float> get3DValue();
    void set3DValue (Vector3D<float>);

private:
    
    float previousInput;
    float input;
    float wl;
    float factor = 4;

    Vector3D<float> input3D;
    Vector3D<float> previousInput3D;
    Vector3D<float> wl3D;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformLength)
};

#endif
