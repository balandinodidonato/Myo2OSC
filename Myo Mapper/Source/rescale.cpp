#include "rescale.h"
#include <algorithm>

Rescale::Rescale()
:   reversed (0),
    calibrated (0),
    offset (0),
    targetValue (0),
    scaled (0),
    input (0),
    PI (3.141592653589793238L),
    r2PI (1 / (2 * PI)),
    test (0),
    inMin (0),
    inMax (1),
    outMin (0),
    outMax (1)
{
    addAndMakeVisible (calibrate);
    calibrate.setButtonText ("Calibrate");
    calibrate.addListener (this);
    
    addAndMakeVisible (mmSlider);
    mmSlider.setSliderStyle (juce::Slider::ThreeValueHorizontal);
    mmSlider.setRange (-1.0, 2.0);
    mmSlider.setMinValue (0.0);
    mmSlider.setMaxValue (1.0);
    mmSlider.addListener (this);
    addAndMakeVisible (mmSliderLabel);
    mmSliderLabel.attachToComponent (&mmSlider, true);
    
    addAndMakeVisible (reverse);
    reverse.addListener (this);
    reverse.setButtonText ("Reverse");

    outMinSliderLabel.setText ("Out Min", dontSendNotification);
    outMinSliderLabel.attachToComponent (&outMinSlider, true);
    addAndMakeVisible (outMinSliderLabel);
    outMinSlider.addListener (this);
    
    outMaxSliderLabel.setText ("Out Max", dontSendNotification);
    outMaxSliderLabel.attachToComponent (&outMaxSlider, true);
    addAndMakeVisible (outMaxSliderLabel);
    outMaxSlider.addListener (this);

    inMinSliderLabel.setText ("In Min", dontSendNotification);
    inMinSliderLabel.attachToComponent (&inMinSlider, true);
    addAndMakeVisible (inMinSliderLabel);
    inMinSlider.addListener (this);
    
    inMaxSliderLabel.setText ("In Max", dontSendNotification);
    inMaxSliderLabel.attachToComponent (&inMaxSlider, true);
    addAndMakeVisible (inMaxSliderLabel);
    inMaxSlider.addListener (this);
    
    outMinSlider.setRange (-1.0, 2.0, 0.001);
    outMinSlider.setValue (0.0);
    outMinSlider.setIncDecButtonsMode (juce::Slider::incDecButtonsDraggable_Vertical);
    outMinSlider.setSliderStyle (juce::Slider::IncDecButtons);
    addAndMakeVisible (outMinSlider);
    
    outMaxSlider.setRange (-1.0, 2.0, 0.001);
    outMaxSlider.setValue (0.0);
    outMaxSlider.setIncDecButtonsMode (juce::Slider::incDecButtonsDraggable_Vertical);
    outMaxSlider.setSliderStyle (juce::Slider::IncDecButtons);
    addAndMakeVisible (outMaxSlider);
    
    inMinSlider.setRange (0, 1, 0.001);
    inMinSlider.setValue (0);
    inMinSlider.setIncDecButtonsMode (juce::Slider::incDecButtonsDraggable_Vertical);
    inMinSlider.setSliderStyle (juce::Slider::IncDecButtons);
    addAndMakeVisible (inMinSlider);
  
    inMaxSlider.setRange (0, 1, 0.001);
    inMaxSlider.setValue (1);
    inMaxSlider.setIncDecButtonsMode (juce::Slider::incDecButtonsDraggable_Vertical);
    inMaxSlider.setSliderStyle (juce::Slider::IncDecButtons);
    addAndMakeVisible (inMaxSlider);
}

void Rescale::paint (juce::Graphics &g)
{
    g.fillAll (Colours::white);   // clear the background
    g.setColour (Colours::white);
    g.drawRoundedRectangle (0, 0, getWidth(), getHeight(), 3, 3);
    g.setColour (Colours::black);
    g.setFont (getHeight() * 0.17);
    g.drawText (labelWidget, getLocalBounds(), Justification::centredTop, true);   // draw some placeholder text
}

void Rescale::resized()
{
    calibrate.setBounds (10, 25, getWidth() * 0.2, getHeight() * 0.3);
    reverse.setBounds (calibrate.getRight() + getWidth() * 0.02,
                       calibrate.getY(),
                       getWidth( ) * 0.15,
                       calibrate.getHeight() * 0.6);
    inMinSlider.setBounds (reverse.getRight() + getWidth() * 0.1,
                           reverse.getY(),
                           getWidth() * 0.18,
                           getHeight() * 0.15);
    outMinSlider.setBounds (inMinSlider.getRight() + getWidth() * 0.13,
                            inMinSlider.getY(),
                            inMinSlider.getWidth(),
                            inMinSlider.getHeight());
    inMaxSlider.setBounds (inMinSlider.getX(),
                           reverse.getBottom() + reverse.getHeight() * 0.5,
                           inMinSlider.getWidth(),
                           inMinSlider.getHeight());
    outMaxSlider.setBounds (outMinSlider.getX(),
                            inMaxSlider.getY(),
                            outMinSlider.getWidth(),
                            outMinSlider.getHeight());
    mmSlider.setBounds (calibrate.getX(),
                        calibrate.getBottom() + 25,
                        getWidth() * 0.95,
                        getHeight() * 0.2);
}

void Rescale::buttonClicked (juce::Button *button)
{
    if (button == &calibrate)
    {
        setCalibrate();
    }
}

void Rescale::sliderValueChanged (juce::Slider *slider)
{
    if (slider == &mmSlider)
    {
        outMin = mmSlider.getMinValue();
        outMax = mmSlider.getMaxValue();
        outMaxSlider.setValue (outMax);
        outMinSlider.setValue (outMin);
    }
    else if (slider == &outMinSlider)
    {
        outMin = outMinSlider.getValue();
		mmSlider.setMinValue (outMin);
    }
    
    if (slider == &outMaxSlider)
    {
        outMax = outMaxSlider.getValue();
		mmSlider.setMaxValue (outMax);
	}
    else if (slider == &inMinSlider)
    {
        inMin = inMinSlider.getValue();
    }
    
    if (slider == &inMaxSlider)
    {
        inMax = inMaxSlider.getValue();
    }
}

void Rescale::setReverse (bool Status)
{
    reverse.setToggleState (Status, dontSendNotification);
    reverse.setState (juce::Button::buttonDown);
}

void Rescale::setLabelWidget (juce::String LabelWidget)
{
    labelWidget = LabelWidget;
    mmSliderLabel.setText (labelWidget, dontSendNotification);
}

void Rescale::setTargetValue (float TargetValue)
{
    targetValue = TargetValue;
}

void Rescale::setCalibrate()
{
    offset = scaled;
    test = 1;
}

void Rescale::setMin (float Value)
{
    mmSlider.setMinValue (Value);
}

void Rescale::setMax (float Value)
{
    mmSlider.setMaxValue (Value);
}

void Rescale::setValue (float Value)
{
    input = Value;
    
    // scale between 0 and 1
    scaled = (input + PI) / (2 * PI); // scale input from -PI,PI to 0,1
    
    // limit input
	calibrated = std::max (scaled, inMin); // limit lower values
	calibrated = std::min (scaled, inMax); // limit maximum values

    // calibrate
    input1 = 1 - offset;
    calibrated = scaled + input1;
    calibrated = calibrated + (targetValue * test);
    
    // mod
    calibrated = calibrated * 10000000;
    calibrated = (float)((int) (calibrated) % (int)10000000);
    calibrated = calibrated * 0.0000001;

    // reverse velues
    if (reverse.getToggleStateValue() == true){
        calibrated = 1 - calibrated;
    }
    
    // scale output
    calibrated = jmap (calibrated, inMin, inMax, outMin, outMax);
    mmSlider.setValue (calibrated);
}

float Rescale::getValue()
{
    return calibrated;
}
