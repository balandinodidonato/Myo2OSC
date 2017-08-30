#include "../JuceLibraryCode/JuceHeader.h"
#include "HelpWindow.h"

//==============================================================================
HelpWindow::HelpWindow (const String& name, Colour backgroundColour, int buttonsNeeded)
:   DocumentWindow (name, backgroundColour, buttonsNeeded),
    wiki ("Wiki", URL ("https://github.com/balandinodidonato/MyoMapper/wiki")),
    sourceCode ("Source Code", URL ("https://github.com/balandinodidonato/MyoMapper/")),
    support("Ask the developper", URL ("info@balandinodidonato.com"))
{
    juce::Component::addAndMakeVisible (wiki);
    juce::Component::addAndMakeVisible (sourceCode);
    juce::Component::addAndMakeVisible (support);
}

HelpWindow::~HelpWindow()
{
}

void HelpWindow::paint (Graphics& g)
{
    g.fillAll (Colours::grey);   // clear the background
}

void HelpWindow::resized()
{
    int space = getHeight() * 0.1;
        
    wiki.setFont (juce::Font::plain, true);
    wiki.setBounds (0, space, getWidth(), getHeight() * 0.2);
    
    sourceCode.setFont (juce::Font::plain, true);
    sourceCode.setBounds (wiki.getX(), wiki.getBottom() + space, wiki.getWidth(), wiki.getHeight());
    
    support.setFont (juce::Font::plain, true);
    support.setBounds (wiki.getX(), sourceCode.getBottom() + space, wiki.getWidth(), wiki.getHeight());
}

void HelpWindow::closeButtonPressed()
{
    delete this;
}
