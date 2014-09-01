#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>

int stop = 0;

#define SLEEPTIME 500000

void sighand(int sig)
{
	stop = 1;
}

char *fifopath = "/tmp/fifotst";

int openfifo(char *path)
{
	int fd = open(fifopath, O_WRONLY | O_NONBLOCK);
	if (fd < 0)
		return -1;
	return fd;
}

void cleanup()
{
	unlink(fifopath);
}
	
int main(int argc, char **argv)
{
	
	int ret;
	int fd;
	struct pollfd pollfds[1];

	cleanup();

	signal(SIGINT, sighand);
	signal(SIGTERM, sighand);

	ret = mkfifo(fifopath, 0666);

	if (ret) {
		fprintf(stderr, "Error creating %s fifo: %s\n", fifopath, strerror(errno));
		cleanup();
		exit(errno);
	}

	while (!stop) {
		fd = openfifo(fifopath);
		if (fd < 0) {
			if (errno == ENXIO) {
				usleep(SLEEPTIME);
				continue;
			}
			else {
				fprintf(stderr, "Error opening fifo: %s\n", strerror(errno));
				cleanup();
				exit(errno);
			}
		}
		write(fd, "Got an event!\n", 14);
		close(fd);
		sleep(1);
	}
	close(fd);
	cleanup();
	exit(0);
}
	
