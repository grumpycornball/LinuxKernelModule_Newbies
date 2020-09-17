# LinuxKernelModule_Newbies

# Why this repository?

Anyone who is beginning to learn linux module programming should definitely go through 'Linux Kernel Module Programming Guide' book (https://www.tldp.org/LDP/lkmpg/2.6/lkmpg.pdf ). I think this book has very well explained all the concepts and basics, but the examples in the book are outdated as in, the API calls used has changed since Linux Kernel 2.x.   

Although I highley recommend going through the source code and figure out the changes by yourself but still if you need a little help here you go.

# Linux kernel used:
4.15.0-117-generic


# Module program details

1. Parameter.c  	: Basic module that accepts parameters in module
2. CDeviceDriver.c : Basic character driver module
3. proc.c 			: Creating directory and using file operation in Virtual filesystem /proc
4. ProDriver.c 	: Character driver module with IOCTL operations
5. ioctl.c 		: C program to invoke ioctl operations
6. Controller.c    : Another C program to invoke ioctl operations
7. interval.c 		: Calling a function after some time interval using workqueue
8. HelpMe.h 		: Consider it a text file and sort of cheat sheet of the functions and arguments required during developement so you don't have to go through the linux source code again and again
9. Deny.c 			: Only allows one process to access proc dir created during initialization, Others are put to sleep. 
10. Keylogger.c 	: Basic Keylogger with the help of Keyboard.h api


# How to use

Just edit the Makefile and build the module using 'make' command followed by inserting module with 'insmod *ko'. In some examples you might have to create the character device using 'mknod /proc/chrdevice c 243 0', use your major number and minor number.
