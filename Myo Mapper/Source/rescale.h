#ifndef RESCALE_H_INCLUDED
#define RESCALE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Rescale   :   public Component,
                    public Button::Listener,
                    public Slider::Listener
{
public:
    
    float outValue; // rescaled output
    
    Rescale();
    ~Rescale() {};

    void paint (Graphics& g) override;
    void  buttonClicked (Button* button) override;
    void sliderValueChanged (Slider* slider) override;
    void resized() override;
    void setLabelWidget (String LabelWidget);
    void setValue (float Value);
    float getValue();
    void setTargetValue (float TargetValue);
    void setMin (float Value);
    void setMax (float Value);
    void setCalibrate();
    void setReverse (bool Status);
    
private:
    
    TextButton calibrate;
    Slider mmSlider;
    Label mmSliderLabel;

    ToggleButton reverse;
    
    Slider outMinSlider;
    Slider outMaxSlider;
    Slider inMinSlider;
    Slider inMaxSlider;
    
    Label outMinSliderLabel;
    Label outMaxSliderLabel;
    Label inMinSliderLabel;
    Label inMaxSliderLabel;
    
    OSCSender sender;
    bool enableOSCvalue;
    int oscPort;
    String hostAddress;
    
    float reversed;
    float calibrated;
    float offset;
    float targetValue = 0.5;
    float scaled;
    float input;
    float input1;
    String labelWidget;
    const long double PI = 3.141592653589793;
    const long double r2PI;
    int test = 0;
    String MyoIDString;
    float inMin;
    float inMax;
    float outMin;
    float outMax;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Rescale)
};


#endif  // RESCALE_H_INCLUDED
