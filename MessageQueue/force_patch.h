#include <sys/types.h>
#include <signal.h>

// the function signals the process to terminate (SIGKILL)
// pid of the process to terminate
int force_patch(pid_t pid);
