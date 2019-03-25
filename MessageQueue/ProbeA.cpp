//
//  ProbeA.cpp
//  MessageQueue
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
#include <bits/stdc++.h>

using namespace std;

const int MTYPE = 555;
const int MAGIC_SEED_ALPHA = 997;
const int DATA_HUB_MTYPE = 117;
const int MAX_INT = std::numeric_limits<int>::max();

int main() {
    srand( time(0) );

    bool isExecuting = true;
    bool isAcknowledged = false;
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

                msg.mtype = MTYPE;
                sendingMsg = "PROBEA:" + to_string(getpid()) + ":EXIT";
                strcpy(msg.greeting, sendingMsg.c_str() );
                msgsnd(qid, (struct msgbuf *)&msg, size, 0); // send message to queue
                
                isExecuting = false;
                continue;
            }

        }
        cout << "The random value is " << randomValue << endl;

        // send to DataHub
        msg.mtype = MTYPE;
        sendingMsg = "PROBEA:" + to_string(getpid()) + ":" + to_string(randomValue);
        strcpy(msg.greeting, sendingMsg.c_str() );
        msgsnd(qid, (struct msgbuf *)&msg, size, 0); // send message to queue

        // wait for acknowledgement from DataHub
        if (!isAcknowledged) {
            msg.mtype = MTYPE;
            msgrcv(qid, (struct msgbuf *)&msg, size, MTYPE, 0); // read incoming message
            vector <string> tokens; // Vector containing split string
            string currentMsg = msg.greeting; // Load message into a string
            stringstream check1(currentMsg); // Stringstream currentMsg
            string intermediate; // Temp string for iteration

            // Tokenizing by ':'
            while(getline(check1, intermediate, ':')) { 
                tokens.push_back(intermediate); 
            }

            if ((tokens[0].compare("HUB") == 0) && (tokens[2].compare("ACKNOWLEDGED") == 0)) {
                isAcknowledged = true;
                cout << "Receive acknowledgement from DataHub\n" << endl;
            }
        }


    }

    return 0;
}
