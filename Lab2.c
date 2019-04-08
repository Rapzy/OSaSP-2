#include "stdio.h"
#include "stdlib.h"
#include "dirent.h"
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <time.h>
#include <libgen.h>

void GetDir(char *dir);
void PrintInfo(char *path);
char *filename, *progname;
int num_file=0, num_dir=1;

int  main(int argc, char *argv[]){
	filename = argv[2];
	progname = basename(argv[0]);
	GetDir(argv[1]);
	printf("Directories checked: %d\n", num_dir);
	printf("Files checked: %d\n", num_file);
    return 0;
}
void GetDir(char *dir){
	struct dirent *d;
	DIR *dp;
	if((dp = opendir(dir)) == NULL)
    {
      fprintf(stderr, "%s: Couldn't open %s.\n",progname,dir);
      return;
    }
	while ((d = readdir(dp))!= NULL){
		char path[1024];
		char buf[200];
		if (d->d_type == DT_DIR) {
			if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
	        	continue;
	        if(strcmp(dir,"/") == 0){
				snprintf(path, sizeof(path), "/%s", d->d_name);
			}
			else{
				snprintf(path, sizeof(path), "%s/%s", dir, d->d_name);
			}
			num_dir++;
		  	GetDir(path);
		}
		else {
			num_file++;
			if (strcmp(d->d_name,filename) == 0){
				snprintf(path, sizeof(path), "%s/%s", dir, d->d_name);
				PrintInfo(path);
			}
		}
	}
	closedir(dp);
}

void PrintInfo(char *path){
	struct stat fileStat;
	stat(path, &fileStat);

	struct timespec ts;
	char buf[PATH_MAX];
	char date[100];
	
	ts = fileStat.st_birthtimespec;
	
	strftime(date, sizeof(date), "%D %T", localtime(&ts.tv_sec));

	printf("%s %s %lld %llu ", realpath(path,buf),date,fileStat.st_size,fileStat.st_ino);
	printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
	printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
	printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
	printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
	printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
	printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
	printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
	printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
	printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
	printf( (fileStat.st_mode & S_IXOTH) ? "x\n" : "-\n");
}
