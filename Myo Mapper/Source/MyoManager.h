#ifndef MYOMANAGER_H_INCLUDED
#define MYOMANAGER_H_INCLUDED

#include "MyoListener.h"
#include "MyoData.h"

class MyoManager    :   private Thread,
                        private ReadWriteLock
{
public:
    MyoManager();
    ~MyoManager();
    
    bool connect();
    void disconnect();
    bool isConnected() const;
    
    void startPoll();
    void stopPoll();
    
    void vibrate (String VibrationType);
        
    std::vector<MyoData> getMyoData (bool &success) const;

private:
    
    void run() override;
    MyoListener listener;
    std::vector<MyoData> myoData;
    int numberOfMyos;
    
    myo::Hub *hub;
    myo::Myo *myo;
};

#endif  // MYOMANAGER_H_INCLUDED
