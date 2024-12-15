#include "uartwrap.h"

int UARTWRAP_SetOpt(int fd,int baudRate, int nBits, char nEvent, int nStop)
{
	struct termios newtio, oldtio;
	
	if ( tcgetattr(fd, &oldtio) != 0) { //获得驱动程序的默认参数放到oldtio中，备份下来
		perror("SetupSerial 1");
		return -1;
	}
	//设置新的参数
	// bzero( &newtio, sizeof( newtio ) );//清0，下面设置新的参数
    memset(&newtio, 0, sizeof(newtio));
	newtio.c_cflag |= CLOCAL | CREAD; 
	newtio.c_cflag &= ~CSIZE; 

	newtio.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
	newtio.c_oflag  &= ~OPOST;   /*Output*/

	switch( nBits )//设置数据位的个数
	{
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
	}

	switch( nEvent )//设置校验位
	{
        case 'O':
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E': 
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N': 
            newtio.c_cflag &= ~PARENB;
            break;
	}

	switch( baudRate )//设置波特率
	{
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 19200:
            cfsetispeed(&newtio, B19200);
            cfsetospeed(&newtio, B19200);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
	}
	
	if( nStop == 1 )//设置停止位
		newtio.c_cflag &= ~CSTOPB;
	else if ( nStop == 2 )
		newtio.c_cflag |= CSTOPB;
	
	newtio.c_cc[VMIN]  = 1;  /* 读数据时的最小字节数: 没读到这些数据我就不返回! */
	newtio.c_cc[VTIME] = 0; /* 等待第1个数据的时间: 
	                         * 比如VMIN设为10表示至少读到10个数据才返回,
	                         * 但是没有数据总不能一直等吧? 可以设置VTIME(单位是10秒)
	                         * 假设VTIME=1，表示: 
	                         *    10秒内一个数据都没有的话就返回
	                         *    如果10秒内至少读到了1个字节，那就继续等待，完全读到VMIN个数据再返回
	                         */

	tcflush(fd,TCIFLUSH);
	
	if((tcsetattr(fd, TCSANOW, &newtio))!=0)//tcsetattr函数设置行规程
	{
		perror("com set error");
		return -1;
	}
	
	return 0;
}

int UARTWRAP_OpenPort(char *com)
{
	int fd;
	//fd = open(com, O_RDWR|O_NOCTTY|O_NDELAY);
	fd = open(com, O_RDWR|O_NOCTTY);//打开设备结点，O_NOCTTY
    /*
    The O_NOCTTY flag tells UNIX that this program doesn't want to be the "controlling terminal" 
    for that port. If you don't specify this then any input (such as keyboard abort signals 
    and so forth) will affect your process. Programs like getty(1M/8) use this feature 
    when starting the login process, but normally a user program does not want this behavior. 
    */
	
    if (-1 == fd){
		return(-1);
    }
	
    if(fcntl(fd, F_SETFL, 0)<0) /* 设置串口为阻塞状态*/
    /*
    1.fcntl(fd,F_SETEL,FNDELAY);
    读数据时不等待，没有数据就返回0；
    2.fcntl(fd,F_SETFL,0);
    读数据时，没有数据阻塞
    */
    {
        printf("fcntl failed!\n");
        return -1;
    }
  
    return fd;
}

int UARTWRAP_InitDevice(char* dev_name, int baudrate)
{
    // open file device
    int fd = UARTWRAP_OpenPort(dev_name);
    if (fd < 0) return -1;

    // set option
    int iRet;
    iRet = UARTWRAP_SetOpt(fd, baudrate, 8, 'N', 1);
    if (iRet != 0) return -2;

    return fd;
}

void UARTWRAP_ReleaseDevice(int fd)
{
    if (fd >= 0) close(fd);
}