#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>

int main ()
{
	int shmid;
	char shmname[] = "/tmp/mem.temp";
	size_t size = 2 *1024 *1024;
	key_t key = ftok(shmname, 1);

	if(key < 0)
	{
		perror("Error key");
		return -1;
	}


	shmid = shmget(key, size, IPC_CREAT | 0666 );
	if(shmid < 0)
	{
		perror("Error create shared memory");
		return -1;
	}

	void * addr = shmat(shmid, 0, 0);
	int *array;

	if((array = (int*)addr) == (int*)(-1) )
	{
		perror("Error attach shared memory");
		return -1;
	}

	unsigned int i;
	for(i = 0; i < 1048576; i++)
	{
		*((char*)addr+i) = 42;
	}
	/*
	for(int i=0; i< 1048576; i++)
	{
		//array[i] = 42;
	}
	array[1000000] = 42;
	*/

	//printf("%d", array[100]);

	//shmdt(array);

	return 0;
}



