#ifndef __COMPUTERNETWORKS_PROJECT_COORDINATOR_H_
#define __COMPUTERNETWORKS_PROJECT_COORDINATOR_H_

#include <omnetpp.h>
#include <string>
#include <vector>
#include "Message_m.h"
using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Coordinator: public cSimpleModule {
private:
    std::string get_current_dir();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

private:
    void read_coordinator(std::string &fileName, int &nodeID,
            float &startTime_sec);
    void modify_payload(std::string &payload);
};

#endif
