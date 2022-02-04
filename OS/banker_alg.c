/*------------------------------------------------------------------------------
*
* @author: Mehmet Kaan Erol
*
------------------------------------------------------------------------------*/
#include <stdio.h>

const unsigned char N = 5; // number of processes
const unsigned char M = 3; // number of resources

char createFinishMatrix(unsigned char[N][M], unsigned char[N][M], unsigned char[N][M], unsigned char[M]);
int main()
{
    // initialization
    unsigned char max[5][3]       = {{8,5,3},{3,2,8},{9,0,2},{2,2,2},{4,3,3}};
    unsigned char allocated[5][3] = {{0,1,0},{2,0,0},{3,0,2},{2,1,1},{0,0,2}};
    unsigned char available[3]    = {3,3,2};
    unsigned char requiredResource[5][3];

    // for each process
    unsigned char finish[N][M];

    // create finish state matrix of processes 
    char check = createFinishMatrix(finish, max, allocated, available);
    if (check == 0)
    {
        printf("Minimum gereksinim:");
        for(int processIndex=0; processIndex<N; processIndex++)
        {
            printf("\nProcess %d: ", (processIndex+1));
            for (int resourceIndex=0;resourceIndex<M;resourceIndex++)
            {
                if (finish[processIndex][resourceIndex] != 0)
                {
                    requiredResource[processIndex][resourceIndex] = (max[processIndex][resourceIndex] - allocated[processIndex][resourceIndex]);                
                }
                else
                {
                    requiredResource[processIndex][resourceIndex] = 0;
                }
                printf("R%d -> %d", (resourceIndex+1), requiredResource[processIndex][resourceIndex]);
                if (resourceIndex != (M-1)) printf(" | ");    
            }
        }
    
        printf("\n\n");

        // add minimum resource requirements than create new finish state matrix
        // initial available state
        available[0] = 3;
        available[1] = 3;
        available[2] = 2;

        // add required resources
        for(int processIndex=0; processIndex<N; processIndex++)
        {
            for (int resourceIndex=0;resourceIndex<M;resourceIndex++)
            {
                allocated[processIndex][resourceIndex] += requiredResource[processIndex][resourceIndex];
            }
        }

        printf(" ** Minimum gereksinimler eklendi. Son durumda: \n\n");

        // create finish matrix
        createFinishMatrix(finish, max, allocated, available);
    }

    printf("\n\n");
    return 0;
}

char createFinishMatrix(unsigned char finish[N][M], unsigned char max[N][M], unsigned char allocated[N][M], unsigned char available[M])
{
    unsigned char ans[N];
    int finishedIndex = 0, flag = 0;

    // initialize finish array
    // for each process
    for (int i=0;i<N;i++)
    {
        // for each resource
        for (int k=0;k<M;k++)
        {
            finish[i][k] = 1;
        }
    }

    for (int k=0; k<N; k++) 
    {
        for (int processIndex=0; processIndex<N; processIndex++) 
        {
            if (finish[processIndex][0] != 0) 
            {
                flag = 0;
                // for each resource
                for (int resourceIndex=0; resourceIndex<M; resourceIndex++) 
                {
                    // needed resource > available resource
                    if ((max[processIndex][resourceIndex] - allocated[processIndex][resourceIndex]) > 
                        available[resourceIndex])
                    {
                        flag = 1;
                        break;
                    }
                }

                if (flag == 0)
                {
                    ans[finishedIndex] = (processIndex+1);
                    finishedIndex++;

                    // for each resource
                    for (int resourceIndex=0; resourceIndex<M; resourceIndex++)
                    {
                        available[resourceIndex] += allocated[processIndex][resourceIndex];
                        finish[processIndex][resourceIndex] = 0;
                    }
                }
            }
        }
    }

    // check system is safe or not
    // check all process state
    for(int i=0; i<N; i++)
    {
        if((finish[i][0] != 0) && (finish[i][1] != 0) && (finish[i][2] != 0))
        {
            printf("Kilitlenme durumu vardır.\n\n");
            return 0;
        }
    }

    printf("Kilitlenme yoktur. Islem sirasi: \n\n");
    for (int i=0; i<N; i++)
    {
        printf("P%d", ans[i]);
        if (i != (N-1)) printf(", ");
    }
    return 1;
}
