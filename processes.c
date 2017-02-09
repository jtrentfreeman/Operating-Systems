#include <stdio.h>
#include <stdlib.h>

struct procInfo
{
    char name[255];
    int arrival;
    int burst;
    int done;
};

void roundRobin(int proCount, int timeUnit, int quantum, struct procInfo *procs);
void fcfs(int proCount, int timeUnit, struct procInfo *procs);
void sjf();

void readFile(FILE* ifp, int proCount, int timeUnit, int quantum, char* type, struct procInfo *procs);
void readMore(FILE* ifp);

int main()
{
    FILE* ifp = fopen("/Users/trentfreeman/Desktop/processes.in", "r");
    int proCount, timeUnit, quantum;
    char type[5];

    char buff[255];
    fscanf(ifp, "%s", buff);
    fscanf(ifp, "%d", &proCount);
    readMore(ifp);

    struct procInfo procs[proCount];
    readFile(ifp, proCount, &timeUnit, &quantum, type, &procs);

    if(strcmp(type, "rr") == 0)
        roundRobin(proCount, timeUnit, quantum, procs);
    else if(strcmp(type, "fcfs") == 0)
        fcfs(int proCount, int timeUnit, struct procInfo *procs);

}

void roundRobin(int proCount, int timeUnit, int quantum, struct procInfo *procs)
{

}

void fcfs()
{

}

void sjf()
{


}

void readFile(FILE* ifp, int proCount, int timeUnit, int quantum, char* type, struct procInfo *procs)
{

    char buff[255];

    fscanf(ifp, "%s", buff);
    fscanf(ifp, "%d", &timeUnit);
    readMore(ifp);

    fscanf(ifp, "%s", buff);
    fscanf(ifp, "%s", type);
    readMore(ifp);

    printf("%d processes\n", proCount);
    if(strcmp(type, "rr") == 0)
        printf("Using Round Robin\n");
    else if(strcmp(type, "fcfs") == 0)
        printf("Using First Come First Served\n");
    else
        printf("Using Shortest Job First\n");


    if(strcmp(type, "rr") == 0)
    {
        fscanf(ifp, "%s", buff);
        fscanf(ifp, "%d", &quantum);
        printf("Quantum %d\n", quantum);
        readMore(ifp);
    }
    else
        readMore(ifp);

    int i;

    for(i = 0; i < proCount; i++)
    {
        fscanf(ifp, "%s", buff);
        fscanf(ifp, "%s", buff);
        fscanf(ifp, "%s", procs[i].name);
        fscanf(ifp, "%s", buff);
        fscanf(ifp, "%d", &procs[i].arrival);
        fscanf(ifp, "%s", buff);
        fscanf(ifp, "%d", &procs[i].burst);

        printf("\tName is %s\tArrival at %d\tBurst is %d\n", procs[i].name, procs[i].arrival, procs[i].burst);
    }


}

void readMore(FILE* ifp)
{
    char c, newline = 10;
    fscanf(ifp, "%c", &c);
    while(c != newline)
        fscanf(ifp, "%c", &c);
}
