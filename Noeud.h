
#ifndef NOEUD_H_
#define NOEUD_H_
#include <omnetpp.h>


using namespace omnetpp;

class Client : public cSimpleModule

{
protected:
    virtual void initialize();
    virtual void handleMessage (cMessage *msg);
    virtual void finish();
};




class Server : public cSimpleModule

{
protected:
    virtual void initialize();
    virtual void handleMessage (cMessage *msg);
};


#endif /* NOEUD_H_ */
