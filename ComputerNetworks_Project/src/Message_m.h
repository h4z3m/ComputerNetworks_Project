//
// Generated file, do not edit! Created by opp_msgtool 6.0 from Message.msg.
//

#ifndef __MESSAGE_M_H
#define __MESSAGE_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// opp_msgtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgtool: 'make clean' should help.
#endif

class Message;
/**
 * Class generated from <tt>Message.msg:20</tt> by opp_msgtool.
 * <pre>
 * packet Message
 * {
 *     \@customize(true);  // see the generated C++ header for more info
 *     int Header;
 *     string Payload;
 *     char Trailer;
 *     int Type;
 *     int ack_no;
 * }
 * </pre>
 *
 * Message_Base is only useful if it gets subclassed, and Message is derived from it.
 * The minimum code to be written for Message is the following:
 *
 * <pre>
 * class Message : public Message_Base
 * {
 *   private:
 *     void copy(const Message& other) { ... }

 *   public:
 *     Message(const char *name=nullptr, short kind=0) : Message_Base(name,kind) {}
 *     Message(const Message& other) : Message_Base(other) {copy(other);}
 *     Message& operator=(const Message& other) {if (this==&other) return *this; Message_Base::operator=(other); copy(other); return *this;}
 *     virtual Message *dup() const override {return new Message(*this);}
 *     // ADD CODE HERE to redefine and implement pure virtual functions from Message_Base
 * };
 * </pre>
 *
 * The following should go into a .cc (.cpp) file:
 *
 * <pre>
 * Register_Class(Message)
 * </pre>
 */
class Message_Base: public ::omnetpp::cPacket {
protected:
    int Header = 0;
    omnetpp::opp_string Payload;
    char Trailer = 0;
    int Type = 0;
    int ack_no = 0;

private:
    void copy(const Message_Base &other);

protected:
    bool operator==(const Message_Base&) = delete;
    // make constructors protected to avoid instantiation
    Message_Base(const char *name = nullptr, short kind = 0);
    Message_Base(const Message_Base &other);
    // make assignment operator protected to force the user override it
    Message_Base& operator=(const Message_Base &other);

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

    virtual ~Message_Base();
    virtual Message_Base* dup() const override {
        throw omnetpp::cRuntimeError(
                "You forgot to manually add a dup() function to class Message");
    }
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual int getHeader() const;
    virtual void setHeader(int Header);

    virtual const char* getPayload() const;
    virtual void setPayload(const char *Payload);

    virtual char getTrailer() const;
    virtual void setTrailer(char Trailer);

    virtual int getType() const;
    virtual void setType(int Type);

    virtual int getAck_no() const;
    virtual void setAck_no(int ack_no);
};

namespace omnetpp {

template<> inline Message_Base* fromAnyPtr(any_ptr ptr) {
    return check_and_cast<Message_Base*>(ptr.get<cObject>());
}

}  // namespace omnetpp

#endif // ifndef __MESSAGE_M_H

