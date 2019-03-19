//
//  DataHub.cpp
//  MessageQueue
//
//  Created by Steven Garcia on 2/21/19.
//  Copyright © 2019 StevenOnSoftware. All rights reserved.
//

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
using namespace std;

const int PROBE_A_MTYPE = 997;
const int PROBE_B_MTYPE = 257;
const int PROBE_C_MTYPE = 251;
const int DATA_HUB_MTYPE = 117;

int main() {
    // create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);
    if (qid == -1) {
        qid = msgget(ftok(".",'u'), 0);
    }
    cout << "qid is " << qid << endl;
    
    string responseMsg = "";
    string currentMsg = "";
    bool receivingMessages = true;
    bool probeA_Executing = true;
    bool probeB_Executing = true;
    bool probeC_Executing = true;
    
    // declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);

//    // Read message (Probe C)
//    msgrcv(qid, (struct msgbuf *)&msg, size, 251, 0); // read message from probe c (251 aka Rho)
//    cout << getpid() << " (Data Hub): Found reading" << endl;
//    cout << msg.greeting << endl;
    
    while (receivingMessages) {
        
        // receive message from Probe A
        if (probeA_Executing) {
            cout << "about to receive message" << endl;
            msgrcv(qid, (struct msgbuf *)&msg, size, PROBE_A_MTYPE, 0); // read incoming message
            cout << "received message" << endl;
            
            if (msg.mtype == PROBE_A_MTYPE) {
                cout << getpid() << " (Probe A) Found reading" << endl;
                cout << "Message: " << msg.greeting << "\n" << endl;
                
                // check if message sent by Probe A was to exit program
                if (currentMsg.compare("ProbeA Exit") == 0) {
                    
                    cout << "exiting" << endl;
                    probeA_Executing = false;
                    
                } else {
                    // otherwise send message in response to Probe A as to acknowledge successfully sent
                    msg.mtype = DATA_HUB_MTYPE;
                    responseMsg = "Acknowledge";
                    strcpy(msg.greeting, responseMsg.c_str() );
                    msgsnd(qid, (struct msgbuf *)&msg, size, 0); // send message to queue back to Probe A
                    cout << responseMsg << endl;
                }
            } else {
                cout << "Did not receive message from Probe A.\n" << endl;
            }
        }
        
        // receive message from Probe B
        if (probeB_Executing) {
            
           msgrcv(qid, (struct msgbuf *)&msg, size, PROBE_B_MTYPE, 0); // read incoming message
        }
        
        // receive message from Probe C
        if (probeC_Executing) {
            msgrcv(qid, (struct msgbuf *)&msg, size, PROBE_C_MTYPE, 0); // read incoming message
            cout << getpid() << " (Probe C) Found reading" << endl;
            cout << msg.greeting << "\n" << endl;
            
            currentMsg = msg.greeting;
            
            // check if message sent by Probe C was to exit program
            if (currentMsg.compare("ProbeC exit.") == 0) {

                probeC_Executing = false;
                
            } else {
                // otherwise send message in response to Probe C as to acknowledge successfully sent
                msg.mtype = DATA_HUB_MTYPE;
                responseMsg = "DataHub Received Message";
                strcpy(msg.greeting, responseMsg.c_str() );
            }

//            msgrcv(qid, (struct msgbuf *)&msg, size, PROBE_C_MTYPE, 0); // read message from probe c (251 aka Rho)
//            cout << getpid() << " (Probe C): Found reading" << endl;
//            cout << msg.greeting << "\n" << endl;
        }
        
    }

	// Delete msgqueue
	msgctl (qid, IPC_RMID, NULL);

    exit(0);
}
