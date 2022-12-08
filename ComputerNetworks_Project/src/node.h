#ifndef __COMPUTERNETWORKS_PROJECT_NODE_H_
#define __COMPUTERNETWORKS_PROJECT_NODE_H_

#include <omnetpp.h>
#include <iostream>
#include <bitset>
#include <string>
#include <sstream>
#include <fstream>
#include <direct.h>
#include <time.h>
#include "Message_m.h"
#include <vector>

#define Delay 0
#define Dup 1
#define Loss 2
#define Modification 3

using namespace omnetpp;

/**
 * TODO - Generated class
 */

class Node: public cSimpleModule {

protected:
    static void openOutputFile();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
public:
    // Output file for logs
    static std::fstream outputFile;
    static std::vector<std::string> outputBuffer;
    /**
     * @enum
     * @brief   Defines error codes that can occur in a message
     *
     */
    typedef enum {
        // [Modification, Loss, Duplication, Delay]
        ErrorCodeType_NoError = 0b0000, /**< No error */
        ErrorCodeType_Delay = 0b0001, /**< Delay only */
        ErrorCodeType_Duplication = 0b0010, /**< Duplication only*/
        ErrorCodeType_Dup_Delay = 0b0011, /**< Duplication & delay */
        ErrorCodeType_Loss = 0b0100, /**< Loss only */
        ErrorCodeType_LossDelay = 0b0101, /**< Loss & delay */
        ErrorCodeType_LossDup = 0b0110, /**< Loss & duplication */
        ErrorCodeType_LossDupDelay = 0b0111,/**< Loss, duplication & delay */
        ErrorCodeType_Modification = 0b1000,/**< Modification only */
        ErrorCodeType_ModDelay = 0b1001, /**< Modification & delay */
        ErrorCodeType_ModDup = 0b1010, /**< Modification & duplication*/
        ErrorCodeType_ModDupDelay = 0b1011, /**< Modification, duplication & delay */
        ErrorCodeType_ModLoss = 0b1100, /**< Modification & loss */
        ErrorCodeType_ModLossDelay = 0b1101,/**< Modification, loss, & delay */
        ErrorCodeType_ModLossDup = 0b1110, /**< Modification, doss & duplication */
        ErrorCodeType_AllErrors = 0b1111, /**< All 4 errors */

    } ErrorCodeType_t;

    typedef enum {
        NodeType_Sender, NodeType_Receiver
    } NodeType_t;
    typedef enum {
        Data = 0, ACK = 1, NACK = 2, To_Send = 3, timeout = 4
    } MsgType_t;

    void readMessages(std::string &fileName,
            std::vector<ErrorCodeType_t> &errorArray,
            std::vector<std::string> &messageArray);
    void modifyMessage(std::string &payload);

    void printReading(ErrorCodeType_t errorCode);
    ~Node();
    char calculateParity(std::string &payload);
    void framing(Message_Base *mptr, std::string &payload, int seq,
            bool modifiedFlag);
private:

    // Gets current working directory of the application
    std::string get_current_dir();
    // Determines whether the node is sender or receiver
    NodeType_t nodeType = NodeType_Receiver;
    bool errorDetection(Message_Base *msg);
    void printBeforeTransimission(Message_Base *msg, ErrorCodeType_t input);
    void send_msg(Message_Base *msg);
    void control_print(Message_Base *msg, bool lost);
    void Timeout_print(int seqnum);
    void selfMessageDelay(Message_Base *msg, double delay);
    void selfMessageDuplicate(Message_Base *msg, double delay);
    void send_logic(Message_Base *mmsg, int msg_index);
    void printToFile();
};
std::fstream Node::outputFile = nullptr;
std::vector<std::string> Node::outputBuffer = { };

#endif
