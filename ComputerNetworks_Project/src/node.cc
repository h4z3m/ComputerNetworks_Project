//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "node.h"
#include <iostream>
#include <bitset>
#include <string>
#include <sstream>
#include <fstream>

Define_Module(Node);

void Node::initialize() {

    // TODO - Generated method body
    // TESTING READ MESSAGES
    std::vector<Message_Base::ErrorCodeType_t> errorArray;
    std::vector<std::string> messageArray;
    std::string fp =
            "C:\\Users\\h4z3m\\Desktop\\Files\\College\\3rd\\1st\\CMPN405\\Project\\ComputerNetworks_Project\\ComputerNetworks_Project\\ComputerNetworks_Project\\ComputerNetworks_Project\\src\\input0.txt";
    readMessages(fp, errorArray, messageArray);
    std::string t = "abcd";
    modifyMessage(t);
    std::cout << t << std::endl;

}
//add comment2
//try e-git
void Node::handleMessage(cMessage *msg) {
    // TODO - Generated method body

}

void Node::readMessages(std::string &filepath,
        std::vector<Message_Base::ErrorCodeType_t> &errorArray,
        std::vector<std::string> &messageArray) {

    // Open the file
    std::ifstream node_file;
    node_file.open(filepath, std::ios::in);
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
                    static_cast<Message_Base::ErrorCodeType_t>(tmp_bits.to_ulong()));
            messageArray.push_back(tmp_msg);
            tmp_msg.clear();
            s_stream.clear();
        }

    }

    return;
}

void Node::modifyMessage(std::string &payload) {
    int bitIdx = rand() % 8;
    int byteIdx = rand() % payload.length();
    payload[byteIdx] ^= (1 << bitIdx);
}
