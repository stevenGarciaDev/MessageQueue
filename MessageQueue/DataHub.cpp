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

int main() {
    // create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);
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

	// Read message (Probe C)
	msgrcv(qid, (struct msgbuf *)&msg, size, 251, 0); // read message from probe c (251 aka Rho)
    cout << getpid() << " (Data Hub): Found reading" << endl;
    cout << msg.greeting << endl;
    
    
    while (receivingMessages) {
        // receive message from Probe A
        if (probeA_Executing) {
            msgrcv(qid, (struct msgbuf *)&msg, size, PROBE_A_MTYPE, 0); // read incoming message
            
            // then send message to Probe A
        }
        
        // receive message from Probe B
        if (probeB_Executing) {
            msgrcv(qid, (struct msgbuf *)&msg, size, PROBE_B_MTYPE, 0); // read incoming message
        }
        
        // receive message from Probe C
        if (probeC_Executing) {
            msgrcv(qid, (struct msgbuf *)&msg, size, PROBE_C_MTYPE, 0); // read message from probe c (251 aka Rho)
            cout << getpid() << " (Data Hub): Found reading" << endl;
            cout << msg.greeting << endl;
        }
        
    }

	// Delete msgqueue
	msgctl (qid, IPC_RMID, NULL);

    exit(0);
}
