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
    int wait;
    int turnAround;
};

void roundRobin(struct fileInfo fileData, struct procInfo *procData);
void fcfs(struct fileInfo fileData, struct procInfo *procData);

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
        procData[i] = readData(ifp);

    if(strcmp(fileData.type, "rr") == 0)
    {
        roundRobin(fileData, procData);
    }
    else if(strcmp(fileData.type, "fcfs") == 0)
    {
        fcfs(fileData, procData);
    }
    else
    {
        //sjf();
    }
}

void roundRobin(struct fileInfo fileData, struct procInfo *procData)
{
    FILE *fptr;
    fptr = fopen("processes.out","w");
    fprintf(fptr, "%d processes\n", fileData.proCount);
    fprintf(fptr,"Using Round-Robin\n");
    fprintf(fptr,"Quantum %d\n\n", fileData.quantum);

    int i, j, k;
    int tQuantum = 0, newArrived = 0, rotated = 0;
    int numFinished = 0;

    struct procInfo procQueue[fileData.proCount+1];
    struct procInfo activeP, lastActiveP;
    struct procInfo empty;

    strcpy(empty.name, "empty");
    strcpy(lastActiveP.name, empty.name);
    strcpy(activeP.name, empty.name);
    lastActiveP.burst = -1;
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
        rotated = 0;

        // check for arrival
        for(j = 0; j < fileData.proCount; j++)
        {
            if(procData[j].arrival == i)
            {
                fprintf(fptr,"Time %d: %s arrived\n", i, procData[j].name);

                // once arrived, at to queue
                for(k = 0; k <= fileData.proCount; k++)
                {
                    if(strcmp(procQueue[k].name, empty.name) == 0)
                    {
                        strcmp(procQueue[k].name, procData[j].name);
                        procQueue[k].burst = procData[j].burst;
                        procQueue[k].arrival = procData[j].arrival;
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
            fprintf(fptr,"Time %d: IDLE\n", i);
            break;
        }

        // quantum 0 :
        //   select process
        //   decrease burst of process
        //   reset tQuantum
        //   decrease tQuantum
        if(tQuantum == 0)
        {
            // if the last process exists
            if(strcmp(activeP.name, empty.name) != 0)
            {
                //dequeue
                for(j = 0; j < fileData.proCount; j++)
                {
                    procQueue[j] = procQueue[j+1];
                }

                // enqueue
                for(j = 0; j < fileData.proCount; j++)
                {
                    if(strcmp(procQueue[j].name, empty.name) == 0 && activeP.burst != 0)
                    {
                        procQueue[j] = activeP;
                        j = fileData.proCount;
                    }
                }
            }

            // select the new queue
            strcpy(activeP.name, procQueue[0].name);
            activeP.burst = procQueue[0].burst;

            if(strcmp(activeP.name, empty.name) == 0)
            {
                fprintf(fptr,"Time %d: IDLE\n", i);
                break;
            }

            fprintf(fptr,"Time %d: %s selected (burst %d)\n", i, activeP.name, activeP.burst);

            if(activeP.burst < fileData.quantum)
                tQuantum = activeP.burst;
            else
                tQuantum = fileData.quantum;

            activeP.burst--;
            tQuantum--;

            if(activeP.burst == 0)
            {
                fprintf(fptr, "Time %d: %s finished\n", (i+1), activeP.name);
                for(j = 0; j < fileData.proCount; j++)
                {
                    if(strcmp(activeP.name, procData[j].name) == 0)
                    {
                        procData[j].done = i+1;
                        procData[j].turnAround = procData[j].done - procData[j].arrival;
                        procData[j].wait = procData[j].turnAround - procData[j].burst;

                    }

                }
                numFinished++;
            }
        }
        else
        {
            activeP.burst--;
            tQuantum--;
        }

    }

    if(numFinished == fileData.proCount)
    {
        fprintf(fptr, "Finished at time %d\n\n", (i+1));
    }

    for(i = 0; i < fileData.proCount; i++)
    {
        fprintf(fptr, "%s wait %d turnaround %d\n", procData[i].name, procData[i].wait, procData[i].turnAround);
    }

}

void fcfs(struct fileInfo fileData, struct procInfo *procData)
{
    FILE *fptr;
    fptr = fopen("processes.out", "w");
    fprintf(fptr, "%d processes\n", fileData.proCount);
    fprintf(fptr, "Using First Come First Served\n\n");

    int i, j;

    // Check if valid first to avoid errors?
    // Calculate selection times.
    procData[0].selected = procData[0].arrival;
    for(i = 1; i < fileData.proCount; i++)
        procData[i].selected = procData[i-1].selected + procData[i-1].burst;

    // Calculate finish times.
    for(i = 0; i < fileData.proCount; i++)
        procData[i].done = procData[i].selected + procData[i].burst;

    // Print table
    for(i = 0; i <= fileData.runFor; i++)
    {
        for(j = 0; j < fileData.proCount; j++)
        {
            if(procData[j].arrival == i)
                fprintf(fptr, "Time %d: %s arrived\n", i, procData[j].name);
            if(procData[j].selected == i)
                fprintf(fptr, "Time %d: %s selected (burst %d)\n", i, procData[j].name, procData[j].burst);
            if(procData[j].done == i)
                fprintf(fptr, "Time %d: %s finished\n", i, procData[j].name);
        }
    }

    fprintf(fptr, "Finished at time %d\n\n", fileData.runFor);
    for(i = 0; i < fileData.proCount; i++)
        fprintf(fptr, "%s wait %d turnaround %d\n", procData[i].name, procData[i].selected - procData[i].arrival,
                                                    procData[i].done - procData[i].arrival);
    fclose(fptr);
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
