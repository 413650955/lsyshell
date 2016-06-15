//http://blog.csdn.net/sjzs5590/article/details/8254527 ls -l 指令
//struct stat {
//	dev_t         st_dev;       //文件的设备编号
//	ino_t         st_ino;       //节点
//	mode_t        st_mode;      //文件的类型和存取的权限
//	nlink_t       st_nlink;     //连到该文件的硬连接数目，刚建立的文件值为1
//	uid_t         st_uid;       //用户ID
//	gid_t         st_gid;       //组ID
//	dev_t         st_rdev;      //(设备类型)若此文件为设备文件，则为其设备编号
//	off_t         st_size;      //文件字节数(文件大小)
//	unsigned long st_blksize;   //块大小(文件系统的I/O 缓冲区大小)
//	unsigned long st_blocks;    //块数
//	time_t        st_atime;     //最后一次访问时间
//	time_t        st_mtime;     //最后一次修改时间
//	time_t        st_ctime;     //最后一次改变时间(指属性)
//};
//
//st_mode是用特征位来表示文件类型的，特征位的定义如下：
//S_IFMT      0170000     文件类型的位遮罩
//S_IFSOCK    0140000     socket
//S_IFLNK     0120000     符号链接(symbolic link)
//S_IFREG     0100000     一般文件
//S_IFBLK     0060000     区块装置(block device)
//S_IFDIR     0040000     目录
//S_IFCHR     0020000     字符装置(character device)
//S_IFIFO     0010000     先进先出(fifo)
//S_ISUID     0004000     文件的(set user - id on execution)位
//S_ISGID     0002000     文件的(set group - id on execution)位
//S_ISVTX     0001000     文件的sticky位
//S_IRWXU     00700       文件所有者的遮罩值(即所有权限值)
//S_IRUSR     00400       文件所有者具可读取权限
//S_IWUSR     00200       文件所有者具可写入权限
//S_IXUSR     00100       文件所有者具可执行权限
//S_IRWXG     00070       用户组的遮罩值(即所有权限值)
//S_IRGRP     00040       用户组具可读取权限
//S_IWGRP     00020       用户组具可写入权限
//S_IXGRP     00010       用户组具可执行权限
//S_IRWXO     00007       其他用户的遮罩值(即所有权限值)
//S_IROTH     00004       其他用户具可读取权限
//S_IWOTH     00002       其他用户具可写入权限
//S_IXOTH     00001       其他用户具可执行权限
//
//S_ISLINGK(st_mode)      判断是否位符号链接
//S_ISREG(st_mode)        是否为一般文件
//S_ISDIR(st_mode)        是否为目录
//S_ISCHR(st_mode)        是否位字符装置文件
//S_ISBLK(s3e)            是否先进先出
//S_ISSOCK(st_mode)       是否为socket
#include "lsyshell.h"
#include "stdlib.h"
#include "dirent.h"
void show_file_allinfo(const char *filename)//单个的显示一个文件的属性  ls  -l file
{
	struct stat f_info;

	if (stat(filename, &f_info) < 0) //通过文件名filename获取文件信息，并保存在buf所指的结构体stat中
	{
		fprintf(stderr, "@@File to stat %s:%s", filename, strerror(errno));
		return;
	}
	//printf("%d\n", f_info.st_mode);
	switch (f_info.st_mode & S_IFMT)  //文件的类型
	{
	case S_IFSOCK:
		putchar('s');
		break;
	case S_IFLNK:
		putchar('l');
		break;
	case S_IFDIR:
		putchar('d');
		break;
	case S_IFCHR:
		putchar('c');
		break;
	case S_IFBLK:
		putchar('b');
		break;
	case S_IFREG:
		putchar('-');
		break;
	case S_IFIFO:
		putchar('p');
		break;
	default:
		break;
	}


	//主用户的权限
	if (f_info.st_mode & S_IRUSR)
		putchar('r');
	else
		putchar('-');
	if (f_info.st_mode & S_IWUSR)
		putchar('w');
	else
		putchar('-');
	if (f_info.st_mode & S_IXUSR)
		putchar('x');
	else
		putchar('-');
	//用户组的权限
	if (f_info.st_mode & S_IRGRP)
		putchar('r');
	else
		putchar('-');
	if (f_info.st_mode & S_IWGRP)
		putchar('w');
	else
		putchar('-');
	if (f_info.st_mode & S_IXGRP)
		putchar('x');
	else
		putchar('-');
	//其他人的权限
	if (f_info.st_mode & S_IROTH)
		putchar('r');
	else
		putchar('-');
	if (f_info.st_mode & S_IWOTH)
		putchar('w');
	else
		putchar('-');
	if (f_info.st_mode & S_IXOTH)
		putchar('x');
	else
		putchar('-');

	//int i;
	//int mov_n;
	//unsigned int mode;
	////其中，最低的9位(0-8)是权限，9-11是id，12-15是类型。
	//for (mov_n = 6; mov_n >= 0; mov_n -= 3)
	//{
	//	mode = f_info.st_mode >> mov_n;
	//	for (i = 2; i >= 0; i--)
	//	{
	//		printf("%d  ", mode & (1 << i));
	//		switch (mode & (1 << i))
	//		{
	//		case 0:putchar('-'); break;
	//		case 1:putchar('x'); break;
	//		case 2:putchar('w'); break;
	//		case 4:putchar('r'); break;
	//		default:
	//			break;
	//		}
	//	}
	//	i = i - 3;
	//}

	//打印硬连接数
	printf("%4d", f_info.st_nlink);

	//打印用户名
	printf("  %s", getpwuid(f_info.st_uid)->pw_name);

	//打印组名
	printf("  %s", getgrgid(f_info.st_gid)->gr_name);

	//打印大小
	printf(" %5ld", f_info.st_size);

	//打印最后一次次改时间

	time_t t;
	struct tm  *ptm;   //利用到了time  与 localtime
	t = f_info.st_atime;
	ptm = localtime(&t);

	printf("  %d %d %d  ", 1 + ptm->tm_mon, ptm->tm_mday, 1900 + ptm->tm_year);
	printf("%d:%d ", ptm->tm_hour, ptm->tm_min);

	printf("%s \n", filename);

	return;
}
int builtin_command(char *command, char **parameters)
{
	extern struct passwd *pwd;
	if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0)
		exit(0);
	else if (strcmp(command, "about") == 0)
	{
		printf("This is a simulation of shell built by lsy.\n");
		return 1;
	}
	else if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0)
	{
		exit(0);
	}
	else if (strcmp(command, "echo") == 0)
	{
		if (strcmp(parameters[1], "$HOME") == 0){
			printf("$HOME is: %s\n", getenv("HOME"));
			return 1;
		}
		else if (strcmp(parameters[1], "$SHELL") == 0){
			printf("$SHELL is: %s\n", getenv("SHELL"));
			return 1;
		}
		else if (strcmp(parameters[1], "$USER") == 0){
			printf("$USER is: %s\n", getenv("USER"));
			return 1;
		}
	}
	else if (strcmp(command, "cat") == 0)
	{
		int c;
		FILE * fp;
		if ((fp = fopen(parameters[1], "r")) == NULL)
		{
			if (parameters[1] == NULL)
			{
				printf("please choose one file to cat!\n");
			}
			else
			{
				printf("file's name = %s, the file is not existed!\n", parameters[1]);
			}
			return 1;
		}
		printf("-----now,you are using CAT by lsyshell-----\n");
		while ((c = getc(fp)) != EOF)
		{
			putc(c, stdout);
		}
		return 1;
	}
	else if (strcmp(command, "history")==0)
	{
		if (parameters[1] == NULL) //只有history
		{
			int c;
			FILE * fp;
			if ((fp = fopen(".lsy", "r")) == NULL)
			{
				printf("the history is not existed!\n");
				return 1;
			}
			while ((c = getc(fp)) != EOF)
			{
				putc(c, stdout);
			}
			return 1;
		}
		else if (parameters[2] == NULL) //history + 一个数字
		{
			int c;
			int num = atoi(parameters[1]);
			FILE * fp;
			if ((fp = fopen(".lsy", "r")) == NULL)
			{
				printf("the history is not existed!\n");
				return 1;
			}
			char ch;
			int hang = 0;
			//统计文件行数,判断如果num小于总行数，则输出最近的num条记录
			while ((ch = fgetc(fp)) != EOF)
			{
				if (ch == '\n')
					hang++;
			}
			fp = fopen(".lsy", "r");
			if (num >= hang)
			{
				while ((c = getc(fp)) != EOF)
				{
					putc(c, stdout);
				}
			}
			else
			{
				int temp = hang - num;
				int temp2 = temp;
				while ((ch = fgetc(fp)) != EOF)
				{
					if (ch == '\n')
						temp--;
					if (temp == 0)
					{
						temp2++;
						printf("(%2d)   ", temp2);
						while ((c = getc(fp)) != EOF)
						{
							if (c == '\n' && ++temp2<=hang)
							{
								printf("\n(%2d)   ", temp2);
								continue;
							}
							putc(c, stdout);
						}
						return 1;
					}
				}
			}
		}
		else if (parameters[1][0] == '!')
		{
			if (parameters[2][0] != '!')//执行第n条指令
			{
				char ch;
				int hang = 0;
				char zl[200];//代表指令的字符串
				FILE *fp;
				fp = fopen(".lsy", "r");
				int num = atoi(parameters[2]);//第三个参数，代表执行第几条指令
				while ((ch = fgetc(fp)) != EOF)//统计文件总行数
				{
					if (ch == '\n')
						hang++;
				}
				fp = fopen(".lsy", "r");
				int dangqian=1;
				int i=0;
				if (num <= hang){
					while ((ch = fgetc(fp)) != EOF)//找到那一行
					{
						if (dangqian == num)
						{
							zl[i++] = ch;
						}
						if (ch == '\n')
							dangqian++;
					}
					zhilin = zl;
					printf("this zhilin is : %s\n",zhilin);
					//开始执行
					runanother(zhilin);

					return 1;
				}
				else
				{
					printf("error!!!\n");
				}
			}
		}
	}
	//else if (strcmp(command, "pwd") == 0)
	//{
	//	int LMAX = 100;
	//	char *FILE_PATH;
	//	FILE_PATH = (char *)malloc(LMAX);
	//	getcwd(FILE_PATH, LMAX);
	//	printf("pwd now is : %s\n", FILE_PATH);
	//	return 1;
	//}
	else if (strcmp(command, "ls") == 0 || panduanalias(command,"ls",1))
	{
		int t = -1;
		if (strcmp(command, "ls") != 0)
		{
			t = panduanalias(command, "ls", 1) - 1;
			printf("----%d\n", t);
		}
		
		if ((parameters[1] == NULL && t<0) || (a[t].p2[0]=='\0'))
		{
			int LMAX = 100;
			char *FILE_PATH;
			FILE_PATH = (char *)malloc(LMAX);
			getcwd(FILE_PATH, LMAX);
			printf("------now dir is: %s------\n", FILE_PATH);
			DIR *dirp;
			dirp = opendir(FILE_PATH); //打开一个目录的操作
			struct dirent *pdirent;
			chdir(FILE_PATH);

			while ((pdirent = readdir(dirp)) != NULL)
			{
				if (pdirent->d_name[0] == '.')
					continue;
				printf("%s \t", pdirent->d_name);//可以实现对任何目录名字的遍历
			}
			printf("\n");
			closedir(dirp);//关闭打开的目录
		}
		else if ((a[t].p2[0] == '-' && a[t].p2[1] == 'l') || strcmp(parameters[1], "-l") == 0)
		{
			int LMAX = 100;
			char *FILE_PATH;
			FILE_PATH = (char *)malloc(LMAX);
			getcwd(FILE_PATH, LMAX);
			printf("------now dir is: %s------\n", FILE_PATH);
			DIR *dirp;
			dirp = opendir(FILE_PATH); //打开一个目录的操作
			struct dirent *pdirent;
			chdir(FILE_PATH);
			int cc=1;
			while ((pdirent = readdir(dirp)) != NULL)
			{
				if (pdirent->d_name[0] == '.')
					continue;
				printf("(%2d)  ", cc++);
				show_file_allinfo(pdirent->d_name);  //可以实现对当前目录下文件的属性遍历
			}
			printf("\n");
			closedir(dirp);//关闭打开的目录
		}
		else if (strcmp(parameters[1], "-a") == 0)
		{
			int LMAX = 100;
			char *FILE_PATH;
			FILE_PATH = (char *)malloc(LMAX);
			getcwd(FILE_PATH, LMAX);
			printf("------now dir is: %s------\n", FILE_PATH);
			DIR *dirp;
			dirp = opendir(FILE_PATH); //打开一个目录的操作
			struct dirent *pdirent;
			chdir(FILE_PATH);
			int cc = 1;
			while ((pdirent = readdir(dirp)) != NULL)
			{
				printf("(%2d)  ", cc++);
				show_file_allinfo(pdirent->d_name);  //可以实现对当前目录下文件的属性遍历
			}
			printf("\n");
			closedir(dirp);//关闭打开的目录
		}
		return 1;
	}
	else if (strcmp(command, "cd") == 0)
	{
		char *cd_path = NULL;

		if (parameters[1] == NULL)
			//如果输入cd自动补位cd..
		{
			parameters[1] = malloc(3 * sizeof(char));
			parameters[1][0] = '.';
			parameters[1][1] = '.';
			parameters[1][2] = '\0';
		}
		if (parameters[1][0] == '~')
			//如果第一个参数为~
		{
			cd_path = malloc(strlen(pwd->pw_dir) + strlen(parameters[1]));
			//'~' makes length 1 more,but instead of '\0'
			if (cd_path == NULL)//分配失败
			{
				printf("cd:malloc led.\n");
			}
			strcpy(cd_path, pwd->pw_dir);
			strncpy(cd_path + strlen(pwd->pw_dir), parameters[1] + 1, strlen(parameters[1]));
			//printf("path with ~:\n%s\n",cd_path);
		}
		else
		{
			cd_path = malloc(strlen(parameters[1] + 1));
			if (cd_path == NULL)
			{
				printf("cd:malloc failed.\n");
			}
			strcpy(cd_path, parameters[1]);
		}
		if (chdir(cd_path) != 0) //如果没有定位到目标文件夹
			printf("-lsyshell: cd: %s: %s \n", cd_path, strerror(errno));
		free(cd_path);
	}
	else if (strcmp(command, "alias") == 0)
	{
		if (parameters[1] == NULL)
		{
			printalias();
			return 1;
		}
		else 
		{
			char x[500];//dir
			char y[500];//ls -l
			memset(x, 0, sizeof(x));
			memset(y, 0, sizeof(y));
			char *temp = parameters[1];
			/*strcat(temp, " ");*/
			if(parameters[2] !=NULL)
				strcat(temp, parameters[2]);
			if (parameters[3] != NULL)
				strcat(temp, parameters[3]);
			printf("%s\n", temp);
			int i = 0;
			while (temp[i] != '=')
			{
				x[i] = temp[i];
				i++;
			}
			i = i + 2;
			int j = 0;
			while (temp[i] != '\'')
			{
				if (temp[i] == '-')
				{
					y[j] = ' ';
					j++;
				}
				y[j] = temp[i];
				printf("y=%c   ,temp=%c\n",y[j],temp[i]);	
				i++;
				j++;
			}
			strcpy(a[numofalias].name, x);
			strcpy(a[numofalias].refer, y);
			i = 0;
			//分割
			
			while (1)
			{
				int k = 0;
				while (y[i] != ' ' && y[i] != '\0')
				{
					a[numofalias].p1[k] = y[i];
					k++;
					i++;
				}
				i++;
				k = 0;
				while (y[i] != ' ' && y[i] != '\0')
				{
					a[numofalias].p2[k] = y[i];
					k++;
					i++;
				}
				i++;
				k = 0;
				while (y[i] != ' ' && y[i] != '\0')
				{
					a[numofalias].p3[k] = y[i];
					k++;
					i++;
				}
				i++;
				k = 0;
				break;
			}
			/*a[numofalias].name = x;
			a[numofalias].refer = y;*/
			printf("%d new alias is: %s----->'%s'\n", numofalias, a[numofalias].name, a[numofalias].refer);
			printf("%s %s %s\n", a[numofalias].p1, a[numofalias].p2, a[numofalias].p3);
			numofalias++;
		}
	}
	else if (strcmp(command, "unalias") == 0) //删除 某一alias
	{
		if (parameters[0] == NULL)
		{
			printf("error !");
		}
		else
		{
			int i = 0;
			for (i = 0; i <= 400; i++)
			{
				if (strcmp(a[i].name,parameters[1])==0)
				{
					a[i].name[0] ='\0';
					a[i].p1[0] = '\0';
					a[i].p2[0] = '\0';
					a[i].p3[0] ='\0';
					a[i].refer[0] = '\0';

				}
			}
		}
		return 1;
	}
	return 0;
}
void runanother(char *command)
{	
	char *newcommand = NULL;
	char **newparameters;
	int newParaNum;
	char newprompt[MAX_PROMPT];
	newparameters = malloc(sizeof(char *)*(MAXARG + 2));
	newbuffer = malloc(sizeof(char)* MAXLINE);
	type_prompt(newprompt);
	//printf("newcommand= %s\n newapra1= %s\n", newcommand, newparameters[1]);
	read_command(&newcommand, newparameters, newprompt, command);
	//printf("newcommand= %s\n newapra1= %s\n", newcommand, newparameters[1]);
	builtin_command(newcommand, newparameters);
	return;
}
void printalias()
{
	int i = 0;
	for (i = 0;i<=400; i++)
	{
		if (a[i].name[0] != '\0')
		{
			printf("alias  %s='%s'\n", a[i].name, a[i].refer);
		}
	}
}
int panduanalias(char* command,char* ls,int p){
	int i = 0;
	for (i = 0; i <= 400; i++)
	{
		if (a[i].name[0] != '\0')
		{
			if (strcmp(a[i].name, command) == 0)
			{
				if (p == 1)
				{
					if (strcmp(a[i].p1, ls) == 0)
						return i+1;
				}
				if (p == 2)
				{
					if (strcmp(a[i].p2, ls) == 0)
						return i+1;
				}
				if (p == 3)
				{
					if (strcmp(a[i].p3, ls) == 0)
						return i+1;
				}
			}
		}
	}
	return 0;
}