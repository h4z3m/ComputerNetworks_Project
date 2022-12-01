#ifndef __COMPUTERNETWORKS_PROJECT_NODE_H_
#define __COMPUTERNETWORKS_PROJECT_NODE_H_

#include <omnetpp.h>
#include "Message_m.h"
#include <string>
#include <vector>
#include <fstream>
using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Node: public cSimpleModule {
private:

    std::fstream outputFile;
    std::string get_current_dir();

protected:
    void openOutputFile();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
public:
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

    void readMessages(std::string &fileName,
            std::vector<ErrorCodeType_t> &errorArray,
            std::vector<std::string> &messageArray);
    void modifyMessage(std::string &payload);
    void printReading(ErrorCodeType_t errorCode);
    ~Node();
    char calculateParity(std::string &payload);
    void framing(Message_Base *mptr,std::string &payload, int seq, bool modifiedFlag);
};

#endif
