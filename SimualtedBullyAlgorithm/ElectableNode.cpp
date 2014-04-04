#include "ElectableNode.h"
#include <ctime>
#include "MessageRouter.h"
#include <iostream>
using namespace std;
ID ElectableNode::CurrentUniqueID = 0;

ElectableNode::ElectableNode() : mCurrentState(NONE)
{
    mID = CurrentUniqueID;
    CurrentUniqueID++;
    mCoordinatorID = InvalidValue;
    mPickedTimeRequestElection = InvalidValue;
    mPreviousIntervalTime = InvalidValue;
    mPickedTimeAtChekingCoordinator = InvalidValue;
    mMessageToFire = nullptr;
}

ElectableNode::~ElectableNode()
{
    delete mMessageToFire;
}

ID ElectableNode::getID() const
{
    return mID;
}

void ElectableNode::RequestElection()   
{
    cout << "node   "<< mID <<"  requests election "<<endl;

    mPickedTimeRequestElection = time(0);
    mCurrentState = WAITING_ELECTION_RESPOND;
    prepareMessageToFire(Message::ELECTION_REQUEST);
    MessageRouter::broadCastMessage(mMessageToFire);
}

void ElectableNode::handleMessage( Message* fMessage )
{

    switch (fMessage->getType())
    {
    case  Message::I_AM_JUST_CHECKING_COORDINATOR:
        if (mCurrentState == COORDIATOR)
        {
            fMessage->setHandled();
            prepareMessageToFire(Message::I_AM_STILL_ALIVE);
            MessageRouter::sendMessageTo(mMessageToFire ,fMessage->getSenderID());
        }
        break;
    case  Message::I_AM_THE_COORDINATOR:
        mCoordinatorID = fMessage->getSenderID();
        mCurrentState = NONE;
        break;
    case  Message::ELECTION_REQUEST:
        if (fMessage->getSenderID() < mID)
        {
            prepareMessageToFire(Message::I_AM_GREATER_THAN_YOU);
            MessageRouter::sendMessageTo(mMessageToFire, fMessage->getSenderID());
            RequestElection();
        }
        break;
    case  Message::I_AM_GREATER_THAN_YOU:
        mCurrentState = NONE;
        break;
    case  Message::I_AM_STILL_ALIVE:
        mCurrentState = NONE;
        break;
    }
}

void ElectableNode::update()
{
    //this method will be called in a tick time loop
    if (mCurrentState == WAITING_ELECTION_RESPOND )
    {
        double waitingTime = time(0) - mPickedTimeRequestElection;
        if (waitingTime >= TimeOutLimit)
        {
            setAsCoordinator();
        }
    }

    if (mCurrentState == CHECKING_COORINATOR)
    {
        double timeWaiting = time(0) - mPickedTimeAtChekingCoordinator;
        if (timeWaiting >= TimeOutLimit)
        {
            RequestElection();
        }
    }

    //each time stamp check coordinator availability.
    double intervalOffset = time(0) - mPreviousIntervalTime;
    if (intervalOffset >= timeStampLimit && mCurrentState != COORDIATOR && mCurrentState != CHECKING_COORINATOR)
    {
        mPreviousIntervalTime = time(0);
        mPickedTimeAtChekingCoordinator = time(0);
        checkCoordinator();
    }
}

void ElectableNode::setAsCoordinator()
{
    cout<<"Current Coordinator    "<< mID <<endl;
    mCurrentState = COORDIATOR;
    mCoordinatorID = mID;
    prepareMessageToFire(Message::I_AM_THE_COORDINATOR);
    MessageRouter::broadCastMessage(mMessageToFire);
}

void ElectableNode::prepareMessageToFire( Message::MessageType fType, void* fData /*= nullptr*/ )
{
    if (mMessageToFire)
        delete mMessageToFire;

    mMessageToFire = new Message(fType, mID, fData);
}

void ElectableNode::checkCoordinator()
{
    mCurrentState = CHECKING_COORINATOR;
    prepareMessageToFire(Message::I_AM_JUST_CHECKING_COORDINATOR);
    MessageRouter::sendMessageTo(mMessageToFire, mCoordinatorID);
}

ID ElectableNode::getCoordinatorID() const
{
    return mCoordinatorID;
}
