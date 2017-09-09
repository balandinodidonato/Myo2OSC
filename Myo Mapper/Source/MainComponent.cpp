#include "MainComponent.h"
#include "MyoData.h"

MainComponent::MainComponent()
:
selectedMyoID(0),
menuBar(this)
{
    
#if JUCE_MAC
    if (MenuBarModel::getMacMainMenu() != nullptr)
    {
        MenuBarModel::setMacMainMenu (nullptr);
        menuBar.setModel (this);
    }
    else
    {
        menuBar.setModel (nullptr);
        MenuBarModel::setMacMainMenu (this);
    }
#endif
    
    setSize(getParentWidth()*0.4, getParentHeight());

    addAndMakeVisible(&menuBar);

    getLookAndFeel().setUsingNativeAlertWindows(true);
    
    addAndMakeVisible(orientation);
    orientation.setVisible (false);
    addAndMakeVisible(settingsPannel);
    addAndMakeVisible(pose);
    pose.setVisible (false);
    
    settingsPannel.showOrientation.addListener (this);
    settingsPannel.showPose.addListener (this);
    
    settingsPannel.myoList.addListener(this);
    myoManager.connect();
    myoManager.startPoll();
    
    osc.setSender(settingsPannel.getHostAddress(), settingsPannel.getOSCPortSender());
    osc.connectSender();
    osc.connectReceiver();
    
    startTimer(25);
}

MainComponent::~MainComponent()
{
    #if JUCE_MAC
        MenuBarModel::setMacMainMenu (nullptr);
    #endif
    PopupMenu::dismissAllActiveMenus();
}

void MainComponent::paint(juce::Graphics &g)
{
    
    if(settingsPannel.getOSCsettingsStatusSender())
    {
        osc.disconnectSender();
        osc.setSender(settingsPannel.getHostAddress(), settingsPannel.getOSCPortSender());
        settingsPannel.setOSCsettingsStatusSender(false);
        osc.connectSender();
    }

    if(settingsPannel.getOSCsettingsStatusReceiver())
    {
        osc.disconnectReceiver();
        osc.setReceiver(settingsPannel.getOSCPortReceiver());
        settingsPannel.setOSCsettingsStatusReceiver(false);
        osc.connectReceiver();
    }
    
    g.fillAll(Colours::grey);
}

void MainComponent::resized()
{
    #if JUCE_MAC
        settingsPannel.setBounds(10, 10, getRight()-20, getHeight()*0.19-10);
    #else
        menuBar.setBounds(0, 0, getWidth(), 20);
        settingsPannel.setBounds(10, menuBar.getBottom()+10, getRight()-20, getHeight()*0.19-10);
    #endif
    
    if(settingsPannel.getShowOrientation()==1)
    orientation.setBounds(settingsPannel.getX(), settingsPannel.getBottom()+5, settingsPannel.getWidth(), ((getHeight()*0.63)-15));
    else orientation.setBounds(0, 0, 0, 0);
        
    if(settingsPannel.getShowPose()==1 && settingsPannel.getShowOrientation()==1)
        pose.setBounds(orientation.getX(), orientation.getBottom()+10, settingsPannel.getWidth()*settingsPannel.getShowPose(), (getHeight()*0.12));
    else if(settingsPannel.getShowPose()==1 && settingsPannel.getShowOrientation()==0)
        pose.setBounds(settingsPannel.getX(), settingsPannel.getBottom()+5, settingsPannel.getWidth(), (getHeight()*0.12));
    else pose.setBounds(0, 0, 0, 0);
}

// Function checking if ToggleButton was pressed to update GUI
void MainComponent::buttonClicked (Button* button)
{
    if (button == &settingsPannel.showOrientation)
    {
        if (orientation.isVisible() == true)
        {
            orientation.setVisible (!orientation.isVisible());
            orientation.setBounds (0, 0 ,0,0 );
            pose.setBounds (settingsPannel.getX(),
                            settingsPannel.getBottom() + 5,
                            settingsPannel.getWidth(),
                            getHeight() * 0.12);
        }
        else if (orientation.isVisible() == false)
        {
            orientation.setVisible (!orientation.isVisible());
            orientation.setBounds (settingsPannel.getX(),
                                   settingsPannel.getBottom() + 5,
                                   settingsPannel.getWidth(),
                                   (getHeight() * 0.63) - 15);
            pose.setBounds (orientation.getX(),
                            orientation.getBottom() + 10,
                            settingsPannel.getWidth() * settingsPannel.getShowPose(),
                            getHeight() * 0.12);
        }
    }
    
    if (button == &settingsPannel.showPose)
    {
        if (pose.isVisible() == true)
        {
            pose.setVisible (!pose.isVisible());
            pose.setBounds (0, 0, 0, 0);
        }
        else if (pose.isVisible() == false)
        {
            pose.setVisible (!pose.isVisible());
            if (orientation.isVisible() == true)
            {
                pose.setBounds (orientation.getX(),
                                orientation.getBottom() + 10,
                                settingsPannel.getWidth() * settingsPannel.getShowPose(),
                                getHeight() * 0.12);
            }
            else if (orientation.isVisible() == false)
            {
                pose.setBounds (settingsPannel.getX(),
                                settingsPannel.getBottom() + 5,
                                settingsPannel.getWidth(),
                                getHeight() * 0.12);
            }
        }
    }
}

void MainComponent::timerCallback()
{
    bool success = false;
    std::vector<MyoData> myoData = myoManager.getMyoData(success);
    
    if (!success) return;
    
    if (!selectedMyoID || selectedMyoID >= myoData.size()) return;

    uint8 id = selectedMyoID;
        
    orientation.setValues(myoData[id].orientationRaw);
    pose.setPoseLabel(myoData[id].pose+" - "+String(myoData[id].poseID));

    osc.sendOSC(id, myoData[id].emgRaw, myoData[id].emgScaled, myoData[id].emgScaledAbs, myoData[id].quaternion, myoData[id].mav,  myoData[id].mavWfL, myoData[id].gyro, myoData[id].gyroWfL, myoData[id].gyroScaled, myoData[id].gyroScaledWfL, myoData[id].acceleration, myoData[id].accelerationWfL, myoData[id].accelerationScaled, myoData[id].accelerationScaledWfL, myoData[id].orientationRaw, orientation.getValue(), orientation.getWl(), myoData[id].pose, myoData[id].poseID);
 
    
    if(osc.vibrate)
    {
        myoManager.vibrate(osc.vibrationType);
        osc.vibrate = false;
    }

    for(int i=0; i<4; i++) // myoData
        for(int y=1; y<5; y++) // action
            if(osc.map[i][y])
            {
                orientation.map(i, y, osc.value, osc.reverseStatus);
                osc.map[i][y] = false;
            }
}

void MainComponent::disconnectMyoAndOSC()
{
    myoManager.disconnect();
    osc.disconnectSender();
    osc.disconnectReceiver();
}


void MainComponent::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged == &settingsPannel.myoList)
	{
		selectedMyoID = settingsPannel.myoList.getSelectedId();
		osc.setMyoIdReceiver(selectedMyoID);
	}

}

// ===================================================================
//                          MENU BAR
// ===================================================================

StringArray MainComponent::getMenuBarNames()
{
    const char* menuNames[] = {"Help", nullptr};
    return StringArray (menuNames);
}

PopupMenu MainComponent::getMenuForIndex(int index, const String& name)
{
    PopupMenu menu;
    if (name == "Help")
    {
        menu.addItem(AboutMyoMapper, "About Myo Mapper");
        menu.addItem(onlineDocumentation, "Online Documentation");
    }
    return menu;
}

void MainComponent::menuItemSelected(int menuID, int index)
{
        if (menuID == AboutMyoMapper) AboutMyoMapperDialogWindow();
        else if (menuID == onlineDocumentation) HelpDialogWindow();
}

void MainComponent::AboutMyoMapperDialogWindow()
{
    AboutWindow* aboutWindow = new AboutWindow ("About Myo Mapper", Colours::grey, DocumentWindow::closeButton);
    aboutWindow->addToDesktop();
    
    Rectangle<int> area (0, 0, 200, 200);
    
    RectanglePlacement placement ((true ? RectanglePlacement::xMid
                                   : RectanglePlacement::xRight)
                                  | RectanglePlacement::yMid
                                  | RectanglePlacement::doNotResize);
    
    Rectangle<int> result (placement.appliedTo (area, Desktop::getInstance().getDisplays()
                                                .getMainDisplay().userArea.reduced (20)));
    aboutWindow->setBounds (result);
    
    aboutWindow->setResizable(false, false);
    aboutWindow->setUsingNativeTitleBar (true);
    aboutWindow->setVisible (true);
}

void MainComponent::HelpDialogWindow()
{
    HelpWindow* helpWindow = new HelpWindow ("Online Documentation", Colours::grey, DocumentWindow::closeButton);
    helpWindow->addToDesktop();
    
    Rectangle<int> area (0, 0, 300, 150);
    
    RectanglePlacement placement ((true ? RectanglePlacement::xMid
                                   : RectanglePlacement::xRight)
                                  | RectanglePlacement::yMid
                                  | RectanglePlacement::doNotResize);
    
    Rectangle<int> result (placement.appliedTo (area, Desktop::getInstance().getDisplays()
                                                .getMainDisplay().userArea.reduced (20)));
    helpWindow->setBounds (result);
    
    helpWindow->setResizable(false, false);
    helpWindow->setUsingNativeTitleBar (true);
    helpWindow->setVisible (true);
}
