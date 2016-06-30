#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>

int main ()
{
	int shmid;
	char shmname[] = "/tmp/mem.temp";
	size_t size = 2048 *1000 *1000;
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

	void * addr = shmat(shmid, NULL, 0);
	int *array;

	if((array = (int*)addr) == (int*)(-1) )
	{
		perror("Error attach shared memory");
		return -1;
	}

	for(int i=0; i< 1024 * 1024; i++)
	{
		array[i] = 42;
	}

	printf("%d", array[1000]);

	shmdt(array);

	return 0;
}



