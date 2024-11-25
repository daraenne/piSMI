#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>

#define SIGTX 44
#define REGISTER_UAPP _IO('S', 'g')
#define DRIVER_FILENAME "/dev/KpiSMI"

int fd;

void signalhandler(int sig) {
	write(fd, "1", 1);
	printf("sighandler\r\n");
}

int main(void){
	fd = 0;
	signal(SIGTX, signalhandler);

	printf("PID: %d\n", getpid());

	/* Open the device file */
	fd = open("/dev/KpiSMI", O_RDWR);
	if(fd < 0) {
		perror("Could not open device file");
		return -1;
	}

	/* Register app to KM */
	if(ioctl(fd, REGISTER_UAPP, NULL)) {
		perror("Error registering app");
		close(fd);
		return -1;
	}

	/* Wait for Signal */
	printf("Wait for signal...\n");
	while(1) sleep(1);

	return 0;
}