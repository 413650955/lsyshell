#include "lsyshell.h"
//#define DEBUG
//返回值，参数的个数
//0 表示只有命令没有参数
//-1表示输入有误
int read_command(char **command, char **parameters, char *prompt,char *zl)
{
	//memset(parameters, 0, 200);
	char* Res_fgets;
	if (zl[0] == '\0')
	{
		printf("%s", prompt);
		Res_fgets = fgets(buffer, MAXLINE, stdin);
	}
	else{
		Res_fgets = zl;
		//printf("Res_fgets=%s", Res_fgets);
	}
	char* temp = Res_fgets;
	if (Res_fgets == NULL)
	{
		printf("\n");
		exit(0);
	}
	char *p1 = temp;
	while (*p1 != '\n')
	{
		p1++;
	}
	*p1 = ' ';
	//写入文件.bash_history
	time_t timep;
	time(&timep);
	FILE *fp;
	if ((fp = fopen(".lsy", "a+")) == NULL)
	{
		printf(".lsy is not existed,and now lsyshell has created it!\n");
		fp = fopen(".lsy", "a+");
		fprintf(fp, "%s\n", temp);
		fclose(fp);
	}
	else{
		fp = fopen(".lsy", "a+");
		//fprintf(fp, "%s\n", temp, asctime(gmtime(&timep)));
		fprintf(fp, "%s\n", temp);
		fclose(fp);
	}
    
	if(buffer[0] == '\0' && zl[0]=='\0')
        return -1;
    char *pStart,*pEnd;
    int count = 0;
    int isFinished = 0;
	pStart = pEnd = buffer;
    while(isFinished == 0)
    {
        while((*pEnd == ' ' && *pStart == ' ') || (*pEnd == '\t' && *pStart == '\t'))
        {
			pStart++;
            pEnd++;
        }

		if (*pEnd == '\0' || *pEnd == '\n')
		{
			if (count == 0)
				return -1;
			break;
		}

		while (*pEnd != ' ' && *pEnd != '\0' && *pEnd != '\n')//每个字段的长度
		{
#ifdef DEBUG
			printf("pEnd = %s", pEnd);
#endif
			pEnd++;
		}

        if(count == 0)
        {
            char *p = pEnd;
            *command = pStart;
			
			while(p!=pStart && *p !='/')
                p--;
			if(*p == '/')
				p++;
            //else //p==pStart
			parameters[0] = p;

			count += 2;
#ifdef DEBUG
			printf("parameters = %s", parameters[0]);
			printf("pStart=%s", pStart);
			printf("count=%d", count);
            printf("\ncommand:%s\n",*command);
#endif
        }
        else if(count <= MAXARG)
        {
#ifdef DEBUG
			printf("pStart=%s", pStart);
			printf("count<=MAXARG,count=%d\n", count);
#endif
            parameters[count-1] = pStart;
#ifdef DEBUG
			printf("parameters[%d]= %s",count-1, parameters[count-1]);
#endif

            count++;
        }
        else
        {
            break;
        }

        if(*pEnd == '\0' || *pEnd == '\n')
        {
            *pEnd = '\0';
            isFinished = 1;
        }
        else
        {
            *pEnd = '\0';
#ifdef DEBUG			
			printf("parameters[%d]= %s\n", count - 2, parameters[count - 2]);
#endif       
			pEnd++;
			pStart = pEnd;
        }
    }
    parameters[count-1] = NULL;

#ifdef DEBUG
    printf("输入的信息为:\n");
    printf("pathname:[%s]\ncommand:[%s]\nparameters:\n",*command,parameters[0]);
    int i;
    for(i=0;i<count-1;i++)
        printf("[%s]\n",parameters[i]);
#endif
    return count;
}
