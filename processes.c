#include <stdio.h>
#include <stdlib.h>

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
};

void roundRobin(struct fileInfo fileData, struct procInfo *procData);

struct fileInfo readFile(FILE* ifp);
struct procInfo readData(FILE* ifp);
void readMore(FILE* ifp);

int main()
{
    FILE* ifp = fopen("/Users/trentfreeman/Desktop/asn1-sampleio_2/set1_process.in", "r");
    int i;

    struct fileInfo fileData;

    fileData = readFile(ifp);
   // printf("%d %d %s %d\n", fileData.proCount, fileData.runFor, fileData.type, fileData.quantum);

    struct procInfo procData[fileData.proCount];

    for(i = 0; i < fileData.proCount; i++)
    {
        procData[i] = readData(ifp);
        //printf("Name = %s\tArrival at %d\tBurst is %d\n", procData[i].name, procData[i].arrival, procData[i].burst);
    }

    printf("%d processes\n", fileData.proCount);
    if(strcmp(fileData.type, "rr") == 0)
    {
        printf("Using Round-Robin\n");
        printf("Quantum %d\n\n", fileData.quantum);
        roundRobin(fileData, procData);
    }
    else if(strcmp(fileData.type, "fcfs") == 0)
    {
        printf("Using First Come First Serve\n\n");
        //fcfs();
    }
    else
    {
        printf("Using Shortest Job First\n\n");
        //sjf();
    }
}

void roundRobin(struct fileInfo fileData, struct procInfo *procData)
{
    int i, j;
    for(i = 0; i < fileData.runFor; i++)
    {
        for(j = 0; j < fileData.proCount; j++)
        {
            if(procData[j].arrival == i)
                printf("Time %d: %s arrived\n", i, procData[j].name);
        }
    }
}

struct fileInfo readFile(FILE* ifp)
{

    struct fileInfo tempData;
    char buff[255], c;

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
    char buff[255], c;

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
