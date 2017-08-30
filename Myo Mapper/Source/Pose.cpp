#include "../JuceLibraryCode/JuceHeader.h"
#include "Pose.h"

Pose::Pose()
{
    addAndMakeVisible (poseLabel);
    poseLabel.setJustificationType (36);
}

Pose::~Pose()
{
}

void Pose::paint (Graphics& g)
{
    g.fillAll (Colours::lightgrey);   // clear the background
    g.setColour (Colours::grey);
    g.drawRoundedRectangle (0, 0, getWidth(), getHeight(), 5, 5);
    
    g.setColour (Colours::white);
    g.fillRoundedRectangle (10, getHeight() * 0.3, getWidth() - 22, getHeight() - 45, 5);
    
    g.setColour (Colours::black);
    g.setFont (getHeight()*0.3);
    g.drawText ("Pose", getLocalBounds(), Justification::centredTop, true);   // draw some placeholder text
}

void Pose::resized()
{
    poseLabel.setFont (getHeight() * 0.2);
    poseLabel.setBounds (getX(), getHeight() * 0.35, getWidth() * 0.95, getHeight() * 0.5);
}

void Pose::setPoseLabel (String LabelText)
{
    poseLabel.setText (LabelText, dontSendNotification);
}
