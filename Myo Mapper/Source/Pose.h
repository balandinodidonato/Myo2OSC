#ifndef POSE_H_INCLUDED
#define POSE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Pose  :   public Component
{
public:
    Pose();
    ~Pose();

    void paint (Graphics&);
    void resized();
    void setPoseLabel (String LabelText);
        
private:
    Label poseLabel;    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pose)
};


#endif  // POSE_H_INCLUDED
