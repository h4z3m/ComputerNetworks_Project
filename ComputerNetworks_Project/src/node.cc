#include "node.h"
#include <iostream>
#include <bitset>
#include <string>
#include <sstream>
#include <fstream>
#include <direct.h>
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
//    openOutputFile();
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
    std::string t = "a$bc/d";
    //modifyMessage(t);
    Message_Base *mptr =  new Message_Base();
    framing(mptr,t,5,1);
    std::cout << mptr->getPayload() << std::endl;
    //TESTING PRINT READING
<<<<<<< HEAD
    printReading(ErrorCodeType_t::ErrorCodeType_LossDupDelay);
=======
    printReading(Message_Base::ErrorCodeType_t::ErrorCodeType_LossDupDelay);

}

void Node::handleMessage(cMessage *msg) {
    // TODO - Generated method body

>>>>>>> 878f47c Finished Framing
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

    std::cout << node_reading << std::endl;

    outputFile << node_reading << std::endl;

}

char Node::calculateParity(std::string &payload) {
        char parityByte = 0;
        int payloadSize = payload.size();
        for (int i = 0; i < payloadSize; ++i) {

            parityByte = (parityByte ^ payload[i]);
        }

        parityByte ^= (payloadSize + 2)^(0);

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
Node::~Node() {
    outputFile.close();
}
