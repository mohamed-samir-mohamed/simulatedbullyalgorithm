#include "ElectableNode.h"
#include <ctime>
#include "MessageRouter.h"
#include <iostream>
using namespace std;

ElectableNode::ElectableNode() : mCurrentState(NONE)
{
    mID = InvalidValue;
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

    //each node created must initiate election at creation.
    RequestElection();
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
    //pick the current time.  
    mPickedTimeRequestElection = time(0);
    //set the current state of this node to waiting election request
    mCurrentState = WAITING_ELECTION_RESPOND;
    //prepare message to broad cast election request
    Message message(Message::ELECTION_REQUEST, mID);
    //broadCast the message to all windows.
    MessageRouter::broadCastMessage(message);
}

void ElectableNode::handleMessage( Message fMessage )
{
    //prepare response message to respond to current message if needed.
    Message responseMessage;
    responseMessage.senderID = mID;//set the sender ID to me.
    
    //if the sender was me i don't need to handle this message also the message is not handled.
    if (fMessage.senderID == mID || fMessage.isHandled == true)
        return;
    //console some information.   
    cout <<"Node "<< mID <<" received message type "<< fMessage.getStringMessage() <<" from Node" << fMessage.senderID<<endl;

    switch (fMessage.type)
    {
    case  Message::I_AM_JUST_CHECKING_COORDINATOR:
        if (mCurrentState == COORDIATOR)//if I was the coordinator I will respond with I am sill alive.
        {
           responseMessage.type = Message::I_AM_STILL_ALIVE;
           MessageRouter::sendMessageTo(responseMessage, fMessage.senderID);
        }//else no response.
        break;

    case  Message::I_AM_THE_COORDINATOR:
        //if any node sends me a message that I am the coordinator
        //then I will save his ID and my state will be none.
        mCoordinatorID = fMessage.senderID;
        mCurrentState = NONE;
        break;

    case  Message::ELECTION_REQUEST:
        //if an election request sent to me
        if (fMessage.senderID < mID)//if I am greater than the sender ID
        {     
            //response message will be I am greater than you
            responseMessage.type = Message::I_AM_GREATER_THAN_YOU;
            //sent to the sender.
            MessageRouter::sendMessageTo(responseMessage, fMessage.senderID);
            //initiate the election from me.
            RequestElection();
        }
        break;

    case  Message::I_AM_GREATER_THAN_YOU://recall last case 
        //if the sent message was I am greater than you then my previous state was requesting election
        //then set my state to none not to wait for the election request.
        mCurrentState = NONE;
        break;
    case  Message::I_AM_STILL_ALIVE:
        //if I was checking the coordinator and he respond with message I am still alive
        //then my state will be none.
        mCurrentState = NONE;
        break;
    }
}

void ElectableNode::update()
{
    //if I was waiting for election request.
    if (mCurrentState == WAITING_ELECTION_RESPOND )
    {
        //if the waiting time exceeds the time out limit.
        double waitingTime = time(0) - mPickedTimeRequestElection;
        if (waitingTime >= TimeOutLimit)
        {
            //then set my self as coordinator.
            setAsCoordinator();
        }
    }

    //if I was checking coordinator.
    if (mCurrentState == CHECKING_COORINATOR)
    {
        //if he didn't respond before time out limit.
        double timeWaiting = time(0) - mPickedTimeAtChekingCoordinator;
        if (timeWaiting >= TimeOutLimit)
        {
            //then he dies I have to initiate election.
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
    Message message(Message::I_AM_THE_COORDINATOR, mID);
    MessageRouter::broadCastMessage(message);
}

void ElectableNode::checkCoordinator()
{
    //this method called every time stamp interval to check coordinator.
    mCurrentState = CHECKING_COORINATOR;
    Message message(Message::I_AM_JUST_CHECKING_COORDINATOR, mID);
    MessageRouter::broadCastMessage(message);
}

ID ElectableNode::getCoordinatorID() const
{
    return mCoordinatorID;
}

void ElectableNode::setID( ID fID )
{
    mID = fID;
}
