#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	char semname[] = "/test.sem";
	sem_unlink(semname);

	sem_t *semid = sem_open(semname, O_CREAT, 0666, 66);


	if(semid == SEM_FAILED)
	{
		perror("Error create semaphore");
		printf("Error");
		return -1;
	}


	return 0;
}
