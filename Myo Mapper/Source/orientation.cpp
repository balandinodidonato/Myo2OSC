#include "orientation.h"
#include "../modules/juce_core/containers/juce_Array.h"

Orientation::Orientation()
{
    addAndMakeVisible (rescaleYaw);
    addAndMakeVisible (rescalePitch);
    addAndMakeVisible (rescaleRoll);
    rescaleYaw.setLabelWidget ("Yaw");
    rescalePitch.setLabelWidget ("Pitch");
    rescaleRoll.setLabelWidget ("Roll");
    rescaleYaw.setTargetValue (0.5),
    rescalePitch.setTargetValue (0.5);
    rescaleRoll.setTargetValue (0.5);
}

void Orientation::paint (juce::Graphics &g)
{
    g.fillAll (Colours::lightgrey);
    g.setColour (Colours::grey);
    g.drawRoundedRectangle (0, 0, getWidth(), getHeight(), 5, 5);
    g.setColour (Colours::black);
    g.setFont (getHeight() * 0.06);
    g.drawText ("Orientation", getLocalBounds(), Justification::centredTop, true);
}

void Orientation::resized()
{
    rescaleYaw.setBounds (10, getHeight() * 0.07, getRight() - 30, getHeight() * 0.29);
    rescalePitch.setBounds (rescaleYaw.getX(),
                            rescaleYaw.getBottom() + 7,
                            rescaleYaw.getWidth(),
                            rescaleYaw.getHeight());
    rescaleRoll.setBounds (rescaleYaw.getX(),
                           rescalePitch.getBottom() + 7,
                           rescaleYaw.getWidth(),
                           rescaleYaw.getHeight());
}

void Orientation::setValues (Vector3D<float> Orientation)
{
    rescaleYaw.setValue (Orientation.x);
    rescalePitch.setValue (Orientation.y);
    rescaleRoll.setValue (Orientation.z);
}

Vector3D<float> Orientation::getValue()
{
    orientationScaled.x = rescaleYaw.getValue();
    orientationScaled.y = rescalePitch.getValue();
    orientationScaled.z = rescaleRoll.getValue();
    
    return orientationScaled;
}

Vector3D<float> Orientation::getWl(){
    
    orientationWl.set3DValue (orientationScaled);
    return orientationWl.get3DValue();
}

float Orientation::getYaw()
{
    return rescaleYaw.getValue();
}

float Orientation::getPitch()
{
    return rescalePitch.getValue();
}

float Orientation::getRoll()
{
    return rescaleRoll.getValue();
}

// Recal functions from OSC data in input
void Orientation::map (int myoData, int Action, float Value, bool ReverseStatus)
{
    switch (myoData) {
        case 0:
            switch (Action) {
                case 1: rescaleYaw.setCalibrate(); break;
                //case 2: rescaleYaw.setMin(Value); break;
                //case 3: rescaleYaw.setMax(Value); break;
                //case 4: rescaleYaw.setReverse(ReverseStatus); break;
                default:
                    break;
            }
                break;
        case 1:
            switch (Action) {
                case 1: rescalePitch.setCalibrate(); break;
                //case 2: rescalePitch.setMin(Value); break;
                //case 3: rescalePitch.setMax(Value); break;
                //case 4: rescalePitch.setReverse(ReverseStatus); break;
                default:
                    break;
            }
                break;
        case 2:
            switch (Action) {
                case 1: rescaleRoll.setCalibrate(); break;
                //case 2: rescaleRoll.setMin(Value); break;
                //case 3: rescaleRoll.setMax(Value); break;
                //case 4: rescaleRoll.setReverse(ReverseStatus); break;
                default:
                    break;
            }
        default:
            break;
    }
}

