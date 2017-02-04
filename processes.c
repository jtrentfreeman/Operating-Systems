#include <stdio.h>
#include <stdlib.h>

struct procs
{
    char name[255];
    int arrival;
    int burst;
};

void readProcesses(FILE* ifp, int numProc, struct procs *processes);
void readInputs(FILE *ifp, int* numProc, int* timeUnits, char* type, int* quantum, int* qCheck);
void readMore(FILE *ifp);

int main()
{
    // replace with your test file
    // need an output file before submitting
    FILE* ifp = fopen("/Users/trentfreeman/Desktop/processes.in", "r");
    char type[5], buff[255];
    int numProc, timeUnits, quantum = -1, qCheck = 0;

    // Used to read the file and store input
    readInputs(ifp, &numProc, &timeUnits, &type, &quantum, &qCheck);

    // Prints out data
    printf("%d processes\n", numProc);
    if(strcmp(type, "rr") == 0)
        printf("Using Round-Robin\nQuantum %d\n", quantum);
    else if(strcmp(type, "fcfs") == 0)
        printf("Using First Come First Serve\n");
    else if(strcmp(type, "sjf") == 0)
        printf("Using Shortest Job First\n");

    struct procs processes[numProc];
    //readProcesses(ifp, numProc, &processes);

    int i;


    printf("Looping through %d procs\n", numProc);
    
    // doesn't correctly read through the processes yet
    for(i = 0; i < numProc; i++)
    {

        fscanf(ifp, "%s", buff);
        printf("%s", buff);
//        printf("Process #%d:\n", i);
//        printf("\tName: %s\nArrival time: %d\nBurst: %d", processes[i].name, processes[i].arrival, processes[i].burst);

    }

}

void readProcesses(FILE* ifp, int numProc, struct procs *processes)
{
    char buff[255], newline = (int) 10;
    int i;

    printf("Looping through %d procs", numProc);
    for(i = 0; i < numProc; i++)
    {
        fscanf(ifp, "%s", buff);
        printf("%s", buff);
        fscanf(ifp, "%s", buff);
        fscanf(ifp, "%s", processes[i].name);
        fscanf(ifp, "%s", buff);
        fscanf(ifp, "%s", processes[i].arrival);
        fscanf(ifp, "%s", buff);
        fscanf(ifp, "%s", processes[i].burst);
    }

}

void readInputs(FILE* ifp, int* numProc, int* timeUnits, char* type, int* quantum, int* qCheck)
{
    char buff[255];
    char hash = (int) 35;
    int tempI;

    fscanf(ifp, "%s", buff);
    fscanf(ifp, "%d", &tempI);
    *numProc = tempI;

    fscanf(ifp, "%s", buff);
    if(buff[0] == hash)
        readMore(ifp);

    fscanf(ifp, "%s", buff);
    fscanf(ifp, "%d", &tempI);
    *timeUnits = tempI;

    fscanf(ifp, "%s", buff);
    if(buff[0] == hash)
        readMore(ifp);

    fscanf(ifp, "%s", buff);
    fscanf(ifp, "%s", type);

    if(strcmp(type, "rr") == 0)
        qCheck = 1;

    fscanf(ifp, "%s", buff);
        readMore(ifp);

    if(qCheck)
    {
        fscanf(ifp, "%s", buff);
        fscanf(ifp, "%d", &tempI);
        *quantum = tempI;
    }

    fscanf(ifp, "%s", buff);
    if(buff[0] == hash)
        readMore(ifp);

}

void readMore(FILE *ifp)
{

    char c, newline = (int) 10;
    while(c != newline)
        fscanf(ifp, "%c", &c);

    return;
}
