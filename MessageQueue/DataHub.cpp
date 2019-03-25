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

int force_patch(pid_t pid);

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
    int msgCount = 0;

    // Loop flags
    bool probeA_Executing = true;
    bool probeB_Executing = true;
    bool probeC_Executing = true;

    while (probeA_Executing || probeB_Executing || probeC_Executing) {
        msgrcv(qid, (struct msgbuf *)&msg, size, MTYPE, 0); // read incoming message
        msgCount++; // Increase message count

        vector <string> tokens; // Vector containing split string
        string currentMsg = msg.greeting; // Load message into a string
        stringstream check1(currentMsg); // Stringstream currentMsg
        string intermediate; // Temp string for iteration

        // Tokenizing by ':'
        while(getline(check1, intermediate, ':')) { 
            tokens.push_back(intermediate); 
        } 

        // tokens[0] : PROBE NAME
        // tokens[1] : PID (should be set to int with stoi)
        // tokens[2] : MESSAGE
        
        // receive message from Probe A
        if (probeA_Executing && (tokens[0].compare("PROBEA") == 0)) {
            if (tokens[2].compare("EXIT")) {
                cout << msg.greeting << endl;
                probeA_Executing = false;
            } else {
                msg.mtype = MTYPE;
                string sendingMsg = "HUB:" + to_string(getpid()) + ":ACKNOWLEDGED";
                strcpy(msg.greeting, sendingMsg.c_str() );
                cout << "Sending Message: " << sendingMsg << endl;
                msgsnd(qid, (struct msgbuf*)&msg, size, 0); // Send acknowledgement to queue
            }
        }

        // receive message from Probe B
        if (probeB_Executing && (tokens[0].compare("PROBEB") == 0)) {
            if (msgCount >= 100) {
                cout << msg.greeting << endl;
                probeB_Executing = false;
                pid_t pid = stoi(tokens[1]);
                force_patch(pid);
            } else {
                cout << msg.greeting << endl;
            }
        }

        // receive message from Probe C
        if (probeC_Executing && (tokens[0].compare("PROBEC") == 0)) {
            // check if message sent by Probe C was valid: message ("ProbeC exit.")
            if (tokens[2].compare("VALID") == 0) {
                cout << msg.greeting << endl;
            } else if(tokens[2].compare("EXIT") == 0) {
                cout << msg.greeting << endl;
                probeC_Executing = false; // Toggle flag.
            }
        }
    }


    cout << "[!] All probes have terminated [!]" << endl;
    cout << "Deleting message queue" << endl;
    // Remove message queue
    msgctl (qid, IPC_RMID, NULL);

    exit(0);
}