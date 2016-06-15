//gcc builtin_command.c parsing.c read_command.c type_prompt.c lsyshell.c -o skt
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h> 
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <time.h>
#include <fcntl.h>
#include <grp.h>
#include <dirent.h>
#define MAX_PROMPT 1024
#define MAXLINE 4096 //the length of all args is ARG_MAX
#define MAXARG 20

struct parse_info;
struct passwd *pwd;
char *buffer;
char *newbuffer;
void type_prompt(char*);
int read_command(char **,char **,char*,char*);
int builtin_command(char *,char **);
int parsing(char **,int,struct parse_info *);
void proc(void);
void sig_handler(int sig);
char *zhilin;
void runanother(char *);
void printalias();//打印alias结构体
int panduanalias(char*,char*,int);//判断几级指令
#ifndef STRUCT_PARSE_INFO
#define STRUCT_PARSE_INFO
#define BACKGROUND 			1
#define IN_REDIRECT 		2
#define OUT_REDIRECT 		4
#define OUT_REDIRECT_APPEND	8
#define IS_PIPED 			16
struct parse_info 
{
    int flag;  //表明使用了哪些功能的标志位
	char* in_file; //输入重定向的文件名	
    char* out_file;//输出重定向的文件名
    char* command2;//命令2
	char** parameters2;//命令2的参数表
};
#endif
struct aliasinfo{
	char name[500];
	char refer[500];
	char p1[500];
	char p2[500];
	char p3[500];
}a[500];
int numofalias;
//
