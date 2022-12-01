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

#include "coordinator.h"
#include <iostream>
#include <fstream>
#include <direct.h>
#define GetCurrentDir _getcwd

using namespace std;
Define_Module(Coordinator);
enum M_type {
    Data, Ack, Nack
};

/////////////////////////////
//Test_code for message

/*  Message_Base* trial = new Message_Base();
 trial->setHeader(5);
 trial->setAck_no(0);
 trial->setPayload("This is a test Message");
 trial->setTrailer('A');
 trial->setType(Data);
 scheduleAt(simTime() + 100, trial);
 */

/* Message_Base *mmsg = check_and_cast<Message_Base *>(msg);
 if(mmsg->isSelfMessage()){
 cout<<mmsg->getPayload()<<mmsg->getType()<<endl;}
 cancelAndDelete(msg);*/
////////////////////////////
void Coordinator::initialize() {

    // TODO - Generated method body

    //TESTING READ COORDINATOR

    std::string fp = "coordinator.txt";
    int nodeID = 532;
    float start_t = 0;
    read_coordinator(fp, nodeID, start_t);
}

void Coordinator::handleMessage(cMessage *msg) {

    // TODO - Generated method body
}

void Coordinator::read_coordinator(std::string &fileName, int &nodeID,
        float &startTime_sec) {
    // Open the file
    std::ifstream coordinator_file;
    coordinator_file.open(get_current_dir() + "\\" + fileName, std::ios::in);
    // Return if file was not opened
    if (!coordinator_file.is_open()) {
        std::cerr << "[COORDINATOR] Error opening file." << std::endl;
        return;
    } else {
        // Read the 2 numbers from file
        // NodeID is the first number, then start time in seconds
        coordinator_file >> nodeID >> startTime_sec;

    }
}

std::string Coordinator::get_current_dir() {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}
