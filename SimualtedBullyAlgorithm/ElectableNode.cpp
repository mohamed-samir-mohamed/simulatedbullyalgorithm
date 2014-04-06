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
}

ElectableNode::ElectableNode( ID fID ): mCurrentState(NONE)
{
    mID = fID;
    mCoordinatorID = InvalidValue;
    mPickedTimeRequestElection = InvalidValue;
    mPreviousIntervalTime = InvalidValue;
    mPickedTimeAtChekingCoordinator = InvalidValue;
}

ElectableNode::~ElectableNode()
{
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
    Message message(Message::ELECTION_REQUEST, mID);
    MessageRouter::broadCastMessage(message);
}

void ElectableNode::handleMessage( Message fMessage )
{
    Message responseMessage;
    responseMessage.senderID = mID;

    switch (fMessage.type)
    {
    case  Message::I_AM_JUST_CHECKING_COORDINATOR:
        if (mCurrentState == COORDIATOR)
        {
           responseMessage.type = Message::I_AM_STILL_ALIVE;
           MessageRouter::sendMessageTo(responseMessage, fMessage.senderID);
        }
        break;
    case  Message::I_AM_THE_COORDINATOR:
        mCoordinatorID = fMessage.senderID;
        mCurrentState = NONE;
        break;
    case  Message::ELECTION_REQUEST:
        if (fMessage.senderID < mID)
        {     
            responseMessage.type = Message::I_AM_GREATER_THAN_YOU;
            MessageRouter::sendMessageTo(responseMessage, fMessage.senderID);
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
    Message message(Message::I_AM_THE_COORDINATOR,mID);
    MessageRouter::broadCastMessage(message);
}

void ElectableNode::checkCoordinator()
{
    mCurrentState = CHECKING_COORINATOR;
    Message message(Message::I_AM_JUST_CHECKING_COORDINATOR, mID);
    MessageRouter::sendMessageTo(message, mCoordinatorID);
}

ID ElectableNode::getCoordinatorID() const
{
    return mCoordinatorID;
}

void ElectableNode::setID( ID fID )
{
    mID = fID;
}
