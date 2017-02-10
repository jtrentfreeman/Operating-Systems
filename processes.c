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

struct fileInfo readFile(FILE* ifp);
struct procInfo readData(FILE* ifp);
void readMore(FILE* ifp);

int main()
{
    FILE* ifp = fopen("/Users/trentfreeman/Desktop/asn1-sampleio_2/set2_process.in", "r");
    int i;

    struct fileInfo fileData;

    fileData = readFile(ifp);

    struct procInfo procData[fileData.proCount];

    for(i = 0; i < fileData.proCount; i++)
    {
        procData[i] = readData(ifp);
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
        fcfs(fileData, procData);
    }
    else
    {
        printf("Using Shortest Job First\n\n");
        //sjf();
    }
}

void roundRobin(struct fileInfo fileData, struct procInfo *procData)
{
    int i, j, k, tQuantum = 0;

    // lazy queue
    struct procInfo procQ[fileData.proCount];

    // keep track of which process is selected
    // may end up tracking that in the struct instead
    struct procInfo selected;

    // set a null struct
    struct procInfo empty;
    strcpy(empty.name, "empty");
    empty.arrival = -1;
    empty.burst = -1;

    // initialize the queue
    for(i = 0; i < fileData.proCount; i++)
        procQ[i] = empty;

    // Run each time unit
    for(i = 0; i < fileData.runFor; i++)
    {

        // for each process
        for(j = 0; j < fileData.proCount; j++)
        {

            // check if the process has arrived
            if(procData[j].arrival == i)
            {
                printf("Time %d: %s arrived\n", i, procData[j].name);

                // if it's arrived, insert it into the queue
                for(k = 0; k < fileData.proCount; k++)
                {

                    // insert it into the first empty spot of the queue
                    if(strcmp(procQ[k].name, empty.name) == 0)
                    {
                        strcpy(procQ[k].name, procData[j].name);
                        break;
                    }
                }
            }
        }

/** I WILL NEED TO CHANGE THIS SO ALL QUANTUMS ARE ABLE TO BE USED **/
/** While loop >1 ? **/

        // decrease selected process burst by 1
        if(tQuantum >= 2)
        {
            // find the selected processes and decrease its burst, each time unit
            for(j = 0; j < fileData.proCount; j++)
            {
                if(procData[j].selected == 1)
                    procData[j].burst--;
            }

            // decrease the quantum
            tQuantum--;
        }

        // decrease selected process burst by 1
        // dequeue and enqueue the process that was selected
        else if(tQuantum == 1)
        {

            // find the selected process and decrease its burst
            for(j = 0; j < fileData.proCount; j++)
            {
                if(procData[j].selected == 1)
                    procData[j].burst--;
            }

            // reset the quantum
            tQuantum = 0;

            // dequeue
            for(j = 0; j < fileData.proCount-1; j++)
            {
                procQ[j] = procQ[j+1];
            }

            // enqueue
            for(j = 0; j < fileData.proCount-1; j++)
            {
                // if it was just selected and still has enough burst, requeue it
                if(procData[j].selected == 1 && procData[j].burst > 0)
                {
                    // dont think this works, need to set the queue in a better way
                    procQ[fileData.proCount-1] = procData[j];

                    // unselect the process
                    procData[j].selected = 0;
                }
            }
        }

        // select the process to run
        else if(tQuantum == 0)
        {
            for(j = 0; j < fileData.proCount; j++)
            {
                if(strcmp(procData[j].name, procQ[0].name) == 0)
                {
                    procData[j].selected = 1;
                    printf("Time %d: %s selected (burst %d)\n", i, procData[j].name, procData[j].burst);
                }
            }

            // reset the quantum
            tQuantum = fileData.quantum;
        }
    }
}

void fcfs(struct fileInfo fileData, struct procInfo *procData)
{
    // Following is basically pseudo code
    /*for(each process in procInfo struct)
    {
       we already have arrival and burst times
       calculate selection time
            1st is first process arrival time (not necessarily 0)
            subsequent will be (previous selection time) + (previous process burst time)

       calculate finish time (if finish time > runFor time, print Px doesn't finish)
            always (process selection time) + (process burst time)
    }

     for(use runFor to determine amount of loops (checking each processes info every time unit))
        for(check each process)
            if(current time unit == arrival, selection, or finish time)
                print out corresponding line

     print overall finished time which is equal to runFor
     print wait and turnaround times for each process
        wait time = selection time - arrival time
        turnaround time = 
    */
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
