#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<errno.h>
#include<limits.h>//realpath
#include<libgen.h>//basename
#include<sys/vfs.h>
#include<stdint.h>



char *AppName;
long double byt = 0;
long int bloks = 0;

int ProcessFolder(char *curPath){
	DIR *curDir;
	curDir = opendir(curPath);
	if(curDir == NULL) {
		fprintf(stderr,"%s: %s: %s\n", AppName, curPath, strerror(errno));
		errno = 0;
		return -1;
	}
	struct dirent *dent;
	struct stat buf;
	char *file = alloca(strlen(curPath) + NAME_MAX + 2);
	if(file==NULL){
		fprintf(stderr,"%s: %s.", AppName, strerror(errno));
		return -1;
	}
	long int sum = 0;
	long int lobloks = 0;
	int count = 0;
	errno = 0;
	while(dent = readdir(curDir)){
		if(strcmp(".", dent->d_name) && strcmp("..", dent->d_name)){
			strcpy(file, curPath);
			strcat(file, "/");
 			strcat(file, dent->d_name);
			if (lstat(file,&buf) == -1) {
				fprintf(stderr,"%s: %s: %s\n", AppName, curPath, strerror(errno));
				return -1;
			}
			if(!S_ISDIR(buf.st_mode)){
				sum+=buf.st_size;
				lobloks+=buf.st_blocks;
				count++;
			}
			if(S_ISDIR(buf.st_mode)){
				ProcessFolder(file);
			}
		}
	}

	if(errno!=0){
		fprintf(stderr,"%s: %s: %s\n", AppName, curPath, strerror(errno));
		return -1;
	}
	long double pers = sum/10500;
	printf("%s %d %ld %ld\n", curPath, count, sum, lobloks);
	byt = byt + sum;
	bloks = bloks + lobloks;
	if(closedir(curDir)==-1){
		fprintf(stderr,"%s: %s: %s\n", AppName, curPath, strerror(errno));
		return -1;
	}
	return 0;
}
	long get_fs_size(const char *anyfile)
	{
		struct statfs buf;
		statfs(anyfile, &buf);
		return buf.f_blocks;
	}
	long get_fs_free(const char *anyfile)
{
		struct statfs buf;
		statfs(anyfile, &buf);
		return buf.f_bfree;
}
	size_t blksize(const char *anyfile){
		struct stat st;
stat(anyfile, &st);
return st.st_blksize;
}

int main(int argc, char **argv){
	AppName = alloca(strlen(basename(argv[0])));
	strcpy(AppName, basename(argv[0]));
	if(argc<2){
		fprintf(stderr, "%s:  РњР°Р»Рѕ Р°СЂРіСѓРјРµРЅС‚РѕРІ.\n", AppName);
		return -1;
	}
	
	
	
	if(realpath(argv[1], NULL) == NULL) {
		fprintf(stderr,"%s: %s: %s\n", AppName, argv[1], strerror(errno));
		return -1;
	}
	
	ProcessFolder( realpath(argv[1], NULL));
	struct stat st_s1;
	stat(argv[1], &st_s1);
	
	long double size = st_s1.st_size;
	
	

	fprintf(stderr,"Zanimaet %Lf percents\n", (byt/(bloks*512))*100);
	return 0;
}