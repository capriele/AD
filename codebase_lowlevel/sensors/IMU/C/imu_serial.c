#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <math.h>
struct timeval tv;


/* The IMU-Arduino is on /dev/ttyUSB1 */
char *portname = "/dev/ttyUSB0";
char buf[256];

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

    /* wait for 24 characters to come in before read returns */
    toptions.c_cc[VMIN] = 12;
    /* no minimum time to wait before read returns */
    toptions.c_cc[VTIME] = 0;

    /* commit the options */
    tcsetattr(fd, TCSANOW, &toptions);

    /* Wait for the Arduino to reset */
    usleep(1000 * 1000);
    /* Flush anything already in the serial buffer */
    tcflush(fd, TCIFLUSH);
    /* read up to 128 bytes from the fd */
    //  int n = read(fd, buf, 128);

    FILE *imu_data;
    int k;

    imu_data = fopen("imu_data.txt", "w");

    if(!imu_data)
    {
        printf("error, can't open txt file");
        return 1;
    }
    long long int timestamp;
    gettimeofday(&tv, NULL);
    long long int t_zero = tv.tv_sec * pow(10, 6) + tv.tv_usec;
    timestamp = tv.tv_sec * pow(10, 6) + tv.tv_usec;
    printf("Logging imu data\n");
    while(timestamp - t_zero < 10000000) //10s
    {
        gettimeofday(&tv, NULL);
        timestamp = tv.tv_sec * pow(10, 6) + tv.tv_usec;
        serialport_read_until(fd);
        if(buf != "\n")
            fprintf(imu_data, "%lld   ", timestamp);
        fprintf(imu_data, "%s", buf);
        printf("%s", buf);
        //tcflush(fd, TCIFLUSH);
    }

    printf("Finish!\n");
    fclose(imu_data);


    return 0;
}

int serialport_read_until(int fd)
{
    char until = '\n';
    char b[1];
    int i = 0;
    do
    {
        int n = read(fd, b, 1);  // read a char at a time
        if(n == -1) return -1;   // couldn't read
        if(n == 0)
        {
            usleep(10);   // wait 100 usec try again
            continue;
        }
        buf[i] = b[0]; i++;
    }
    while(b[0] != until);

    buf[i] = 0;  // null terminate the string
    return 0;
}
