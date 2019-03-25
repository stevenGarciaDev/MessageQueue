#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <bits/stdc++.h> 
#include "force_patch.h"
using namespace std;

const int MTYPE = 555;

int main() {
    // create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);
    if (qid == -1) {
        qid = msgget(ftok(".",'u'), 0);
    }
    cout << "QID is " << qid << endl;

    // declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);

    // Loop flags
    bool probeA_Executing = true;
    bool probeB_Executing = true;
    bool probeC_Executing = true;

    while (probeC_Executing) {
        msgrcv(qid, (struct msgbuf *)&msg, size, MTYPE, 0); // read incoming message
        vector <string> tokens; // Vector containing split string
        string currentMsg = msg.greeting; // Load message into a string
        stringstream check1(currentMsg); // Stringstream currentMsg
        string intermediate; // Temp string for iteration

        // Tokenizing by ':'
        while(getline(check1, intermediate, ':')) { 
            tokens.push_back(intermediate); 
        } 

        // tokens[0] : PROBE NAME
        // tokens[1] : PID
        // tokens[2] : MESSAGE
        
        // receive message from Probe A
        if (probeA_Executing && (tokens[0].compare("PROBEA") == 0)) {

        }

        // receive message from Probe B
        if (probeB_Executing && (tokens[0].compare("PROBEB") == 0)) {
            
        }

        // receive message from Probe C
        if (probeC_Executing && (tokens[0].compare("PROBEC") == 0)) {
            // check if message sent by Probe C was valid: message ("ProbeC exit.")
            cout << currentMsg << endl;
            if (tokens[2].compare("VALID") == 0) {
                currentMsg = msg.greeting;
                probeC_Executing = false;
            } else {
                currentMsg = msg.greeting;
                probeC_Executing = false;
            }
        }
    }

    exit(0);
}