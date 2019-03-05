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

using namespace std;

const int MAGIC_SEED_ALPHA = 997;
const int MAX_INT = std::numeric_limits<int>::max();

int main() {
    bool isExecuting = true;
    string sendingMsg = "";
    
    cout << "Testing" << endl;
    
    // generates system wide key for the queue
    int qid = msgget(ftok(".",'u'), 0);
    
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
        int randomValue = 0;
        while (randomValue % MAGIC_SEED_ALPHA != 0) {
            randomValue = rand() % MAX_INT;
            
            if (randomValue <= 100) {
                cout << "Probe A will terminate as random value is less than 100: value is " << randomValue << endl;
                isExecuting = false;
                continue;
            }
            
        }
        cout << "The random value is " << randomValue << endl;
        
        // send to DataHub
        msg.mtype = 117;
        sendingMsg = "997:" + to_string(randomValue);
        strcpy(msg.greeting, sendingMsg.c_str() );
        msgsnd(qid, (struct msgbuf*)&msg, size, 0); // send message to queue
        
        // wait for acknowledgement from DataHub
        
        
        
        // repeat on loop, until generate a random num < 100
    }

    return 0;
}
