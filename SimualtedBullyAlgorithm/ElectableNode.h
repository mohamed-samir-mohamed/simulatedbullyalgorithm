#ifndef ABSTRACTELETABLE_H
#define ABSTRACTELETABLE_H

#include "MessageInterfaces.h"
#include "GeneralDefinations.h"
#include "Message.h"

class MessageRouter;

class ElectableNode: public IMessageHandler
{
    /************************************************************************/
    /* this class is the basic atom in this system.
    every process will compose an object from this class and sends any messages received from other processes to it 
    and the messages sent by any process will be initiated here.
    the logic of election maintained here*/
    /************************************************************************/
public:    
    enum State
    {
        WAITING_ELECTION_RESPOND,
        COORDIATOR,
        CHECKING_COORINATOR,
        NONE
    };
   ElectableNode();
   ElectableNode(ID fID);
   virtual ~ElectableNode();
   ID getID() const;
   ID getCoordinatorID() const;
   void setID(ID fID);
   /************************************************************************/
   /* this method handles any message received by the process              */
   /************************************************************************/
   virtual void handleMessage(Message fMessage);
   /************************************************************************/
   /* this method called every in a while loop in winMain method */
   /************************************************************************/
   virtual void update();

protected:
    State mCurrentState; //each node is a state saving unit - states helps in handling logic -.
    ID mID; //the unique ID for the process.
    ID mCoordinatorID;//the coordinator ID - some times equals to mID - when this node is the coordinator.
    double mPickedTimeRequestElection;//the time picked at the moment of requesting election (time out will set this node as coordinator).
    double mPreviousIntervalTime;//the time picked since last time checking for coordinator existence.
    double mPickedTimeAtChekingCoordinator;

    void RequestElection();
    void checkCoordinator();
    void setAsCoordinator();

private:
	//message router member for routing messages.
	MessageRouter* mMessageRouter;
};

#endif //! ABSTRACTELETABLE_H