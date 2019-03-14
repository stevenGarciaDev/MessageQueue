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
#include "kill_patch.h"
using namespace std;

const int MAGIC_SEED_RHO = 251;

int main() {
    srand(time(0)); // Seed for better random values

    // Message buffer struct
    struct buf {
        long mtype;
        char greeting[50];
    };

    // generates system wide key for the queue
    int qid = msgget(ftok(".",'u'), 0);

    buf msg;
	int size = sizeof(msg)-sizeof(long);

    // Call kill patch function
    msg.mtype = 521; // Set to mtype for exit
    string exitmsg = "Probe C is closing.";
    strcpy(msg.greeting, exitmsg.c_str() );
    kill_patch(qid, (struct msgbuf*)&msg, size, msg.mtype);
    

    cout << "Probe C: Finding valid reading" << endl;
    bool isValid = false; // Flag to stop validating 
    do {
        // Valid Reading
        int randValue = rand();
        int reading = randValue % MAGIC_SEED_RHO;
        if (reading == 0) { // random integer is divisible by seed.
            isValid = true; // Toggle flag
            string sendingMsg = ""; // Message sent to queue

            // send to DataHub
            msg.mtype = 251; // mtype set to seed, why not?
            sendingMsg = to_string(getpid()) + " (Probe C): " + to_string(randValue); // set message to send as PID of probe + valid reading
            strcpy(msg.greeting, sendingMsg.c_str() ); // Copy message string to char array
            msgsnd(qid, (struct msgbuf*)&msg, size, 0); // Send message to queue
        }
    } while (!isValid);

    // Delete msgqueue when testing probe C only
	msgctl (qid, IPC_RMID, NULL);

    exit(0);
}