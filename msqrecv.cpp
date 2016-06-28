#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <cstdio>
#include <iostream>
#include <fcntl.h>

int main()
{
	int msqid;
	char pathname[] = "/tmp/msg.temp";
	char pathresult[] = "/home/alevya/message.txt";
	key_t key;
	int len;
	int maxlen;

	FILE * fd;
	//fd = open("/home/box/message.txt", "w");
	//fd = open("/home/box/message.txt", 0666);


	struct message
		{
			long mtype;
			char mtext[80];
		} msg;

	if((key = ftok(pathname, 1)) <0)
	{
		printf("Error generate key \n");
		return -1;
	}

	if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
	{
		printf("Error get message que \n");
		return -1;
	}

	while(1)
	{
		maxlen = 80;

		if(len = msgrcv(msqid, (struct message*) &msg, maxlen, 0, 0) < 0)
		{
			printf("Error receive message \n");
			return -1;
		}

		printf("message type = %ld, info = %s\n", msg.mtype, msg.mtext);

		fd = fopen(pathresult, "w");
		fprintf(fd, "%s\n", msg.mtext);

		fclose(fd);


		//fprintf(fd, "%s\n", msg.mtext);


	}


	return 0;
}
