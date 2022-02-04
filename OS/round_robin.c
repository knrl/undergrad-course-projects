/**
	Mehmet Kaan Erol
	   170419512
**/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define N 100
#define MAX_INT_16 65535

typedef unsigned int uint16;
typedef struct 
{
	uint16 burst;
	uint16 priority;
	uint16 repeat;
} Process;

uint16 getMustPriorJob(Process *);
void orderArray(Process *);
int main()
{
	srand(time(NULL));
	uint16 timeQuant = 2;
	Process processArray[N] = { 0 };

	// init processArray
	// I assume that all process arrived at the same time
	for (int i=0;i<100;i++)
	{
		processArray[i].priority 	 = 11 + (uint16) (rand() % 90);
		processArray[i].burst	 	 = 1 + (uint16) (rand() % 10);
		processArray[i].repeat	 	 = 0;
	}

	// order processArray
	orderArray(processArray);

	// schedule processes
	uint16 index = 0, currentPriorityHeadIndex = 0, check = 0, totalTime = 0, processWaitingTime = 0;
	double averageWaitingTime = 0;
	uint16 remainProcessNumber = N;
	printf("\n\nProcess\tBaslama\tBitis\tKalan");
	while (remainProcessNumber != 0)
	{
		while (processArray[index].priority == processArray[currentPriorityHeadIndex].priority)
		{
			if (processArray[index].burst != 0)
			{
				processWaitingTime += totalTime - processArray[index].repeat * timeQuant;
				if (processArray[index].burst < timeQuant)
				{
					printf("P%d\t%d\t%d\t%d\n",(index+1),totalTime,(totalTime+processArray[index].burst),0);
					totalTime += processArray[index].burst;
					processWaitingTime += timeQuant - processArray[index].burst;
					processArray[index].burst = 0;
				}
				else
				{
					printf("P%d\t%d\t%d\t%d\n",(index+1),totalTime,(totalTime+timeQuant),(processArray[index].burst -timeQuant));
					processArray[index].burst -= timeQuant;
					totalTime += timeQuant;
				}
				averageWaitingTime += processWaitingTime;

				if (processArray[index].burst == 0)
				{
					remainProcessNumber--;
					if (remainProcessNumber == 0)
						break;
				}
				check = 1;
			}
			index++;
		}

		if (check)
		{
			index = currentPriorityHeadIndex;
		}
		else
		{
			currentPriorityHeadIndex = index;
		}

		check = 0;
	}
	averageWaitingTime /= totalTime;
	printf("Total Waiting Time: %d\nAverage Waiting Time: %f\n\n",totalTime,averageWaitingTime);

	return 0;
}

void orderArray(Process processArray[N])
{
	uint16 temp = 0, min = 0;
	for (int i=0;i<N;i++)
	{
		min = i;
		for (int k=i+1;k<N;k++)
		{
			if (processArray[min].priority > processArray[k].priority)
			{
				min = k;
			}
		}

		if (min != i)
		{
			temp = processArray[min].priority;
			processArray[min].priority = processArray[i].priority;
			processArray[i].priority = temp;
		}
	}
}