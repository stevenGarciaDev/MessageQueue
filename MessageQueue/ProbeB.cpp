//
// ProbeB.cpp
// MessageQueue
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
#include <stdlib.h> /* srand, rand */
#include <time.h> /* time */
#include "force_patch.h"

using namespace std;

const int MTYPE = 555;
const int PROBE_B_SEED = 257;
const int MAX_INT_LIMIT = std::numeric_limits<int>::max();
const int DATA_HUB_LIMIT = 10000;

int main() {
	srand(time(0));
	
	bool isExecuting = true;
	string hubMessage = "";
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

	cout << "Magic ProbeB seed: " << PROBE_B_SEED << endl;
	cout << "Probe B begins here: " << endl;
	int count = 0;//messages sent to the queue
	while (true) {
		// generate a valid random number
		int random=MAX_INT_LIMIT;
		if(random % PROBE_B_SEED != 0){
			random = rand() % MAX_INT_LIMIT;
			count++;
			cout << "The random value is: " << random << endl;
			
			// send to DataHub
			msg.mtype = MTYPE;
			hubMessage = "PROBEB:" + to_string(getpid()) + ":VALID";
			strcpy(msg.greeting, hubMessage.c_str() );
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); // send message to queue
		}
	}
}