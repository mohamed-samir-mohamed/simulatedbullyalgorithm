#ifndef ABSTRACTELETABLE_H
#define ABSTRACTELETABLE_H

#include "MessageInterfaces.h"
#include "GeneralDefinations.h"
#include "Message.h"

class ElectableNode: public IMessageHandler
{
public:    
    enum State
    {
        WAITING_ELECTION_RESPOND, COORDIATOR, CHECKING_COORINATOR, NONE
    };
   ElectableNode();
   ElectableNode(ID fID);
   virtual ~ElectableNode();
   ID getID() const;
   ID getCoordinatorID() const;
   virtual void handleMessage(Message* fMessage);
   virtual void update();

protected:
    State mCurrentState;
    ID mID;
    ID mCoordinatorID;
    double mPickedTimeRequestElection;
    double mPreviousIntervalTime;
    double mPickedTimeAtChekingCoordinator;


    void RequestElection();
    void prepareMessageToFire(Message::MessageType fType, void* fData = nullptr);
    void checkCoordinator();
    void setAsCoordinator();

private:
    Message* mMessageToFire;
    static ID CurrentUniqueID;
};

#endif //! ABSTRACTELETABLE_H