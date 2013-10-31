#ifndef LASTNODE_H
#define LASTNODE_H

#include<cstdlib>

class LastNode
{
    public:
        double lastWeight;
        double output;
        double delta;
        LastNode();

        virtual ~LastNode();
    protected:
    private:
};

#endif // LASTNODE_H
