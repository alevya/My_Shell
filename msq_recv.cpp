#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

int main()
{
	char mqName[] = "/test.mq";
	char pathresult[] = "/home/alevya/message.txt";
	mqd_t mq;

	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 1024;
	attr.mq_curmsgs = 0;

	char buffer[1025] = {0};
	int stop = 0;
	FILE * fd;

	mq = mq_open(mqName, O_RDONLY | O_CREAT | O_EXCL, 0666, &attr);

	if(mq<0)
	{
		printf("Error open queue");
		return -1;
	}

	do{
		ssize_t bytes;
		bytes = mq_receive(mq, buffer, 1024, NULL);

		if(! strncmp(buffer, "exit", strlen("exit")))
		{
			stop = 1;
		}
		else
		{
			printf("Received: %s\n", buffer);

			fd = fopen(pathresult, "w");
			fprintf(fd, "%s",  buffer);

			fclose(fd);
		}
	}while(!stop);

	mq_close(mq);
	mq_unlink(mqName);


	return 0;
}


