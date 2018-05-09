/*
CECS 326
Project 4: Programming with semaphores and shared memory
Andrew Myer
012939730
*/

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
void parentProc(int killArray[], SEMAPHORE &, int shmid);

int main() {

	//initialize the smeaphore and set to two
	SEMAPHORE sem(1);
	sem.V(UandV);
  sem.V(UandV);

	//set shared variable to a boolean and set to true
	int shmid = shmget(IPC_PRIVATE, sizeof(bool), PERMS);
	bool *shmBUF = (bool *)shmat(shmid, 0, SHM_RND);
	*shmBUF=true;

  // PID for the fork
  pid_t childPID;

	/*array of childPID's to be used when the the parent process kills the
	child processes*/
	int killArray[4];

	//creates four child processes
	/*for(int i=0;i<4;i++)
	{
		childPID = fork();
		if(childPID) //parent process
		{
			killArray[i]=childPID;
		}
		else //child process
		{
			while(true) //child processes can't proceed from here
			{
				critSec(sem,shmBUF);
			}
		}
	}*/
	childPID = fork();
	if(childPID) //parent process
	{
		killArray[0]=childPID;
		childPID = fork();
		if(childPID) //parent process
		{
			killArray[1]=childPID;
			childPID = fork();
			if(childPID) //parent process
			{
				killArray[2]=childPID;
				childPID = fork();
				if(childPID) //parent process
				{
					killArray[3]=childPID;
				}
				else //child process
				{
					while(true) //child processes can't proceed from here
					{
						critSec(sem,shmBUF);
					}
				}
			}
			else //child process
			{
				while(true) //child processes can't proceed from here
				{
					critSec(sem,shmBUF);
				}
			}
		}
		else //child process
		{
			while(true) //child processes can't proceed from here
			{
				critSec(sem,shmBUF);
			}
		}
	}
	else //child process
	{
		while(true) //child processes can't proceed from here
		{
			critSec(sem,shmBUF);
		}
	}


	parentProc(killArray,sem,shmid);

  return 0;
}
/*-------------------------------------------------------------------
creates a random number in a loop until that number is divisible by
100 or a multiple of uov.
param
	int pov: the number for the P processes or the V process
return
	null
--------------------------------------------------------------------*/
void proc(int uov)
{
	int tmp;
	while(true)
	{
		tmp=rand();
		if((tmp%uov==0)||tmp<100)
		{
			break;
			break;
		}
	}
}

/*-------------------------------------------------------------------
Enters a critical section so that no other processes can work on the same
number as the current process. since sem(UandV) is initially set to 2
allows for 2 processes to run concurrently
param
	SEMAPHORE &sem: the semaphore object to be used to order the processes
	bool *shmBUF: the boolean flag to show if the U process is available
return
	null
--------------------------------------------------------------------*/
void critSec(SEMAPHORE &sem, bool *shmBUF)
{
	int U=827395909;
	int V=962094883;

  sem.P(UandV);
	bool u=*shmBUF;
  if(u)//u is available
  {
    *shmBUF=false;	//makes U unavailable for other processes
		proc(U);
    *shmBUF=true;
  }
  else //u is not available, run V process
  {
		proc(V);
  }
  sem.V(UandV);
}
/*-------------------------------------------------------------------
for the parent process to terminate all children processes, clean up
and exit semaphore and shared boolean when the user inputs "!wq"
param
	int killArray[]: array with all the childIDs to terminate
return
	null
--------------------------------------------------------------------*/
void parentProc(int killArray[],SEMAPHORE &sem, int shmid)
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
				kill(killArray[i],SIGTERM); //command to kill child process
			}
			shmctl(shmid, IPC_RMID, NULL);
			sem.remove();
			exit(0);  //process terminates, close out program
		}
	}
}
