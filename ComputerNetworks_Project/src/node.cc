#include "node.h"

#define GetCurrentDir _getcwd

Define_Module(Node);

void Node::openOutputFile() {

    outputFile.open("output.txt", std::ios::out);

    // Return if file was not opened
    if (!outputFile.is_open()) {
        std::cerr << "[NODE] Error opening output file." << std::endl;
        return;
    }
}

void Node::initialize() {

//    // Open output file
   // openOutputFile();
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
    // TODO - Generated method body

    // Open output file
    openOutputFile();

    // TODO - Generated method body
    // TESTING READ MESSAGES

    std::vector<ErrorCodeType_t> errorArray;
    std::vector<std::string> messageArray;
    std::string fp = "input0.txt";
    readMessages(fp, errorArray, messageArray);
    //TESTING MODIFY MESSAGE
    std::string t = "abcd";
    //modifyMessage(t);
    Message_Base *mptr =  new Message_Base();
    framing(mptr,t,5,1);

    std::cout << mptr->getPayload() << std::endl;

    std::cout << errorDetection(mptr) << std::endl;
    std::cout << par("ErrorDelay").doubleValue()<<std::endl;
    std::cout << par("TransmissionDelay").doubleValue()<<std::endl;
    printBeforeTransimission(mptr, ErrorCodeType_t::ErrorCodeType_LossDelay  , 0.5 );
    send_msg(mptr , 1.0);
    //selfMessageDuplicate(mptr,1.0);
    //TESTING PRINT READING
    printReading(ErrorCodeType_t::ErrorCodeType_LossDupDelay);

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

        while (std::getline(node_file, tmp_line)) {
            std::stringstream s_stream(tmp_line);
            s_stream >> tmp_errorcodeBinary;
            std::getline(s_stream, tmp_msg, '\n');

            // Push back into the vectors
            std::bitset<4> tmp_bits(tmp_errorcodeBinary);
            errorArray.push_back(
                    static_cast<ErrorCodeType_t>(tmp_bits.to_ulong()));
            messageArray.push_back(tmp_msg);
            tmp_msg.clear();
            s_stream.clear();
            node_file.close();
        }

    }

    return;
}

void Node::modifyMessage(std::string &payload) {
    int bitIdx = rand() % 8;
    int byteIdx = rand() % payload.length();
    payload[byteIdx] ^= (1 << bitIdx);
}

void Node::printReading(ErrorCodeType_t errorCode) {

    std::string node_reading = "At time [" + simTime().str() + "], "
            + this->getName() + +", Introducing channel error with code = "
            + std::bitset<4>(errorCode).to_string() + "\n";

    std::cout << node_reading;


    std::cout << node_reading << std::endl;

    outputFile << node_reading << std::endl;


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

void Node::framing(Message_Base *mptr, std::string &payload, int seq, bool modificationFlag){
    std::string modified = "$";
    int payloadSize =payload.size();

    for (int i = 0; i < payloadSize; i++){
        char c = payload[i];

        if(c== '$'){
            modified += "/$";
        } else if(c== '/'){
            modified += "//";
        } else{
            modified += payload[i];
        }

    }
    modified +="$";

    char parity = calculateParity(modified);
    if(modificationFlag == 1)
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

bool  Node::errorDetection(Message_Base *msg){

    std::vector<std::bitset<8> > vbitset;
//    vbitset.push_back(msg->getHeader());

    std::string PayLoad = msg->getPayload();


    for (int i = 0;i<PayLoad.size();i++)
    {
    vbitset.push_back(PayLoad[i]);
    }
    std::bitset<8> trailer_bits = msg->getTrailer();
    std::bitset<8> check(0);

        for (int i = 0;i<vbitset.size();i++)
        {
            check = check^vbitset[i];
        }



        check = check^trailer_bits;


        if(check==0)
        {
            return true;
        }
        else{
            return false;
        }
}

void Node::printBeforeTransimission(Message_Base *msg, ErrorCodeType_t input , double ErrorDelay){

    //TODO get the correct node id
    //TODO get the correct duplicate version
    std::bitset<8> trailer_bits = msg->getTrailer();
    std::bitset<4> code(input);


    std::string lost = "No";
    if(code[2] == 1){
        lost = "Yes";
    }

    float delay = 0;
    if(code[0] == 1){
        delay = ErrorDelay;
    }

    std::string line_to_print = "At time [" + simTime().str() + "] Node[" + this->getName()[4] +"] sent frame with seq_num=["+std::to_string(msg->getHeader())+"], and payload=["+msg->getPayload()+"], and trailer =["+trailer_bits.to_string()+"] ,Lost ["+lost+"], Duplicate ["+std::to_string(msg->getType())+"], Delay ["+std::to_string(delay)+"].\n";
    std::cout<<line_to_print<<std::endl;
    outputFile << line_to_print << std::endl;
}

void Node::send_msg(Message_Base *msg ,double TransmissionDelay)
{
    msg->setType(0);

    sendDelayed(msg, TransmissionDelay , "out_gate");
}

////////////gilany////////////////////////////


void Node::control_print(Message_Base *msg , bool lost)
{
    double time_after_processing= simTime().dbl()+par("ProcessingDelay").doubleValue();
    std::string ack;
    std::string loss = (lost)?"Yes":"No";
            if(msg->getType() == 2){ack = "NACK";}
            else if (msg->getType() == 1){ack = "ACK";}
            else{/*nothing*/}

    std::string line_to_print = "At time ["+ std::to_string(time_after_processing)+", Node [" + this->getName()[4]+"] Sending ["+std::to_string(msg->getHeader())+"] with number["+ + "], loss ["+loss+"]\n" ;
       std::cout<<line_to_print<<std::endl;
       outputFile << line_to_print << std::endl;


}
void Node::Timeout_print(int seqnum)
{

    std::string line_to_print = "Time out event at time ["+ simTime().str()+", at Node [" + this->getName()[4]+"] for frame with seq_num=["+std::to_string(seqnum)+"]; \n" ;
       std::cout<<line_to_print<<std::endl;
       outputFile << line_to_print << std::endl;
}

void Node::selfMessageDelay(Message_Base *msg , double delay){
    scheduleAt(simTime() + exponential(delay), msg);
}
void Node::selfMessageDuplicate(Message_Base *msg,double delay){
    double duplicationDelay = par("DuplicationDelay").doubleValue();
    Message_Base *duplicatedMessage = msg->dup();
    msg->setType(1);
    duplicatedMessage->setType(2);
    selfMessageDelay(msg,delay);
    selfMessageDelay(duplicatedMessage,delay+duplicationDelay);
}



Node::~Node() {
    outputFile.close();
}
