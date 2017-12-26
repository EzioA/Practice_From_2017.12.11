#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static char buf[256];

int main()
{
	int fd;
	int i;

	fd = open("/dev/helo", O_RDONLY);

	if (fd == 0)
	{
		printf("Fail to open file!\n");
		exit(1);
	}

	read(fd, &buf, 256);
	for (i = 0; i < 256; i++)
		printf("%c", buf[i]);
	close(fd);
	return 0;
}
