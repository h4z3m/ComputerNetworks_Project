#include "node.h"

#define GetCurrentDir _getcwd

Define_Module(Node);

static int base = 0;
static int next_frame_to_send = 0;
//static int frame_expected = 0;
static int control_frame_expected = 0;

static std::vector<Node::ErrorCodeType_t> errorArray;
static std::vector<std::string> messageArray;
static int size_of_Array;
static int frame_to_print = 1;


void Node::openOutputFile() {

    outputFile.open("output.txt", std::ios::out | std::ios::trunc);

    // Return if file was not opened
    if (!outputFile.is_open()) {
        std::cerr << "[NODE] Error opening output file." << std::endl;
        return;
    }
}

void Node::initialize() {

//    // Open output file

//
//    // TODO - Generated method body
//    // TESTING READ MESSAGES
//
//    std::vector<ErrorCodeType_t> errorArray;
//    std::vector<std::string> messageArray;
//    std::string fp = "input0.txt";
//    readMessages(fp, errorArray, messageArray);
//    //TESTING MODIFY MESSAGE
//    std::string t = "abcd";
//    modifyMessage(t);
//    std::cout << t << std::endl;
//    //TESTING PRINT READING
//    printReading(ErrorCodeType_t::ErrorCodeType_LossDupDelay);

}

void Node::handleMessage(cMessage *msg) {
    Message_Base *mmsg = check_and_cast<Message_Base*>(msg);
    // TODO - Generated method body

    // Open output file

//    // TODO - Generated method body
//    // TESTING READ MESSAGES
//
//    std::vector<ErrorCodeType_t> errorArray;
//    std::vector<std::string> messageArray;
//    std::string fp = "input0.txt";
//    readMessages(fp, errorArray, messageArray);
//    //TESTING MODIFY MESSAGE
//    std::string t = "a$bc/d";
//    //modifyMessage(t);
//    Message_Base *mptr =  new Message_Base();
//    framing(mptr,t,5,1);
//    std::cout << mptr->getPayload() << std::endl;
//
//    //TESTING PRINT READING
//    printReading(ErrorCodeType_t::ErrorCodeType_LossDupDelay);

// Get the gate name

//[SENDER NODE] Coordinator message indicating the node will be sender

    if (msg->isSelfMessage() && NodeType_Sender == nodeType) {
        if (base >= size_of_Array) {
            ////////////////////
            //Remove All events
            cFutureEventSet *heap =
                    cSimulation::getActiveSimulation()->getFES();
            heap->clear();

            printToFile();
            finish();
            return;
            /////////////////
        }
        MsgType_t ss = static_cast<MsgType_t>(mmsg->getType());
        switch (ss) {
        case MsgType_t::To_Send: //start sending msg
        {

            //cancelEvent(mmsg);
            if (next_frame_to_send < size_of_Array) {
                scheduleAt(simTime() + par("ProcessingDelay").doubleValue(), mmsg);

            }

            int limit =
                    ((par("WindowSize").intValue() + base) > size_of_Array) ?
                            size_of_Array : par("WindowSize").intValue() + base;
            if (next_frame_to_send >= base && next_frame_to_send < limit) {

                if (next_frame_to_send < size_of_Array-1) {


                    printReading(errorArray[frame_to_print]);

                }



                Message_Base *newmsg = new Message_Base();
                send_logic(newmsg, next_frame_to_send);
                //Message_Base *duplicatedMessage = mmsg->dup();
                Message_Base *duplicatedMessage = new Message_Base(*newmsg);
                duplicatedMessage->setType(MsgType_t::timeout);
                scheduleAfter(par("TimeoutInterval").doubleValue(),
                        duplicatedMessage);
                next_frame_to_send++;
                frame_to_print++;
            }

            break;
        }
        case MsgType_t::timeout: //timeout_happened to be send
        {
            if (mmsg->getHeader() == base) {
                Timeout_print(mmsg->getHeader()% par("WindowSize").intValue());
                next_frame_to_send = base + 1;
                frame_to_print = base+1;
                cancelAndDelete(mmsg);
                ////////////////////
                //Remove All events
                cFutureEventSet *heap =
                cSimulation::getActiveSimulation()->getFES();
                heap->clear();
                /////////////////




                //printReading(errorArray[base]);
                Message_Base *nmsg = new Message_Base();
                framing(nmsg, messageArray[base], base, false);
                nmsg->setType(MsgType_t::Data);
                sendDelayed(nmsg, par("TransmissionDelay").doubleValue()+par("ProcessingDelay").doubleValue(), "out_gate");


                Message_Base *smsg = new Message_Base();
                smsg->setType(MsgType_t::To_Send);
                scheduleAfter(2*par("ProcessingDelay").doubleValue(), smsg);

                Message_Base *tmsg = new Message_Base();
                tmsg->setType(MsgType_t::timeout_print);
                scheduleAfter(par("ProcessingDelay").doubleValue(), tmsg);



                Message_Base *timeout = new Message_Base();
                timeout->setType(MsgType_t::timeout);
                scheduleAfter(par("TimeoutInterval").doubleValue()+par("ProcessingDelay").doubleValue(), timeout);

            }

            break;
        }
        case MsgType_t::timeout_print: //timeout_happened to be send
               {

                   framing(mmsg, messageArray[base], base, false);
                   mmsg->setType(MsgType_t::Data);
                   ErrorCodeType_t ss = ErrorCodeType_NoError;
                   printBeforeTransimission(mmsg, ss);
                   if((base +1) <size_of_Array)
                   {
                   printReading(errorArray[base+1]);
                   }
                  cancelAndDelete(mmsg);


               break;
               }
        default: {
            std::bitset<4> tmp_bits(errorArray[mmsg->getHeader()]);

            if (tmp_bits[Loss] == 0) {
                send_msg(mmsg);
            }
            else
            {
                cancelAndDelete(mmsg);
            }
        }
        }
    }

    else if (msg->isSelfMessage() && NodeType_Receiver == nodeType) {


        if (mmsg->getHeader() == control_frame_expected) {
            control_frame_expected++;
            mmsg->setAck_no(control_frame_expected);
            control_print(mmsg, mmsg->getTrailer());
        }
        cancelAndDelete(mmsg);
        return;
    } else {

        cGate *msgArrivalGate = msg->getArrivalGate();
        std::string gateName = msgArrivalGate->getName();

        if (gateName == "coordinator_gate") {
            // I am now the sender foreevaa
            nodeType = NodeType_Sender;
            base = 0;
            next_frame_to_send = 0;
            frame_to_print = 1;
            std::string fp = mmsg->getPayload();
            readMessages(fp, errorArray, messageArray);
            size_of_Array = messageArray.size();
            // send_logic(mmsg , next_frame_to_send);
            mmsg->setType(MsgType_t::To_Send);
            printReading(errorArray[0]);
            scheduleAt(simTime() + par("ProcessingDelay").doubleValue(), mmsg);
            //TODO Start Go back N protocol here

        }
        //[SENDER NODE] Self message for timeout or ACK

        else if (gateName == "in_gate" && NodeType_Sender == nodeType) {
            //TODO recieve ACK/NACK
            //delay

            if (mmsg->getHeader() == base && mmsg->getType()==MsgType_t::ACK) {
                base++;
            }
            if (base >= size_of_Array) {
                        ////////////////////
                        //Remove All events
                        cFutureEventSet *heap =
                        cSimulation::getActiveSimulation()->getFES();
                        heap->clear();
                        std::cout<<"I'm the sender and i got the last frame\n....Terminating.....\n"<<std::endl;
                        outputBuffer.push_back("I'm the sender and i got the last frame\n....Terminating.....\n");
                        printToFile();
                        finish();
                        return;
                        /////////////////
                    }
            cancelAndDelete(mmsg);
        } else if (gateName == "in_gate" && NodeType_Receiver == nodeType) {
            //[RECEIVER NODE] New message for receiver
            srand(time(0));

            if (errorDetection(mmsg)) {
                mmsg->setType(MsgType_t::NACK);
            } else {
                mmsg->setType(MsgType_t::ACK);
            }





            mmsg->setPayload("");
            int loss_prob = rand() % 100;
            bool lost_tt = (loss_prob < par("ACKLossProbability").doubleValue());

            if (!lost_tt) {
                sendDelayed(mmsg,
                        par("TransmissionDelay").doubleValue()
                                + 2*par("ProcessingDelay").doubleValue(),
                        "out_gate");
            } else {
                cancelAndDelete(mmsg);
            }
            Message_Base *printSelfMessage = new Message_Base(*mmsg);
            printSelfMessage->setTrailer(lost_tt);
            scheduleAfter(par("ProcessingDelay").doubleValue(),
                    printSelfMessage);
        }

    }

}

void Node::readMessages(std::string &fileName,
        std::vector<ErrorCodeType_t> &errorArray,
        std::vector<std::string> &messageArray) {

// Open the file
    std::ifstream node_file;

    node_file.open(get_current_dir() + "\\" + fileName, std::ios::in);

// Return if file was not opened
    if (!node_file.is_open()) {
        std::cerr << "[NODE] Error opening file." << std::endl;
        return;
    } else {
        std::string tmp_errorcodeBinary;

        std::string tmp_line = "";
        std::string tmp_msg = "";
        char dummy;
        while (std::getline(node_file, tmp_line)) {
            std::stringstream s_stream(tmp_line);
            s_stream >> std::noskipws >>tmp_errorcodeBinary>>dummy;

            std::getline(s_stream, tmp_msg, '\n');

            // Push back into the vectors
            std::bitset<4> tmp_bits(tmp_errorcodeBinary);
            errorArray.push_back(
                    static_cast<ErrorCodeType_t>(tmp_bits.to_ulong()));
            messageArray.push_back(tmp_msg);
            tmp_msg.clear();
            s_stream.clear();

        }
        node_file.close();

    }

    return;
}

void Node::modifyMessage(std::string &payload) {
    int bitIdx = rand() % 8;
    int byteIdx = rand() % payload.length();
    payload[byteIdx] ^= (1 << bitIdx);
}

void Node::printReading(ErrorCodeType_t errorCode) {

    std::string node_reading = "At time [" + simTime().str() + "], Node["
            + this->getName()[4] + +"], Introducing channel error with code = "
            + std::bitset<4>(errorCode).to_string() + "\n";
    static int i =0;
    std::cout << node_reading << "  i = "<<i<<std::endl;
    i++;
    outputBuffer.push_back(node_reading);
//outputFile << node_reading << std::endl;

}
char Node::calculateParity(std::string &payload) {
    char parityByte = 0;
    int payloadSize = payload.size();
    for (int i = 0; i < payloadSize; ++i) {

        parityByte = (parityByte ^ payload[i]);
    }

//parityByte ^= (payloadSize + 2)^(0);

    return parityByte;
}

void Node::framing(Message_Base *mptr, std::string &payload, int seq,
        bool modificationFlag) {
    std::string modified = "$";
    int payloadSize = payload.size();

    for (int i = 0; i < payloadSize; i++) {
        char c = payload[i];

        if (c == '$') {
            modified += "/$";
        } else if (c == '/') {
            modified += "//";
        } else {
            modified += payload[i];
        }

    }
    modified += "$";

    char parity = calculateParity(modified);
    if (modificationFlag == 1)
        modifyMessage(modified);

    mptr->setPayload(modified.c_str());
    mptr->setHeader(seq);
    mptr->setTrailer(parity);
    mptr->setType(0);

}

std::string Node::get_current_dir() {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}

////////////gilany////////////////////////////

bool Node::errorDetection(Message_Base *msg) {

    std::vector<std::bitset<8> > vbitset;
//    vbitset.push_back(msg->getHeader());

    std::string PayLoad = msg->getPayload();

    for (int i = 0; i < PayLoad.size(); i++) {
        vbitset.push_back(PayLoad[i]);
    }
    std::bitset<8> trailer_bits = msg->getTrailer();
    std::bitset<8> check(0);

    for (int i = 0; i < vbitset.size(); i++) {
        check = check ^ vbitset[i];
    }

    check = check ^ trailer_bits;

    if (check == 0) {
        return false;
    } else {
        return true;
    }
}

void Node::printBeforeTransimission(Message_Base *msg, ErrorCodeType_t input) {

//TODO get the correct node id
//TODO get the correct duplicate version
    std::bitset<8> trailer_bits = msg->getTrailer();
    std::bitset<4> code(input);

    std::string lost = "No";
    if (code[2] == 1) {
        lost = "Yes";
    }

    float delay = 0;
    if (code[0] == 1) {
        delay = par("ErrorDelay").doubleValue();
    }

    std::string line_to_print = "At time [" + simTime().str() + "] Node["
            + this->getName()[4] + "] sent frame with seq_num=["
            + std::to_string(msg->getHeader()%par("WindowSize").intValue()) + "], and payload=["
            + msg->getPayload() + "], and trailer =[" + trailer_bits.to_string()
            + "] ,Lost [" + lost + "], Duplicate ["
            + std::to_string(msg->getType()) + "], Delay ["
            + std::to_string(delay) + "].\n";

    std::cout << line_to_print << std::endl;
    outputBuffer.push_back(line_to_print);
//outputFile << line_to_print << std::endl;
}

void Node::send_msg(Message_Base *msg) {
    msg->setType(0);

    sendDelayed(msg, par("TransmissionDelay").doubleValue(), "out_gate");
}

////////////gilany////////////////////////////

void Node::control_print(Message_Base *msg, bool lost) {
    std::string ack;
    std::string loss = (lost) ? "Yes" : "No";
    if (msg->getType() == 2) {
        ack = "NACK";
    } else if (msg->getType() == 1) {
        ack = "ACK";
    } else {/*nothing*/
    }

    std::string line_to_print = "At time [" + simTime().str() + "], Node["
            + this->getName()[4] + "] Sending [" + ack + "] with number["
            + std::to_string((msg->getHeader()+1)%par("WindowSize").intValue()) + "], loss [" + loss + "]\n";

    std::cout << line_to_print << std::endl;
    outputBuffer.push_back(line_to_print);
// outputFile << line_to_print << std::endl;

}
void Node::Timeout_print(int seqnum) {

    std::string line_to_print = "Time out event at time [" + simTime().str()
            + "], at Node [" + this->getName()[4] + "] for frame with seq_num=["
            + std::to_string(seqnum) + "]; \n";
    std::cout << line_to_print << std::endl;
    outputBuffer.push_back(line_to_print);
}

void Node::selfMessageDelay(Message_Base *msg, double delay) {
    cancelEvent(msg);
    printBeforeTransimission(msg, errorArray[msg->getHeader()]);
    scheduleAt(simTime() + delay, msg);
}
void Node::selfMessageDuplicate(Message_Base *msg, double delay) {
    double duplicationDelay = par("DuplicationDelay").doubleValue();
//Message_Base *duplicatedMessage = msg->dup();
    Message_Base *duplicatedMessage = new Message_Base(*msg);
    msg->setType(1);
    duplicatedMessage->setType(2);
    selfMessageDelay(msg, delay);
    selfMessageDelay(duplicatedMessage, delay + duplicationDelay);
}
void Node::send_logic(Message_Base *mmsg, int msg_index) {
    std::bitset<4> tmp_bits(errorArray[msg_index]);
    framing(mmsg, messageArray[msg_index], msg_index, tmp_bits[Modification]);
    double delay_time =
            (tmp_bits[Delay] == 1) ? par("ErrorDelay").doubleValue() : 0;
    if (tmp_bits[Dup] == 1) {

        selfMessageDuplicate(mmsg, delay_time);
    } else {
        selfMessageDelay(mmsg, delay_time);
    }

}
void Node::printToFile() {
    openOutputFile();
    for (auto it : outputBuffer) {
        outputFile << it << std::endl;
    }
    outputFile.close();

}

Node::~Node() {

}
