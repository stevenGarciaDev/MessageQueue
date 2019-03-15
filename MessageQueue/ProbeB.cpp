//
//  ProbeB.cpp
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
#include <string>
#include "kill_patch.h"
using namespace std;

const int MAGIC_SEED_BETA = 257;

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
}
