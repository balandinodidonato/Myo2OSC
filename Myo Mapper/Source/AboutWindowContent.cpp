#include "AboutWindowContent.h"

AboutWindowContent::AboutWindowContent ()
{
    addAndMakeVisible (appName = new Label ("appName",
                                            TRANS ("MyoMapper")));
    appName->setFont (Font (22.00f, Font::plain));
    appName->setJustificationType (Justification::centred);
    appName->setEditable (false, false, false);
    appName->setColour (TextEditor::textColourId, Colours::black);
    appName->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (image = new ImageComponent());
    image->setName ("image");

    addAndMakeVisible (version = new Label ("version",
                                            TRANS ("Version 2.5.13")));
    version->setFont (Font (12.00f, Font::plain));
    version->setJustificationType (Justification::centred);
    version->setEditable (false, false, false);
    version->setColour (TextEditor::textColourId, Colours::black);
    version->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (copyright = new Label ("copyright",
                                                CharPointer_UTF8 ("\xc2\xae")));
    copyright->setFont (Font (11.00f, Font::plain));
    copyright->setJustificationType (Justification::centredLeft);
    copyright->setEditable (false, false, false);
    copyright->setColour (TextEditor::textColourId, Colours::black);
    copyright->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (date = new Label ("date",
                                         TRANS ("2017")));
    date->setFont (Font (11.00f, Font::plain));
    date->setJustificationType (Justification::centredRight);
    date->setEditable (false, false, false);
    date->setColour (TextEditor::textColourId, Colours::black);
    date->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (hyperlinkButton = new HyperlinkButton (TRANS ("Balandino Di Donato"),
                                                              URL ("http://www.balandinodidonato.com/")));
    hyperlinkButton->setTooltip (TRANS ("balandinoDiDonato\n"));
    hyperlinkButton->setButtonText (TRANS ("Balandino Di Donato"));


    setSize (1000, 200);

    MemoryInputStream stream (BinaryData::icon_png, BinaryData::icon_pngSize, false);
    image->setImage (ImageFileFormat::loadFrom (stream));
}

AboutWindowContent::~AboutWindowContent()
{
    appName = nullptr;
    image = nullptr;
    version = nullptr;
    copyright = nullptr;
    date = nullptr;
    hyperlinkButton = nullptr;
}

void AboutWindowContent::paint (Graphics& g)
{
    g.fillAll (Colours::lightgrey);
}

void AboutWindowContent::resized()
{
    appName->setBounds (25, 8, 150, 24);
    version->setBounds (appName->getX(), appName->getBottom() + 3, 150, 17);
    image->setBounds (getWidth() * 0.24, version->getBottom() + 5, getWidth() * 0.5, getHeight() * 0.5);
    copyright->setBounds (appName->getX()-6, image->getBottom()+3, 23, 23);
    hyperlinkButton->setBounds (copyright->getRight(), copyright->getY() + 3, 105, 17);
    date->setBounds (hyperlinkButton->getRight(), copyright->getY(), 36, 23);
}
