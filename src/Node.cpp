#include <Node.h>
#include <ctime>

Node::Node()
{
    srand((unsigned)time(0));
    for(int i=0;i<10;i++)
    {
        weight[i] = (double)rand()/RAND_MAX;
    }
    output = 0.0;
}

Node::~Node()
{
    //dtor
}
