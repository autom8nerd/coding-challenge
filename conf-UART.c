#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

int main() {
  char path[] = "/dev/pts/4"; // letting compiler decide the size of addr
  int fd = open(path, O_NOCTTY |
                          O_RDWR); // opening the file in both read & write mode

  // Friendly error check just in case the USB isn't plugged in!
  if (fd < 0) {
    printf("Error opening %s: %s\n", path, strerror(errno));
    return 1;
  }

  size_t count = 1000; // how much to read from the device
  char buf[1000];
  speed_t v = B115200;      // standard value to set the speed
  struct termios termios_p; // pre-defined termios in the header

  tcgetattr(fd, &termios_p); // get the previous values of i/o device

  printf("%u %u %u %u\n", termios_p.c_iflag, termios_p.c_oflag,
         termios_p.c_cflag, termios_p.c_lflag);
  printf("%u\n", cfgetispeed(&termios_p)); // get the previous speed

  termios_p.c_cflag &= ~CSIZE;  // set the data bits to 0 by AND
  termios_p.c_cflag |= CS8;     // setting the 8 data bits
  termios_p.c_cflag &= ~PARENB; // disabling the parity
  termios_p.c_cflag &= ~CSTOPB; // clears the two stop bit flag, and configures
                                // it for one stop bit
  termios_p.c_cflag |= CREAD;   // enables the receiver
  termios_p.c_cflag |=
      CLOCAL; // ignores the modem control lines that were for the old systems

  cfsetispeed(&termios_p, v); // setting input speed
  cfsetospeed(&termios_p, v); // setting output speed
  tcsetattr(fd, TCSANOW, &termios_p);

  char buf1[] = "Hello world \r\n";
  size_t count1 = strlen(buf1);
  ssize_t toWrite =
      write(fd, buf1, count1); // writes buf1 into the external device
  printf("\nBytes written: %zd\n", toWrite);

  fd_set read_fds;       // special dtype for select() our watchlist
  FD_ZERO(&read_fds);    // clears the read_fds. always done initially
  FD_SET(fd, &read_fds); // tells select() which fd to watch

  struct timeval timeout; // struct for the timeout
  timeout.tv_sec = 5;
  timeout.tv_usec = 0;

  int result = select(fd + 1, &read_fds, NULL, NULL,
                      &timeout); // passed null for writefds and exceptds

  if (result < 0) {
    printf("Error: %s\n", strerror(errno));
  } else if (result == 0) {
    printf("Timeout\n");
  } else {
    ssize_t toRead = read(fd, buf, count - 1);

    if (toRead > 0) {
      buf[toRead] = '\0'; // to see the end of the data the device sent
      printf("Device replied: %s\n", buf);
    }
  }

  close(fd);
  return 0;
}
