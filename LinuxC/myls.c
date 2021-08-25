#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
#include<string.h>
#include<grp.h>
#include<pwd.h>
#include<time.h>

#define Ten 10
#define MAX_SIZE 255

void isFile(char *name,char *Filename);
void read_Dir(char *dir);
void read_Dir_LR(char *dir);


void FileNameProcessing (char* path);
void A_FileList(char* path);
void R_FileList(char* path);
void RL_FileList(char* name,char* );
void RA_FileList(char* path);
void L_FileList(char* path);
void AL_FileList(char* path);
void NormalFileList(char* path);


int main(int argc,char *argv[])
{
    char Parameter;
    char path[MAX_SIZE]={'\0'};
    int i,j,k;
    for(i=1;i<argc;i++){
        if(argv[i][0]=='-'){
            Parameter=argv[i][1]+argv[i][2];
        }else{
            strcpy(path,argv[i]);
        }
    }
    if(strcmp)
    printf("Path:%s Para:%c %d\n",path,Parameter,Parameter);
    switch (Parameter)
    {
    case 'A':
        
        break;
    case 'R':
        isFile(path,path);
        break;

    case 'L':
        L_FileList(path);    
        break;

    case 'A'+'L':
        
        break;
    case 'A'+'R':
        
        break;

    case 'X':
        RL_FileList(path,path);
        break;
    
    default:
        NormalFileList(path);
        break;
    }

    }

void read_Dir(char *dir)
{
    struct stat buf;
    char path[MAX_SIZE];
    DIR *dp;
    struct dirent *sdp;
    dp=opendir(dir);
    if(dp==NULL){
        perror("Opendir Error");
        return;
    }
    while((sdp=readdir(dp))!=NULL){
        stat(sdp->d_name,&buf);
        if(sdp->d_name[0]=='.')
            continue;
        sprintf(path,"%s/%s",dir,sdp->d_name);
        if(!S_ISDIR(buf.st_mode))
            printf("%s:\n",path);
        isFile(path,sdp->d_name);        
    }
    closedir(dp);
    return;
}

void isFile(char *name,char *Filename)
{
    struct stat sb;
    int ret = 0;
    ret = stat(name,&sb);
    if(ret == -1){
        perror("stat error");
        return;
    }

    if(S_ISDIR(sb.st_mode)){
        read_Dir(name);
    }else{
        printf("%-10s\t\n",Filename);
    }
    
    return;
}

void FileNameProcessing (char* path)
{
    DIR* DirFlow;
    struct dirent *fNP;
    DirFlow=opendir(path);
    if(DirFlow==NULL){
        perror("Open:");
        exit(1);
    }
    
}

void NormalFileList(char* path)
{
    DIR* dp;
    struct dirent *sdp;
    dp=opendir(path);
    if(dp==NULL){
        perror("Opendir Error");
        exit(1);
    }
    while ((sdp=readdir(dp))!=NULL)
    {
        if(sdp->d_name[0]=='.')
            continue;
        printf("%-10s\t",sdp->d_name);
        /* code */
    }
    printf("\n");
    closedir(dp);
}

void L_FileList(char* path)
{
    DIR* dp;
    struct stat buf;
    struct dirent *sdp;
    char buf_time[32];
	struct passwd *psd;
	struct group *grp;
    dp=opendir(path);
    if(dp==NULL){
        perror("Opendir Error");
        exit(1);
    }
    while((sdp=readdir(dp))!=NULL)
    {
        if(sdp->d_name[0]=='.')
            continue;
        stat(sdp->d_name,&buf);
        if(S_ISLNK(buf.st_mode)){
		printf("l");
	}else if(S_ISREG(buf.st_mode)){
		printf("-");
	}else if(S_ISDIR(buf.st_mode)){
		printf("d");
	}else if(S_ISCHR(buf.st_mode)){
		printf("c");
	}else if(S_ISBLK(buf.st_mode)){
		printf("b");
	}else if(S_ISFIFO(buf.st_mode)){
		printf("f");
	}else if(S_ISSOCK(buf.st_mode)){
		printf("s");
	}
	if(buf.st_mode&S_IRUSR){
		printf("r");
	}else{
		printf("-");
	}
	if(buf.st_mode&S_IWUSR){
		printf("w");
	}else{
		printf("-");
	}
	if(buf.st_mode&S_IXUSR){
		printf("x");
	}else{
		printf("-");
	}

	if(buf.st_mode&S_IRGRP){
		printf("r");
	}else{
		printf("-");
	}
	if(buf.st_mode&S_IWGRP){
		printf("w");
	}else{
		printf("-");
	}
	if(buf.st_mode&S_IXGRP){
		printf("x");
	}else{
		printf("-");
	}

	if(buf.st_mode&S_IROTH){
		printf("r");
	}else{
		printf("-");
	}
	if(buf.st_mode&S_IWOTH){
		printf("w");
	}else{
		printf("-");
	}
	if(buf.st_mode&S_IXOTH){
		printf("x");
	}else{
		printf("-");
	}
	printf(" ");

	psd=getpwuid(buf.st_uid);
	grp=getgrgid(buf.st_gid);
	printf("%-4d",buf.st_nlink);
	printf("%-8s",psd->pw_name);
	printf("%-8s",grp->gr_name);

	printf("%6d",buf.st_size);
	strcpy(buf_time,ctime(&buf.st_mtime));
	buf_time[strlen(buf_time)-1]=' ';
	buf_time[strlen(buf_time)-2]=' ';
	buf_time[strlen(buf_time)-3]='\b';
	buf_time[strlen(buf_time)-4]='\b';
	buf_time[strlen(buf_time)-5]=' ';
	buf_time[strlen(buf_time)-7]='\b';
	buf_time[strlen(buf_time)-9]=' ';
	buf_time[strlen(buf_time)-5-18]=' ';
	buf_time[strlen(buf_time)-5-19]=' ';
	buf_time[strlen(buf_time)-5-20]='\b';
	printf(" %s",buf_time);
	if(S_ISLNK(buf.st_mode)){
		printf("\033[;34m%s\033[0m\n",sdp->d_name);
	}else if(S_ISREG(buf.st_mode)){
		printf("\033[;33m%s\033[0m\n",sdp->d_name);
	}else if(S_ISDIR(buf.st_mode)){
		printf("\033[;35m%s\033[0m\n",sdp->d_name);
	}else if(S_ISCHR(buf.st_mode)){
		printf("\033[;34m%s\033[0m\n",sdp->d_name);
	}else if(S_ISBLK(buf.st_mode)){
		printf("\033[;31%s\033[0m\n",sdp->d_name);
	}else if(S_ISFIFO(buf.st_mode)){
		printf("\033[;37m%s\033[0m\n",sdp->d_name);
	}else if(S_ISSOCK(buf.st_mode)){
		printf("\033[;36m%s\033[0m\n",sdp->d_name);
	}
    }
}


void RL_FileList(char* name,char* Filename){
    struct stat sb;
    DIR* dp;
    struct stat buf;
    struct dirent *sdp;
    char buf_time[32];
	struct passwd *psd;
	struct group *grp;
    int ret = 0;
    ret = stat(name,&sb);
    if(ret == -1){
        perror("stat error");
        return;
    }

    if(S_ISDIR(sb.st_mode)){
        read_Dir_LR(name);
    }else{
        stat(sdp->d_name,&buf);
        if(S_ISLNK(buf.st_mode)){
		printf("l");
	}else if(S_ISREG(buf.st_mode)){
		printf("-");
	}else if(S_ISDIR(buf.st_mode)){
		printf("d");
	}else if(S_ISCHR(buf.st_mode)){
		printf("c");
	}else if(S_ISBLK(buf.st_mode)){
		printf("b");
	}else if(S_ISFIFO(buf.st_mode)){
		printf("f");
	}else if(S_ISSOCK(buf.st_mode)){
		printf("s");
	}
	if(buf.st_mode&S_IRUSR){
		printf("r");
	}else{
		printf("-");
	}
	if(buf.st_mode&S_IWUSR){
		printf("w");
	}else{
		printf("-");
	}
	if(buf.st_mode&S_IXUSR){
		printf("x");
	}else{
		printf("-");
	}

	if(buf.st_mode&S_IRGRP){
		printf("r");
	}else{
		printf("-");
	}
	if(buf.st_mode&S_IWGRP){
		printf("w");
	}else{
		printf("-");
	}
	if(buf.st_mode&S_IXGRP){
		printf("x");
	}else{
		printf("-");
	}

	if(buf.st_mode&S_IROTH){
		printf("r");
	}else{
		printf("-");
	}
	if(buf.st_mode&S_IWOTH){
		printf("w");
	}else{
		printf("-");
	}
	if(buf.st_mode&S_IXOTH){
		printf("x");
	}else{
		printf("-");
	}
	printf(" ");

	psd=getpwuid(buf.st_uid);
	grp=getgrgid(buf.st_gid);
	printf("%-4d",buf.st_nlink);
	printf("%-8s",psd->pw_name);
	printf("%-8s",grp->gr_name);

	printf("%6d",buf.st_size);
	strcpy(buf_time,ctime(&buf.st_mtime));
	buf_time[strlen(buf_time)-1]=' ';
	buf_time[strlen(buf_time)-2]=' ';
	buf_time[strlen(buf_time)-3]='\b';
	buf_time[strlen(buf_time)-4]='\b';
	buf_time[strlen(buf_time)-5]=' ';
	buf_time[strlen(buf_time)-7]='\b';
	buf_time[strlen(buf_time)-9]=' ';
	buf_time[strlen(buf_time)-5-18]=' ';
	buf_time[strlen(buf_time)-5-19]=' ';
	buf_time[strlen(buf_time)-5-20]='\b';
	printf(" %s",buf_time);
	if(S_ISLNK(buf.st_mode)){
		printf("\033[;34m%s\033[0m\n",sdp->d_name);
	}else if(S_ISREG(buf.st_mode)){
		printf("\033[;33m%s\033[0m\n",sdp->d_name);
	}else if(S_ISDIR(buf.st_mode)){
		printf("\033[;35m%s\033[0m\n",sdp->d_name);
	}else if(S_ISCHR(buf.st_mode)){
		printf("\033[;34m%s\033[0m\n",sdp->d_name);
	}else if(S_ISBLK(buf.st_mode)){
		printf("\033[;31%s\033[0m\n",sdp->d_name);
	}else if(S_ISFIFO(buf.st_mode)){
		printf("\033[;37m%s\033[0m\n",sdp->d_name);
	}else if(S_ISSOCK(buf.st_mode)){
		printf("\033[;36m%s\033[0m\n",sdp->d_name);
	}
    }
    
    return;

}

void read_Dir_LR(char *dir)
{
    struct stat buf;
    char path[MAX_SIZE];
    DIR *dp;
    struct dirent *sdp;
    dp=opendir(dir);
    if(dp==NULL){
        perror("Opendir Error");
        return;
    }
    while((sdp=readdir(dp))!=NULL){
        stat(sdp->d_name,&buf);
        if(sdp->d_name[0]=='.')
            continue;
        sprintf(path,"%s/%s",dir,sdp->d_name);
        if(!S_ISDIR(buf.st_mode))
            printf("%s:\n",path);
        RL_FileList(path,sdp->d_name);        
    }
    closedir(dp);
    return;
}

