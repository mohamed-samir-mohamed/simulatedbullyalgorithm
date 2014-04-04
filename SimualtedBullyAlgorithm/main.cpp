#include "ElectableNode.h"
#include "MessageRouter.h"
#include <vector>
#include <ctime>
#include "NodesManager.h"
using namespace std;
int main()
{
    NodesManager manager;
    for (int i = 0; i < 10 ; i++)
    { 
        manager.addNode(new ElectableNode());
    }

    double t = time(0);
    int count = 9;
    while(true)
    {
        manager.update();
        double now = time(0);
        if (now - t > 10 )
        {
            manager.deleteNode(count);
            count--;
            t = time(0);
        }
    }

    return 0;
}