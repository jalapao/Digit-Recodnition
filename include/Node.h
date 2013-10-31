
#ifndef NODE_H
#define NODE_H

#include <cstdlib>
using namespace std;

class Node
{
    public:
        double weight[10];
        double output;
        double delta[10];

        Node();
        virtual ~Node();
    protected:
    private:
};

#endif // NODE_H
