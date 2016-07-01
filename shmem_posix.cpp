#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main ()
{
	char name[] = "/test.shm";

	int memid = shm_open(name, O_CREAT | O_RDWR, 0666);

	if(memid < 0)
	{
		perror("Error create mem");
		return -1;
	}

	off_t size = 1024 * 1024;

	if(ftruncate(memid, size) < 0)
	{
		perror("Error truncate memory");
		return -1;
	}

	void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, memid, 0);

	int *array;
	if((array = (int*)addr) == (int*)(MAP_FAILED) )
	{
			perror("Error attach shared memory");
			return -1;
	}

	unsigned int i;
	for(i = 0; i < 1048576; i++)
	{
		//*(array+i) = 13;
		*((char*)addr+i) = 13;
	}

	//printf("%d", ((char*)addr)[1000]);

	return 0;
}



