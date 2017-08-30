#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

class MyoMapperApplication  :   public JUCEApplication
{
public:
    MyoMapperApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    void initialise (const String& commandLine) override
    {
        mainWindow = new MainWindow (getApplicationName());

    }
    
    void shutdown() override
    {
        mainWindow = nullptr; // (deletes our window)
    }

    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        mainWindow = new MainWindow (getApplicationName());
    }

    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Colours::lightgrey,
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);
            setCentrePosition (getWidth() * 0.5, 0);
            setVisible (true);
            setResizable(true, true);
            setResizeLimits (getParentWidth() * 0.4,
                             getParentHeight() * 0.8,
                             getParentWidth(),
                             getParentHeight() * 0.8);
        }

        void closeButtonPressed() override
        {
            MainComponent().disconnectMyoAndOSC();
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    ScopedPointer<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (MyoMapperApplication)
