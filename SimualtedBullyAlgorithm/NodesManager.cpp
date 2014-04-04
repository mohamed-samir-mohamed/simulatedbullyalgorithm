#include "NodesManager.h"
#include "ElectableNode.h"
#include "MessageRouter.h"

NodesManager::NodesManager()
{
    mNodes.clear();
}

NodesManager::~NodesManager()
{
    for (auto it = mNodes.begin(); it != mNodes.end(); it++)
    {
        MessageRouter::removeHandler(it->first);
        delete it->second;
        it->second = nullptr;
    }
    mNodes.clear();
}


void NodesManager::addNode( ElectableNode* fNode )
{
    mNodes[fNode->getID()] = fNode;
    MessageRouter::addHandler(fNode);
}

void NodesManager::deleteNode( ID fID )
{
    if (mNodes.find(fID) != mNodes.end())
    {
        MessageRouter::removeHandler(fID);
        delete mNodes[fID];
        mNodes[fID] = nullptr;
        mNodes.erase(fID);  
    }
}

ElectableNode* NodesManager::getByID( ID fID )
{
    if (mNodes.find(fID) != mNodes.end())
    {
        return mNodes[fID];
    }
    return nullptr;
}

void NodesManager::deleteCoordinator()
{
    if(mNodes.begin()->second != nullptr)
        deleteNode(mNodes.begin()->second->getCoordinatorID());
}

void NodesManager::update()
{
    for (auto it = mNodes.begin(); it != mNodes.end(); it++)
    {
        it->second->update();
    }
}
