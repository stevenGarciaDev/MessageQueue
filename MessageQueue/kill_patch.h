#include<iostream>
#include<signal.h>
#include<unistd.h>
#include <sys/types.h>

// the function sends one message to the queue, when the user enters the kill command (SIGUSR1) & terminates the process
// qid - id of attached queue (retval from msgget)
// exitmsg - pointer to message object contains the exit message to its recipient
// size - size of the message object
// mtype - mtype associated with this message object
void kill_patch(int qid, msgbuf *exitmsg, int size, long mtype);
