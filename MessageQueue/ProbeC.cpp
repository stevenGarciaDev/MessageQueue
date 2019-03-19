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
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <string>
#include "kill_patch.h"

using namespace std;

const int MAGIC_SEED_RHO = 251;
void kill_patch(int, struct msgbuf *, int, long);

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

    cout << "Probe C: Finding valid reading" << endl;
    bool isValid = false; // Flag to stop validating 
    while (!isValid) {
        // Valid Reading
        int randValue = rand();
        int reading = randValue % MAGIC_SEED_RHO;
        if (reading == 0) { // random integer is divisible by seed.
            isValid = true; // void kill_patch(int qid, msgbuf *exitmsg, int size, long mtype);Toggle flag
            string sendingMsg = ""; // Message sent to queue

            cout << "(Probe C): Found a valid reading: " << to_string(randValue) << endl;

            // send to DataHub
            msg.mtype = MAGIC_SEED_RHO; // mtype set to seed, why not?
            sendingMsg = "ProbeC Exit";
            strcpy(msg.greeting, sendingMsg.c_str() );
            msgsnd(qid, (struct msgbuf*)&msg, size, 0); // Send message to queue
        } else {
            cout << "(Probe C): Found garbage: " << to_string(randValue) << endl;
        }
    }

    // Set parameters for kill patch
    msg.mtype = MAGIC_SEED_RHO; // Set to mtype for exit
    string exitmsg = "ProbeC exit.";
    strcpy(msg.greeting, exitmsg.c_str() );

    // Call kill_patch.h function.
    kill_patch(qid, (struct msgbuf *)&msg, size, MAGIC_SEED_RHO);
    
    // Delete msgqueue when testing probe C only
	//msgctl (qid, IPC_RMID, NULL);

    exit(0);
}
