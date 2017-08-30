#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"

Settings::Settings()
:   hostAddress("127.0.0.1"),
    oscSettingsChangedSender(false),
    oscSettingsChangedReceiver(false),
    oscPortSender (5432),
    oscPortReceiver (5431)
{
    oscPortSliderSender.setRange (0, 9999, 1);
    oscPortSliderSender.setValue (5432);
    oscPortSliderSender.setIncDecButtonsMode (juce::Slider::incDecButtonsDraggable_Vertical);
    oscPortSliderSender.setSliderStyle (juce::Slider::IncDecButtons);
    addAndMakeVisible (oscPortSliderSender);
    
    oscPortLabelSender.setText ("OSC Port Sender", dontSendNotification);
    oscPortLabelSender.attachToComponent (&oscPortSliderSender, true);
    addAndMakeVisible (oscPortLabelSender);
    oscPortSliderSender.addListener (this);
    
    hostAddressTitle.setText ("Host Address:", dontSendNotification);
    addAndMakeVisible (hostAddressTitle);
    
    setHostAddress.setText (hostAddress, dontSendNotification);
    setHostAddress.setEditable (true);
    addAndMakeVisible (setHostAddress);
    setHostAddress.addListener (this);
    
    oscPortSliderReceiver.setRange (0, 9999, 1);
    oscPortSliderReceiver.setValue (5431);
    oscPortSliderReceiver.setIncDecButtonsMode (juce::Slider::incDecButtonsDraggable_Vertical);
    oscPortSliderReceiver.setSliderStyle (juce::Slider::IncDecButtons);
    addAndMakeVisible (oscPortSliderReceiver);
    
    oscPortLabelReceiver.setText ("OSC Port Receiver", dontSendNotification);
    oscPortLabelReceiver.attachToComponent (&oscPortSliderReceiver, true);
    addAndMakeVisible (oscPortLabelReceiver);
    oscPortSliderReceiver.addListener (this);
    
    myoList.setText ("Select Myo");
    addAndMakeVisible (myoList);
    
    myoList.addItem ("Myo n. 1", 1);
    myoList.addItem ("Myo n. 2", 2);
    myoList.addItem ("Myo n. 3", 3);
    myoList.addItem ("Myo n. 4", 4);
    
    showOrientation.setButtonText ("Show Orientation");
    addAndMakeVisible (showOrientation);
    showPose.setButtonText ("Show Pose");
    addAndMakeVisible (showPose);
}

Settings::~Settings()
{
}

void Settings::paint (Graphics& g)
{
    g.fillAll (Colours::lightgrey);   // clear the background
    g.setColour (Colours::grey);
    g.drawRoundedRectangle (0, 0, getWidth(), getHeight(), 5, 5);
    
    g.setColour (Colours::white);
    g.fillRoundedRectangle (10,
                            getHeight() * 0.15,
                            getRight() - 30,
                            getHeight() * 0.795,
                            5);
    
    g.setColour (Colours::black);
    g.setFont (getHeight()*0.13);
    g.drawText ("Settings", getLocalBounds(), Justification::centredTop, true);
}

void Settings::resized()
{
    hostAddressTitle.setBounds (20, 20, getWidth() * 0.18, getHeight() * 0.28);
    setHostAddress.setBounds (hostAddressTitle.getRight() + getWidth() * 0.01,
                              hostAddressTitle.getY() + hostAddressTitle.getHeight() * 0.25,
                              getWidth() * 0.2,
                              hostAddressTitle.getHeight() * 0.5);
    myoList.setBounds (getWidth() * 0.5,
                       setHostAddress.getY(),
                       getWidth() * 0.465,
                       setHostAddress.getHeight());
    
    oscPortSliderSender.setBounds (135,
                                   hostAddressTitle.getBottom() + 5,
                                   getWidth() * 0.25,
                                   getHeight() * 0.2);
    oscPortSliderReceiver.setBounds (myoList.getX() + oscPortLabelReceiver.getWidth(),
                                     oscPortSliderSender.getY(),
                                     oscPortSliderSender.getWidth(),
                                     oscPortSliderSender.getHeight());
    
    showOrientation.setBounds (hostAddressTitle.getX(),
                               oscPortSliderSender.getBottom() + 5,
                               getWidth() * 0.3,
                               getHeight() * 0.2);
    showPose.setBounds (showOrientation.getRight() + 10,
                        showOrientation.getY(),
                        showOrientation.getWidth(),
                        showOrientation.getHeight());
}

void Settings::sliderValueChanged (juce::Slider *slider)
{
    if (slider == &oscPortSliderSender)
    {
        oscPortSender = oscPortSliderSender.getValue();
        oscSettingsChangedSender = true;
    }
    
    if (slider == &oscPortSliderReceiver)
    {
        oscPortReceiver = oscPortSliderReceiver.getValue();
        oscSettingsChangedReceiver = true;
    }
}

void Settings::labelTextChanged (juce::Label *labelThatHasChanged)
{
    if (labelThatHasChanged == &setHostAddress)
    {
        hostAddress = setHostAddress.getText();
        oscSettingsChangedSender = true;
        oscSettingsChangedReceiver = true;
    }
}

int Settings::getOSCPortSender()
{
    return oscPortSender;
    oscSettingsChangedSender = false;
}

int Settings::getOSCPortReceiver()
{
    return oscPortReceiver;
}

String Settings::getHostAddress()
{
    return hostAddress;
}

int Settings::getSelectedMyo()
{
    return myoList.getSelectedId() - 1;
}

bool Settings::getShowOrientation()
{
    return showOrientation.getToggleState();
}

bool Settings::getShowPose()
{
    return showPose.getToggleState();
}

bool Settings::getOSCsettingsStatusSender()
{
    return oscSettingsChangedSender;
}

void Settings::setOSCsettingsStatusSender(bool changeStatus)
{
    oscSettingsChangedSender = changeStatus;
}

bool Settings::getOSCsettingsStatusReceiver()
{
    return oscSettingsChangedReceiver;
}

void Settings::setOSCsettingsStatusReceiver(bool changeStatus)
{
    oscSettingsChangedReceiver = changeStatus;
}
