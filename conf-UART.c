#include <fcntl.h>
#include <linux/limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
int main() {
  char path[] = "/dev/ttyUSB0";
  int fd = open(path, O_NOCTTY | O_RDWR);
  size_t count = 1000;
  char buf[1000];
  speed_t v = B115200;
  struct termios termios_p;
  tcgetattr(fd, &termios_p);
  printf("%u %u %u %u", termios_p.c_iflag, termios_p.c_oflag, termios_p.c_cflag,
         termios_p.c_lflag);
  printf("\n");
  printf("%u", cfgetispeed(&termios_p));
  termios_p.c_cflag &= ~CSIZE;
  termios_p.c_cflag |= CS8;
  termios_p.c_cflag &= ~PARENB;
  termios_p.c_cflag &= ~CSTOPB;
  termios_p.c_cflag |= CREAD;
  termios_p.c_cflag |= CLOCAL;
  cfsetispeed(&termios_p, v);
  cfsetospeed(&termios_p, v);
  tcsetattr(fd, TCSANOW, &termios_p);
  ssize_t toRead = read(fd, buf, count);
  fd_set read_fd;

  char buf1[] = "Hello world \r\n";
  size_t count1 = strlen(buf1);
  ssize_t toWrite = write(fd, buf1, count1);
  printf("\n");
  printf("%zd", toWrite);
}
