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
    cout << "node "<< mID <<"requests election "<<endl;

    mPickedTimeRequestElection = time(0);//review.
    mCurrentState = WAITING_ELECTION_RESPOND;
    prepareMessageToFire(Message::ELECTION_REQUEST);
    MessageRouter::broadCastMessage(mMessageToFire);
}

void ElectableNode::handleMessage( Message* fMessage )
{
    if (fMessage->getType() == Message::I_AM_THE_COORDINATOR)
    {
        mCoordinatorID = fMessage->getSenderID();
        mCurrentState = NONE;
    }
    else if ( (fMessage->getType() == Message::ELECTION_REQUEST) && (mCurrentState == COORDIATOR) )
    {
        if(mID < fMessage->getSenderID())//if the node that requested an election greater than me don't respond.
            return;
        fMessage->setHandled();
        prepareMessageToFire(Message::I_AM_STILL_HERE);
        MessageRouter::sendMessageTo(mMessageToFire, fMessage->getSenderID());
    }
    else if ((fMessage->getType() == Message::ELECTION_REQUEST) && (mCurrentState == NONE))
    {
        if (mID > fMessage->getSenderID())
        {
            fMessage->setHandled();
            prepareMessageToFire(Message::I_AM_GREATER_THAN_YOU);
            MessageRouter::sendMessageTo(mMessageToFire, fMessage->getSenderID());
            RequestElection();
        }
    }
    else if ( (fMessage->getType() == Message::I_AM_STILL_HERE) && (mCurrentState == WAITING_ELECTION_RESPOND) )
    {
        mCurrentState = NONE;
    }
    else if ( (fMessage->getType() == Message::I_AM_STILL_HERE) && (mCurrentState == CHECKING_COORINATOR) )
    {
        mCurrentState = NONE;
    }
    else if ( (fMessage->getType() == Message::I_AM_GREATER_THAN_YOU) && (mCurrentState == WAITING_ELECTION_RESPOND) )
    {
        mCurrentState = NONE;
    }
}

void ElectableNode::update()
{
    //this method will be called in a tick time loop
    if (mCurrentState == WAITING_ELECTION_RESPOND || mCurrentState == CHECKING_COORINATOR)
    {
        double timeOffset = time(0) - mPickedTimeRequestElection;//review
        if (timeOffset >= TimeOutLimit)
        {
            setAsCoordinator();
        }
    }


    if (mCurrentState == CHECKING_COORINATOR)
    {
        double timeOffset = time(0) - mPickedTimeAtChekingCoordinator;//review
        if (timeOffset >= TimeOutLimit)
        {
            RequestElection();
        }
    }

    //each time stamp check coordinator availability.
    double intervalOffset = time(0) - mPreviousIntervalTime;
    if (intervalOffset >= timeStampLimit && mCurrentState != COORDIATOR )
    {
        mPreviousIntervalTime = time(0);
        mPickedTimeAtChekingCoordinator = time(0);//review
        checkCoordinator();
    }
}

void ElectableNode::setAsCoordinator()
{
    cout<<"Current Coordinator"<< mID <<endl;
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
    prepareMessageToFire(Message::ELECTION_REQUEST);
    MessageRouter::sendMessageTo(mMessageToFire, mCoordinatorID);
}