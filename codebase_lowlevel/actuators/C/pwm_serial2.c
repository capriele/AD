#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
 
/* The PWM-Arduino is on /dev/ttyUSB0 */
char *portname = "/dev/ttyUSB0";
char buf[256];
char buf2[256]; 

int main(int argc, char *argv[])
{
	 int fd; //file descriptor
	 
	/* Open the file descriptor in non-blocking mode */
	 fd = open(portname, O_RDWR | O_NOCTTY);
	 
	/* Set up the control structure */
	 struct termios toptions;
	 
	 /* Get currently set options for the tty */
	 tcgetattr(fd, &toptions);
	 
	/* Set custom options */
	 
	/* 115200 baud */
	 cfsetispeed(&toptions, B115200);
	 cfsetospeed(&toptions, B115200);
	 /* 8 bits, no parity, no stop bits */
	 toptions.c_cflag &= ~PARENB;
	 toptions.c_cflag &= ~CSTOPB;
	 toptions.c_cflag &= ~CSIZE;
	 toptions.c_cflag |= CS8;
	 /* no hardware flow control */
	 toptions.c_cflag &= ~CRTSCTS;
	 /* enable receiver, ignore status lines */
	 toptions.c_cflag |= CREAD | CLOCAL;
	 /* disable input/output flow control, disable restart chars */
	 toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
	 /* disable canonical input, disable echo,
	 disable visually erase chars,
	 disable terminal-generated signals */
	 toptions.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	 /* disable output processing */
	 toptions.c_oflag &= ~OPOST;
	 
	/* wait for 2 characters to come in before read returns */
	 toptions.c_cc[VMIN] = 1;
	 /* no minimum time to wait before read returns */
	 toptions.c_cc[VTIME] = 0;
	 
	/* commit the options */
	 tcsetattr(fd, TCSANOW, &toptions);
	 
	/* Wait for the Arduino to reset */
	 usleep(1000*1000);
	 /* Flush anything already in the serial buffer */
	 tcflush(fd, TCIFLUSH);
	 /* read up to 128 bytes from the fd */
        //  int n = read(fd, buf, 128);

	 while(1){
		//tcflush(fd, TCIFLUSH);
		printf("done\n");
		scanf("%s",buf);        //4 values, between 0000 e 1024, concatenated without spaces
		printf("string: %s\n",buf);
		int m = write(fd, buf, 16);
		printf("serial write return: %d\n",m);		
		usleep(10);
	/*	int n = read(fd, buf, 128);
		printf("%d %c",n, buf); */
	 }

	return 0;
}

