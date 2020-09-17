#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define MAJOR_NUM 100
#define IOCTL_SET_MSG _IOW(MAJOR_NUM, 0, char *)
#define IOCTL_GET_MSG _IOR(MAJOR_NUM, 1, char *)
#define IOCTL_GET_NTH_BYTE _IOWR(MAJOR_NUM, 2, int)


int main(void){

	int fd;
	int num;
	printf("%s\n","Inside main function" );
	fd=open("/proc/Chags",O_RDWR );
	
	if(fd < 0){
		printf("%s\n","Cannot open the file" );
		return 0;
	}

	printf("%s\n","Enter the data to send" );
	//scanf("%d",&num);

	//ioctl(fd,IOCTL_SET_MSG,(int32_t*)&num);

	ioctl(fd,IOCTL_GET_MSG,(int32_t*)&num);
	printf("Value got is %d\n",num);
	close(fd);
}
