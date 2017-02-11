#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct fileInfo
{
    int proCount;
    int runFor;
    char type[5];
    int quantum;
};

struct procInfo
{
    char name[50];
    int arrival;
    int burst;
    int selected;
    int done;
};

void roundRobin(struct fileInfo fileData, struct procInfo *procData);
void fcfs(struct fileInfo fileData, struct procInfo *procData);
void sjf(struct fileInfo fileData, struct procInfo *procData);
int min(int x, int y);

struct fileInfo readFile(FILE* ifp);
struct procInfo readData(FILE* ifp);
void readMore(FILE* ifp);

int main()
{
    FILE* ifp = fopen("processes.in", "r");
    int i;

    struct fileInfo fileData;

    fileData = readFile(ifp);

    struct procInfo procData[fileData.proCount];

    for(i = 0; i < fileData.proCount; i++)
    {
        procData[i] = readData(ifp);
    }

    if(strcmp(fileData.type, "rr") == 0)
    {
        roundRobin(fileData, procData);
    }
    else if(strcmp(fileData.type, "fcfs") == 0)
    {
        fcfs(fileData, procData);
    }
    else if(strcmp(fileData.type, "sjf") == 0)
    {
        sjf(fileData, procData);
    }
}

void roundRobin(struct fileInfo fileData, struct procInfo *procData)
{
    FILE *fptr;
    fptr= fopen("/Users/trentfreeman/Desktop/processes.out","w");
    fprintf(fptr, "%d processes\n", fileData.proCount);
    fprintf(fptr,"Using Round-Robin\n");
    fprintf(fptr,"Quantum %d\n\n", fileData.quantum);

    int i, j, k;
    int tQuantum = 0, newArrived = 0;

    struct procInfo procQueue[fileData.proCount+1];
    struct procInfo activeP;
    struct procInfo empty;

    strcpy(empty.name, "empty");
    empty.arrival = -1;
    empty.burst = -1;
    empty.selected = -1;
    empty.done = -1;

    // initialize the queue
    for(i = 0; i < fileData.proCount+1; i++)
        procQueue[i] = empty;

    for(i = 0; i < fileData.runFor; i++)
    {
        newArrived = 0;

        // check for arrival
        for(j = 0; j < fileData.proCount; j++)
        {
            if(procData[j].arrival == i)
            {
                printf("Time %d: %s arrived\n", i, procData[j].name);

                // once arrived, at to queue
                for(k = 0; k <= fileData.proCount; k++)
                {
                    if(strcmp(procQueue[k].name, empty.name) == 0)
                    {
                        procQueue[k] = procData[j];
                        k = fileData.proCount+1;
                        newArrived = 1;
                    }
                }
            }
        }

        // if there is no active process, IDLE
        if(strcmp(procQueue[0].name, empty.name) == 0)
        {
            printf("Time %d: IDLE\n", i);
            break;
        }

        // quantum 0 :
        //   select process
        //   decrease burst of process
        //   reset tQuantum
        //   decrease tQuantum
        if(tQuantum == 0)
        {
            // set the active process
            activeP = procQueue[0];

            printf("Time %d: %s selected (burst %d)\n", i, procQueue[0].name, procQueue[0].burst);

            for(j = 0; j < fileData.proCount; j++)
            {
                printf("Queue[%d]: %s\n", j, procQueue[j].name);
            }

            // rotate the queue
            for(j = 0; j < fileData.proCount-1; j++)
            {
                procQueue[j] = procQueue[j+1];
                //printf("%s is now %s\n", procQueue[j].name, procQueue[j+1].name);
            }

            if(activeP.burst < fileData.quantum)
                tQuantum = activeP.burst;
            else
                tQuantum = fileData.quantum;

            procQueue[0].burst--;

            activeP.burst--;
            tQuantum--;

            // if the activeProcess's burst is positive, add it to the queue
            if(activeP.burst > 0)
            {
                for(j = 0; j < fileData.proCount; j++)
                    if(strcmp(procQueue[j].name, empty.name) == 0)
                    {
                        procQueue[j] = activeP;
                        j = fileData.proCount;
                    }
            }
        }

        else
        {
           // printf("Time %d: %d\n", i, tQuantum);
            procQueue[0].burst--;
            activeP.burst--;
            tQuantum--;
        }


    }

}

void sjf(struct fileInfo fileData, struct procInfo *procData)
{
	FILE *fptr;
	fptr= fopen("processes.out","w");
	fprintf(fptr, "%d processes\n", fileData.proCount);
	fprintf(fptr,"Using Shortest Job First (Pre)\n\n");
	int x,y,z,p;
	int q=100;
	int w=0;
	z=1000;
	p=0;
	int end[fileData.proCount];
	int burst[fileData.proCount];
	for(y=0;y<fileData.proCount;y++)
		burst[y]=procData[y].burst;
	for(x=0;x<fileData.runFor;x++)
	{
		for(y=0;y<fileData.proCount;y++)
		{
			if(procData[y].arrival<=x&&procData[y].burst>0)
			{
				z=min(z,procData[y].burst);
				if(z==procData[y].burst)
					p=y;
			}else{
				w++;
			}
			if(procData[y].arrival==x)
				fprintf(fptr,"Time %d: %s arrived\n", x, procData[y].name);
			
		}
		z=1000;
		if(w==fileData.proCount)
			fprintf(fptr,"Time %d: IDLE\n",x);
		w=0;
		if(p!=q){
			fprintf(fptr,"Time %d: %s selected (burst %d)\n",x, procData[p].name, procData[p].burst);
		}
		//printf("Time %d: %s arrived\n", x, procData[p].name);
		q=p;
		procData[p].burst--;
		if(procData[p].burst==0){
			fprintf(fptr,"Time %d: %s finished\n", x+1, procData[p].name);
			end[p]=x+1;
		}
	}
	fprintf(fptr,"Finished at time %d\n\n", x);
	
	for(y=0;y<fileData.proCount;y++){
		z=end[y]-procData[y].arrival;
		p=z-burst[y];
		fprintf(fptr,"%s wait %d turnaround %d\n",procData[y].name, p, z);
	}
	fclose(fptr);
	
	
}
int min(int x, int y){
	return y ^ ((x ^ y) & -(x < y));
}

struct fileInfo readFile(FILE* ifp)
{

    struct fileInfo tempData;
    char buff[255];

    fscanf(ifp, "%s", buff);
    fscanf(ifp, "%d", &tempData.proCount);
    readMore(ifp);

    fscanf(ifp, "%s", buff);
    fscanf(ifp, "%d", &tempData.runFor);
    readMore(ifp);

    fscanf(ifp, "%s", buff);
    fscanf(ifp, "%s", tempData.type);
    readMore(ifp);

    if(strcmp(tempData.type, "rr") == 0)
    {
        fscanf(ifp, "%s", buff);
        fscanf(ifp, "%d", &tempData.quantum);
        readMore(ifp);
    }
    else
        readMore(ifp);

    return tempData;
}

struct procInfo readData(FILE* ifp)
{
    struct procInfo tempData;
    char buff[255];

    fscanf(ifp, "%s", buff);
    fscanf(ifp, "%s", buff);
    fscanf(ifp, "%s", tempData.name);

    fscanf(ifp, "%s", buff);
    fscanf(ifp, "%d", &tempData.arrival);

    fscanf(ifp, "%s", buff);
    fscanf(ifp, "%d", &tempData.burst);

    return tempData;

}

void readMore(FILE* ifp)
{
    char c, newline = 10;
    fscanf(ifp, "%c", &c);
    while(c != newline)
        fscanf(ifp, "%c", &c);
}
