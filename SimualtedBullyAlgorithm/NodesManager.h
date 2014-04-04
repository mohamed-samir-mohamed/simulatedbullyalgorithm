#ifndef NODESMANAGER_H
#define NODESMANAGER_H
#include "GeneralDefinations.h"
#include <map>

using namespace std;
class ElectableNode;

class NodesManager
{
public:
    NodesManager();
    ~NodesManager();
    void addNode(ElectableNode* fNode);
    void deleteNode(ID fID);
    void deleteNode(ElectableNode* fID);
    ElectableNode* getByID(ID fID);
    void deleteCoordinator();
    void update();
private:
   map<ID, ElectableNode*> mNodes;
};

#endif// !NODESMANAGER_H