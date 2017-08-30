#include "MyoManager.h"
#include "MyoListener.h"

MyoManager::MyoManager()
:   Thread ("Myo Data Thread"),
    hub (nullptr),
    myo (nullptr)
{
    
}

MyoManager::~MyoManager()
{
    stopPoll();
    disconnect();
}

bool MyoManager::connect()
{
    disconnect();
    bool isConnected = false;
    
    try
    {
        if ((hub = new myo::Hub("com.yourcompany.MyoMapper")))
        {
            std::cout << "Attempting to find a Myo..." << std::endl;
            myo = hub->waitForMyo (10000);
            listener.knownMyos.push_back (myo);
            myo->setStreamEmg (myo::Myo::streamEmgEnabled);
            myo->unlock (myo::Myo::unlockHold);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "Error",
                                          e.what(),
                                          "OK");
        
    }
    
    if (myo)
    {
        hub->addListener (&listener);
    }
    else
    {
        std::cerr << "Error: Myo not found" << std::endl;
        disconnect();
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "Myo not found",
                                          "Please, conncect the Myo armband and relaunch Myo Mapper.",
                                          "OK");
        
    }

    return isConnected;
}

void MyoManager::run()
{
    if (!myo) return;
    
    while (!threadShouldExit())
    {
        hub->runOnce (20);
        
        if (tryEnterWrite())
        {
            myoData = listener.getMyoData();
            exitWrite();
        }
    }
}

std::vector<MyoData> MyoManager::getMyoData (bool &success) const
{
    std::vector<MyoData> dataCopy;
    
    if (tryEnterRead())
    {
        dataCopy = myoData;
        success = true;
        exitRead();
    }
    
    return dataCopy;
}

void MyoManager::disconnect()
{
    myo = nullptr;
    
    if (!hub)
    {
        return;
    }
    stopPoll();
    // It's safe to call removeListener() for a not-added listener
    hub->removeListener (&listener);
    delete hub;
    hub = nullptr;
}

void MyoManager::startPoll() 
{
    startThread();
}

void MyoManager::stopPoll()
{
    stopThread (1000);
}

void MyoManager::vibrate(String VibrationType)
{
    if(VibrationType == "long")
        myo->vibrate (myo::Myo::vibrationLong);
    if(VibrationType == "medium")
        myo->vibrate (myo::Myo::vibrationMedium);
    if(VibrationType == "short")
        myo->vibrate (myo::Myo::vibrationShort);
}
