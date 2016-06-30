#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <cstdio>
#include <iostream>
#include <fcntl.h>
#include <sys/sem.h>

int main()
{
	key_t key;
	int semid;
	char pathname[] = "/tmp/sem.temp";

	if((key = ftok(pathname, 1)) <0)
	{
		perror("Error generate key \n");
		return -1;
	}

	if((semid = semget(key, 16, 0666 | IPC_CREAT)) < 0)
	{
		perror("Error get message semaphor \n");
		return -1;
	}

	for(int n=0; n <16; n++)
	{
		if(semctl(semid, n, SETVAL, n) < 0)
		{
			perror("Error SETVAL");
			return -1;
		}
	}

	return 0;
}




