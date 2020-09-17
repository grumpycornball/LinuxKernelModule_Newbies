obj-m = Parameter.o
obj-m = CDeviceDriver.o
obj-m = proc.o
obj-m = ProDriver.o
obj-m = ioctl.o
obj-m = Controller.o
obj-m = interval.o
obj-m = HelpMe.o
obj-m = Deny.o
obj-m = Keylogger.o

all:
	make -C /lib/modules/`uname -r`/build M=$(PWD) modules
clean:
	make -C /lib/modules/`uname -r`/build M=$(PWD) clean