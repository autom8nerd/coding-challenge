# UART Configuration in C

This repository contains a C program that initializes, configures, and tests a UART (serial) interface on Linux using the `termios` API. It demonstrates non-blocking I/O using `select()`, hardware configuration (8N1), and standard read/write operations.

## Instructions for Running the Program

### 1. Clone the Repository
First, pull the code down to your local machine and navigate into the directory:

    git clone https://github.com/autom8nerd/coding-challenge.git
    cd coding-challenge

### 2. Configure the Device Path
Before compiling, you need to specify which serial interface you want the program to connect to. 

Open `conf-UART.c` in your text editor and locate the `path` character array. Update it to match your desired hardware interface address (e.g., `/dev/ttyUSB0` or `/dev/ttyS0`).

#### 🛠️ Testing without Hardware (Virtual Devices)
If you do not have a physical serial device plugged in, you can simulate a connection using `socat`. Open a terminal and run the following command to create a virtual null-modem cable:

    socat -d -d pty,raw,echo=0 pty,raw,echo=0

This will output two virtual device paths (for example, `/dev/pts/2` and `/dev/pts/3`). Copy the first path and paste it into the `path` variable in the C code.

### 3. Compile the Code
Compile the program using the GCC compiler:

    gcc conf-UART.c

### 4. Run the Program
Execute the compiled binary:

    ./a.out

*(Note: If you are using the socat virtual devices, you can interact with your C program by opening a separate terminal and using `cat < /dev/pts/X` to listen, and `echo "message" > /dev/pts/X` to talk back!)*
