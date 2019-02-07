// Sample script to experiment with pthread/getting pid

#include <stdio.h>

int main(){

	int pid;

	pid = getpid();

	printf("PID is: %d\n", pid);

	return 0;
}
