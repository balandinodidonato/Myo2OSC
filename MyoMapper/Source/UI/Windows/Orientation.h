#ifndef MAPPING_H_INCLUDED
#define MAPPING_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Rescale.h"
#include "../../Features/FirstOrderDifference.h"
#include "../../Features/SecondOrderDifference.h"
#include "../MyoMapperLookAndFeel.h"

class Orientation   : public Component

{
public:
    Orientation();
    
    ~Orientation() {};
    
    void paint (Graphics& g) override;
    void resized() override;
    void setValues (Vector3D<float>, Vector3D<float>);
    
    void setReverseYaw();
    void setReversePitch();
    void setReverseRoll();
    
    void setInMinYaw();
    void setInMaxYaw();
    void setOutMinYaw();
    void setOutMaxYaw();
    
    void setInMinPitch();
    void setInMaxPitch();
    void setOutMinPitch();
    void setOutMaxPitch();
    
    void setInMinRoll();
    void setInMaxRoll();
    void setOutMinRoll();
    void setOutMaxRoll();
    
    Vector3D<float> getValue();
    
private:
    
    Label titleLabel;
    
    Rescale rescaleYaw;
    Rescale rescalePitch;
    Rescale rescaleRoll;
    FirstOrderDifference orientationFod;
    SecondOrderDifference orientationSod;
    
    Vector3D<float> orientationScaled;
    Vector3D<float> FodScaled;
    Vector3D<float> SodScaled;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Orientation)
};


#endif  // ORIENTATION_H_INCLUDED
