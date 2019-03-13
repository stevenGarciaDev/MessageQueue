//
//  ProbeC.cpp
//  MessageQueue
//

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <string>
using namespace std;

const int MAGIC_SEED_RHO = 251;

int main() {
    srand(time(0));
    // generates system wide key for the queue
    int qid = msgget(ftok(".",'u'), 0);

    // Message buffer struct
    struct buf {
        long mtype;
        char greeting[50];
    };

    buf msg;
	int size = sizeof(msg)-sizeof(long);
    

    cout << "/* --------------- Probe C --------------- */" << endl;
    bool isValid = false; // Flag to stop validating 
    do {
        // Valid Reading
        int randValue = rand();
        int reading = randValue % MAGIC_SEED_RHO;
        if (reading == 0) { // random integer is divisible by seed.
            isValid = true; // Toggle flag

            string sendingMsg = "";
            char tempmsg[50];
            strcpy(tempmsg, "Valid Reading Found");
            cout << tempmsg << ": "<< randValue << endl;

            // send to DataHub
            msg.mtype = 117;
            sendingMsg = "251:" + to_string(randValue);
            strcpy(msg.greeting, sendingMsg.c_str() );
            msgsnd(qid, (struct msgbuf*)&msg, size, 0); // send message to queue
        }
    } while (!isValid);

    exit(0);
}