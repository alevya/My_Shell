#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

using namespace std;

static char line[1024];

int main()
{
	FILE * in;
	int count;
	char c;

	while (1) {

			printf("$> ");
			fflush(NULL);

			if (!fgets(line, 1024, stdin))
				return 0;


	if(!(in = popen(line, "r")))
	{
		return 1;
	}

	fflush(NULL);
	remove("/home/alevya/result.out");
	int fd = open("/home/alevya/result.out", O_RDWR | O_CREAT, 0666);

	while((count = read(fileno(in), &c, 1))>0)
	{
		cout << c;
		write(fd, &c, 1);
	}

	close(fd);

	}


	pclose(in);
}



