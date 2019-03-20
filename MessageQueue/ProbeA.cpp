//
//  ProbeA.cpp
//  MessageQueue
//
//  Created by  on 2/21/19.
//  Copyright © 2019 StevenOnSoftware. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <limits>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

const int MAGIC_SEED_ALPHA = 997;
const int DATA_HUB_MTYPE = 117;
const int MAX_INT = std::numeric_limits<int>::max();

int main() {
    srand( time(0) );

    bool isExecuting = true;
    bool isAcknowledged = true;
    string sendingMsg = "";

    // generates system wide key for the queue
    int qid = msgget(ftok(".",'u'), 0);
    cout << "qid is " << qid << endl;

    // declare my message buffer
    struct buf {
        long mtype; // required
        char greeting[50]; // mesg content
    };
    buf msg;
    int size = sizeof(msg)-sizeof(long);

    cout << size << endl;
    cout << MAGIC_SEED_ALPHA << endl;

    cout << "/* ----------- Probe A --------------- */" << endl;

    while (isExecuting) {

        // generate a valid random number
        int randomValue = MAX_INT;
        while (randomValue % MAGIC_SEED_ALPHA != 0) {
            randomValue = rand() % MAX_INT;


            if (randomValue <= 100) {
                cout << "Probe A will terminate as random value is less than 100: value is " << randomValue << endl;

                msg.mtype = DATA_HUB_MTYPE;
                sendingMsg = "ProbeA Exit";
                strcpy(msg.greeting, sendingMsg.c_str() );
                msgsnd(qid, (struct msgbuf *)&msg, size, 0); // send message to queue

                isExecuting = false;
                continue;
            }

        }
        cout << "The random value is " << randomValue << endl;

        // send to DataHub
        msg.mtype = DATA_HUB_MTYPE;
        sendingMsg = to_string(getpid()) + " ProbeA: " + to_string(randomValue);
        strcpy(msg.greeting, sendingMsg.c_str() );
        msgsnd(qid, (struct msgbuf *)&msg, size, 0); // send message to queue
        isAcknowledged = false;

        // wait for acknowledgement from DataHub
        if (!isAcknowledged) {
            //cout << "About to receive" << endl;
            msgrcv(qid, (struct msgbuf *)&msg, size, DATA_HUB_MTYPE, 0);
            //cout << "Received" << endl;
            if (msg.mtype == DATA_HUB_MTYPE) {
                isAcknowledged = true;
                cout << "Receive acknowledgement from DataHub\n" << endl;
            } else {
                 cout << "DID NOT Receive acknowledgement from DataHub\n" << endl;
            }
        }


    }

    return 0;
}
