#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static char buf[5] = "12345";

int main()
{
	int fd;
	fd = open("/dev/helo", O_WRONLY);

	if (fd == 0)
	{
		printf("Fail to open file!\n");
		exit(1);
	}
	lseek(fd, 0, SEEK_SET);
	write(fd, &buf, 5);
	close(fd);
	return 0;
}
