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
       MessageRouter::addHandler(node);
   }

   double t = time(0);
   int handler = 9;
   while(true)
   {
       int size = nodeVector.size(); 
       for (int i = 0; i < size ; i++)
       {
           nodeVector[i]->update();
       }
       double now = time(0);
       if (now - t > 10 )
       {
           if (nodeVector.size() > 1)
           {
               nodeVector.pop_back();
               MessageRouter::removeHandler(handler);
               handler --;
               t = time(0);
           }
       }
   }

    return 0;
}