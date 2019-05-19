#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<limits.h>
#include<execinfo.h>
#include<signal.h>
#include<exception>
#include<setjmp.h>
#include<unistd.h>
#include<unistd.h>

#define BUFSZ PIPE_BUF

void err_quit(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	FILE* fp;
	int count;
	char buf[BUFSZ];
	char command[150];
	pid_t pr;
	if(argc != 2)
	{
		printf("USAGE: example <process name>\n");
		exit(EXIT_SUCCESS);
	}
	else
		sprintf(command, "ps -C %s|wc -l", argv[1] );

	//while(true)
	{
		usleep(1000000);
		if((fp = popen(command,"r")) == NULL)
			err_quit("popen");

		if( (fgets(buf,BUFSZ,fp))!= NULL )
		{
			count = atoi(buf);
			if((count - 1) == 0)
			{

FORK_GAME_SERVER:
				pid_t pc = fork();
			    switch(pc)
			    {
			    case -1:
			        printf("fork error\n");
			        exit(EXIT_FAILURE);
			    case 0: /*子进程*/
			        {
				        printf("child process create is suc\n");
				        printf("child pid=%d, child parent pid=%d\n", getpid(), getppid());
				        printf("child uid=%d, child gid=%d\n", getuid(), getpid());

				        char* const arg[] = {"gameserver", NULL};//第一个参数为进程显示的名称
						char* filename = "./gameserver";
						char* const envp[] = {"环境变量1","环境变量2",NULL};
						execve(filename, arg, envp);	//会阻塞，然后进程退出

						printf("这里不会打印，也不会执行\n");
					}
			    default:/*父进程*/
					/*不阻塞 调用waitpid(), 且父进程不阻塞*/
					while(1)
					{
						pr = waitpid(pc, NULL, WNOHANG);
						/*若子进程还为退出，则父进程等待1秒*/
						if (pr == 0)
						{
							printf("gameserver has is running\n");
							usleep(1000000);
						}
						else
						{
							printf("gameserver has is exit******\n");
							goto FORK_GAME_SERVER;
						}
					}
			    }

			    printf("这里谁在执行 pid=%d\n", getppid());

			}
			else
				printf("process : %s total is %d\n",argv[1],(count - 1));
		}

		pclose(fp);
	}
	exit(EXIT_SUCCESS);
}

//////注意需要放到进程的同级目录，监控进程可以调试到gameserver进程， 当需要启动gdb MonitorProcess进程的时候，执行 gdb MonitorProcess 进行调试 set args gameserver :设置参数 b main：设置断点 r:执行 或者用 r a b c也可以的
//////