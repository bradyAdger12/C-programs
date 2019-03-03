#include<sys/time.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<utime.h>
#include<fcntl.h>
#include<unistd.h>

//GLOBALS
struct tm timeBuf;

 void humanReadable(char *month, char *day, char *year, char *hours, char *minutes, char *seconds) {
       char readableDate[30];
       int y = atoi(year);
       int m = atoi(month) - 1;
       int d = atoi(day);
       int h = atoi(hours);
       int mi = atoi(minutes);
       int s = atoi(seconds);

       //error checks
       if(m > 11) {
	  char *monthError = "Incorrect month(ex. 1-12)\n";
       	  write(2, monthError, strlen(monthError));
	  exit(1);
       }

       if(d > 31) {
       	   char *dayError = "Incorrect day(ex. 1-31)\n";
	   write(2, dayError, strlen(dayError));
	   exit(1);
       }

       if(h > 23) {
       	   char *hourError = "Incorrect hour(ex. 0-23)\n";
	   write(2, hourError, strlen(hourError));
	   exit(1);
       }

       if(mi > 59) {
           char *minuteError = "Incorrect minute(ex. 0-59)\n";
	   write(2, minuteError, strlen(minuteError));
	   exit(1);
       }
       timeBuf.tm_year = y-1900; //subtract 1900 to make year correct
       timeBuf.tm_mon = m;
       timeBuf.tm_mday = d;
       timeBuf.tm_hour = h;
       timeBuf.tm_min = mi;
       timeBuf.tm_sec = s;

       
       //convert and print readable date
       strftime(readableDate, 30, "%c", &timeBuf);
       printf("%s\n", readableDate);
 }

int main(int argc, char **argv) {
        time_t timeConversion;
	time_t epochTime = time(NULL);
	struct utimbuf timeStruct;
	int fd;
	if(argc < 2 || argc > 8) {
		printf("usage: ./exec file month day year hour minute second\n");
		exit(1);
	}
        fd = open(argv[1], 0);
	if(fd == -1) {
		perror("ERROR MESSAGE");
		exit(1);
	}
  	humanReadable(argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]);      
        timeConversion = mktime(&timeBuf);
	timeStruct.actime = timeConversion;
	timeStruct.modtime = timeConversion;
	utime(argv[1], &timeStruct);
	close(fd);
}

