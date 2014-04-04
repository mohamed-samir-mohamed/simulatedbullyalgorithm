#include "ElectableNode.h"
#include "MessageRouter.h"
#include <vector>
#include <ctime>
using namespace std;
int main()
{
    vector<ElectableNode*> nodeVector;
    ElectableNode* node;
   for (int i = 0; i < 10 ; i++)
   {
       node = new ElectableNode();
       nodeVector.push_back(node);
       MessageRouter::addHandler(node->getID(), node);
   }

   double t = time(0);
   while(true)
   {
       for (int i = 0; i < 10 ; i++)
       {
           nodeVector[i]->update();
       }
       double now = time(0);
       if (now - t > 10 )
       {
           nodeVector.pop_back();
           MessageRouter::removeHandler(9);
           now = 0; t = 0;
       }
   }

    return 0;
}