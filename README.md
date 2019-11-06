# 2019-2_System_Programming
 2019 Fall Semester System Programming(ELEC462003, prof. Seokin Hong) Exercises & Assignments


### 190902 - Ch0. Introduction, Ch1. Linux for Beginners
- Terminal, shell, Vi editor
![Operating System Architecture](https://user-images.githubusercontent.com/54846646/68307643-f3c1c400-00ee-11ea-8a8d-a1e5c0e365e4.JPG)
         

### 190909 - Ch2 Users Files and the Manual(File I/O)
- Unix file interface, Reading/creating/writing files, File descriptors, Buffering, Kernel/User mode, Unix time, utmp file, errors
```sh
$ man, who, cp, login
```
```c
open() close(), read(), write(), lseek(), perror()
```

### 190916 - Ch3. Directories and File Properties
- Directory is a list of files, reading directory, types and properties of files, Bit sets and bit masks, User and group ID numbers
```sh
$ ls
```
```c
opendir(), readdir(), closedir(), seekdir()
stat()
chmod(), chown(), utime()
rename()
```

### 190923 - Ch4. File Systems
- Writing pwd
- Unix file system tree, inodes and data blocks, Hard & symbolic Links, how pwd works, Mounting file systems
```sh
$ pwd
```
```c
mkdir(), rmdir(), chdir()
link(), unlink(), rename(), symlink()
```

### 190930 - Ch5. Connection Control
- Studying sty, Device Driver
- Similarities & Differences between files and devices, Attributes of connections, Controlling device drivers
```sh
$ stty, write
```
```c
fcntl(), ioctl()
tcsetattr(), tcgetattr()
```

### 191007 - Ch6. Programming for Humans
- Terminal Control and Signals
- Reading and changing settings of the terminal driver, Modes of the terminal driver, Non-blocking input, Timeouts, Ctrl-C
```c
fcntl(), signal()
```

### 191021 - Midterm Exam

### 191028 - Ch7. Event Driven Programming
- Writing a Video Game
- Asynchronous events, Curses library, Alarms and interval timers, Signal handling, Reentrant code, Critical sections, Asynchrounous input
```c
alarm(), setitimer(), getitimer()
kill(), pause()
sigaction(), sigprocmask()
fcntl(), aio_read()
```

### 191104 - Ch8. Processes and Programs 
- Studying sh
- What a Unix **shell** does, Unix model of process, How to run a **program**, How to create a **process**, How parent and child processes communicate
```sh
$ sh, ps
```
```c
fork(), exec(), wait(), exit()
```
