#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaphore.cpp"
using namespace std;

enum{UandV};

void critSec(SEMAPHORE &, bool *);
void parentProc(int killArray[]);

int main() {

	int shmid = shmget(IPC_PRIVATE, sizeof(bool), PERMS);
	bool *shmBUF = (bool *)shmat(shmid, 0, SHM_RND);
	*shmBUF=true;

  // PID for the fork
  pid_t childPID;

  SEMAPHORE sem(1);

  sem.V(UandV);
  sem.V(UandV);

	int killArray[4];

	for(int i=0;i<4;i++)
	{
		childPID = fork();
		if(childPID)
		{
			killArray[i]=childPID;
		}
		else
		{
			while(true)
			{
				critSec(sem,shmBUF);
			}
		}
	}

	parentProc(killArray);

  return 0;
}

void proc(int uov)
{
	int temp;
	while(true)
	{
		temp=rand();
		if((temp%uov==0)||temp<100)
		{
			break;
			break;
		}
	}
}

void critSec(SEMAPHORE &sem, bool *shmBUF)
{
	int U=827395909;
	int V=962094883;
  sem.P(UandV);
	bool u=*shmBUF;
  if(u)
  {
    *shmBUF=false;
		proc(U);
    *shmBUF=true;
  }
  else
  {
		proc(V);
  }
  sem.V(UandV);
}

void parentProc(int killArray[])
{
	string end;
	while(true)
	{
		cout<<"enter !wq to exit"<<endl;
		getline(cin,end);
		if(end=="!wq") //termination command
		{
			cout<<"exiting..."<<endl;
			for(int i=0;i<4;i++)
			{
				kill(killArray[i],SIGTERM);
			}
			exit(0);  //process terminates, close out program
		}
	}
}
