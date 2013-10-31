#include "LastNode.h"
#include <ctime>
LastNode::LastNode()
{
    srand((unsigned)time(0));
    lastWeight = (double)rand()/RAND_MAX;
    output = 0.0;
}

LastNode::~LastNode()
{
    //dtor
}
