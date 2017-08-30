#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Settings  :   public Component,
                    private Label::Listener,
                    private Slider::Listener
{
public:
    Settings();
    ~Settings();

    void paint (Graphics& g) override;
    void resized() override;
    
    void labelTextChanged (juce::Label *labelThatHasChanged) override;
    void sliderValueChanged (juce::Slider *slider) override;
    
    void setOSCsettingsStatusSender (bool OSCsettingsStatusSender);
    void setOSCsettingsStatusReceiver (bool OSCsettingsStatusReceiver);
    
    void vibrate();
    
    ComboBox myoList;
    
    bool getShowOrientation();
    bool getShowPose();
    int getShowMav();
    
    bool getOSCsettingsStatusSender();
    bool getOSCsettingsStatusReceiver();

    int getOSCPortSender();
    int getOSCPortReceiver();
    String getHostAddress();
    int getSelectedMyo();
    
private:
    Slider oscPortSliderSender;
    Slider oscPortSliderReceiver;

    Label oscPortLabelSender;
    Label oscPortLabelReceiver;
    
    Label hostAddressTitle;
    Label setHostAddress;
    
    ToggleButton showOrientation;
    ToggleButton showMav;
    ToggleButton showPose;
    
    String hostAddress;
    
    bool oscSettingsChangedSender;
    bool oscSettingsChangedReceiver;
    
    int oscPortSender;
    int oscPortReceiver;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Settings)
};


#endif  // SETTINGS_H_INCLUDED
