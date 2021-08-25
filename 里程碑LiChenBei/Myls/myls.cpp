#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<grp.h>
#include<pwd.h>
#include<time.h>
#include<errno.h>
#include<limits.h>

#define NONE 0
#define A	1
#define L	2
#define R	4

#define MAXROWLEN 80
void display_attribute(struct stat buf,char * name);
void display_single(char *name);
void display(int flag,char *pathname);
void display_recurrence(char *name);
void display_dir(int flag_param,char *path);

int g_leave_len=MAXROWLEN;
int g_maxlen;

int main(int argc,char *argv[])
{
	int i=0,j=0,k=0,num=0;
	char path[PATH_MAX+1];
	char param[32];
	//存储命令行参数
	int flag_param = NONE;
	struct stat buf;

	for(i=1;i<argc;i++)
	{
		if(argv[i][0]=='-'){
			for(k=1;k<strlen(argv[i]);k++,j++){
				param[j]=argv[i][k];
				//将参数存入param[]
			}
			num++;
		}
	}

	for(i=0;i<j;i++){
		if (param[i]=='a'){
			flag_param|=A;
			continue;
		}else if(param[i]=='l'){
			flag_param|=L;
			continue;
		}else if(param[i]=='R'){
			flag_param|=R;
			continue;
		}	else{
			printf("my_ls:invalid option -%c\n",param[i]);
				exit(1);
		}
	}
	param[j]='\0';

	if((num+1)==argc){
		strcpy(path,"./");
		path[2]='\0';
		display_dir(flag_param,path);
		return 0;
	}
	i=1;
	do{
		if(argv[i][0]=='-'){
			i++;
			continue;
		}else{
			strcpy(path,argv[i]);
			
			if(stat(path,&buf)==-1)
				perror("stat");
			if(S_ISDIR(buf.st_mode)){
				if(path[strlen(argv[i]-1)]!='/'){
					path[strlen(argv[i])]='/';
					path[strlen(argv[i]+1)]='\0';
				}else{
					path[strlen(argv[i])]='\0';
					display_dir(flag_param,path);
					i++;
				}
			}
			else{
					display(flag_param,path);
					i++;
				}
		}
	}while(i<argc);
	return 0;
}

void display_dir(int flag_param,char *path)
{
	DIR *dir;
	struct dirent *ptr;
	int count =0;
	char filenames[256][PATH_MAX+1],temp[PATH_MAX+1];
	
	dir = opendir(path);
	if(!dir){
		perror("文件打开");
	}
	while ((ptr=readdir(dir))!=NULL){
		if(g_maxlen<strlen(ptr->d_name)){
			g_maxlen=strlen(ptr->d_name);
		}
		count++;
	}
	closedir(dir);
	int i,j,len=strlen(path);
	dir=opendir(path);
	for(i=0;i<count;i++){
		ptr=readdir(dir);
		if(ptr==NULL){
			perror("文件读取");
		}
		strncpy(filenames[i],path,len);
		filenames[i][len]='\0';
		strcat(filenames[i],ptr->d_name);
		filenames[i][len+strlen(ptr->d_name)]='\0';
	}
	for(i=0;i<count-1;i++){
		for(j=0;j<count-1-i;j++){
			if(strcmp(filenames[j],filenames[j+1])>0){
				strcpy(temp,filenames[j+1]);
				temp[strlen(filenames[j+1])]='\0';
				strcpy(filenames[j+1],filenames[j]);
				filenames[j+1][strlen(filenames[j])]='\0';
				strcpy(filenames[j],temp);
				filenames[j][strlen(temp)]='\0';
			}
		}
	}


	if(flag_param==R){
		for(i=0;i<count;i++){
		display_single(filenames[i]);
	}
	for(i=3;i<count;i++){
		struct stat buf;
		lstat(filenames[i],&buf);
		if(S_ISDIR(buf.st_mode)&&filenames[i][0]!='.'){
			printf("\n%s:\n",filenames[i]);
			strcat(filenames[i],"/");
			display_dir(flag_param,filenames[i]);
		}
	}
	}else{
		for(i=0;i<count;i++){
			display(flag_param,filenames[i]);
		}
	}
	closedir(dir);
	if((flag_param&L)==0){
	printf("\n");
	}
	return;
}

// void display_recurrence(char *name)
// {
// 	DIR* RootDir;
// 	int ret,i,j;
// 	struct stat info;
// 	struct dirent* Dir;
// 	char dirlist[100][NAME_MAX+1];
// 	RootDir = opendir(name);
// 	if(RootDir ==NULL)
// 	{
// 		perror("文件打开错误");
// 	}
// 	printf("%s:\n",name);
// 	i=0;
// 	while((Dir=readdir(RootDir))!=NULL){
// 		if(Dir->d_name[0]=='.'){
// 			continue;
// 		}
// 		char TempStr[NAME_MAX+1];
// 		char Last = name[strlen(name)-1];
// 		if(Last=='/')
// 		{
// 			sprintf(TempStr,"%s%s",name,Dir->d_name);
// 		}else{
// 			sprintf(TempStr,"%s/%s",name,Dir->d_name);
// 		}
// 		ret = lstat(TempStr,&info);
// 		if(ret<0){
// 			perror("lstat error");
// 			exit(1);
// 		}
// 		if(S_ISDIR(info.st_mode)){
// 			strcpy(dirlist[i++],TempStr);
// 			if(i>100){
// 				i--;
// 				break;
// 			}
// 		}
		
// 	}
// 	closedir(RootDir);
// 	for(j=0;j<i;j++){
// 		display_recurrence(dirlist[j]);
// 	}
// 	return;
// }
void display_single(char *name)
{
	int i,len;
	struct stat buf;
	lstat(name,&buf);
	if(g_leave_len<g_maxlen){
		printf("\n");
		g_leave_len=MAXROWLEN;
	}
	len=strlen(name);
	len=g_maxlen-len;
	if(S_ISLNK(buf.st_mode)){
		printf("\033[;34m%s\033[0m",name);
	}else if(S_ISREG(buf.st_mode)){
		printf("\033[;33m%s\033[0m",name);
	}else if(S_ISDIR(buf.st_mode)){
		printf("\033[;35m%s\033[0m",name);
	}else if(S_ISCHR(buf.st_mode)){
		printf("\033[;34m%s\033[0m",name);
	}else if(S_ISBLK(buf.st_mode)){
		printf("\033[;31%s\033[0m",name);
	}else if(S_ISFIFO(buf.st_mode)){
		printf("\033[;37m%s\033[0m",name);
	}else if(S_ISSOCK(buf.st_mode)){
		printf("\033[;36m%s\033[0m",name);
	}
	for(i=0;i<len;i++){
		printf(" ");
	}
	printf(" ");
	g_leave_len-=(g_maxlen+2);
}

void display(int flag,char *pathname)
{
	int i,j;
	struct stat buf;
	char name[NAME_MAX+1];
	for(i=0,j=0;i<strlen(pathname);i++)
	{
		if(pathname[i]=='/'){
			j =0;
			continue;
		}
		name[j++]=pathname[i];
	}
	name[j]='\0';
		if(lstat(pathname,&buf)==-1){
			perror("lstat");
		}
		switch(flag){
			case NONE:
				if (name[0]!='.'){
					display_single(name);
				}
				break;
			case A:
				display_single(name);
				break;
			case L:
				if(name[0]!='.'){
					display_attribute(buf,name);
				}
				break;
			case A+L:
				display_attribute(buf,name);
				break;
			// case R:
			// 	if(S_ISDIR(buf.st_mode))
			// 	display_recurrence(name);
			// 	printf("%-s\n",name);
				// break;
			default:
				break;
		}
}
void display_attribute(struct stat buf,char *name)
{
	char buf_time[32];
	struct passwd *psd;
	struct group *grp;

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
		printf("\033[;34m%s\033[0m\n",name);
	}else if(S_ISREG(buf.st_mode)){
		printf("\033[;33m%s\033[0m\n",name);
	}else if(S_ISDIR(buf.st_mode)){
		printf("\033[;35m%s\033[0m\n",name);
	}else if(S_ISCHR(buf.st_mode)){
		printf("\033[;34m%s\033[0m\n",name);
	}else if(S_ISBLK(buf.st_mode)){
		printf("\033[;31%s\033[0m\n",name);
	}else if(S_ISFIFO(buf.st_mode)){
		printf("\033[;37m%s\033[0m\n",name);
	}else if(S_ISSOCK(buf.st_mode)){
		printf("\033[;36m%s\033[0m\n",name);
	}
}