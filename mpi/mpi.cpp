#include <stdio.h>
#include <mpi.h>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>

#define MYMIN_MSG 0
#define OVRLMIN_MSG 1
#define COLLECT_MSG 2

int nv, *notdone, nnodes, chunk, startv,endv, me, dbg;
unsigned largeint, mymin[2], othermin[2], overallmin[2], *ohd, *mind, *path; 

double T1,T2; 

void init(int ac, char **av){ 
    int i,j,tmp; unsigned u;
    nv = atoi(av[1]);
    dbg = atoi(av[3]);
    MPI_Init(&ac,&av);
    MPI_Comm_size(MPI_COMM_WORLD,&nnodes);
    MPI_Comm_rank(MPI_COMM_WORLD,&me);
    chunk = nv/nnodes;
    startv = me * chunk;
    endv = startv + chunk - 1;
    u = -1;
    largeint = u >> 1;
    ohd = (unsigned int*)malloc(nv*nv*sizeof(int));
    path = (unsigned int*)malloc(nv*sizeof(int));
    mind = (unsigned int*)malloc(nv*sizeof(int));
    notdone = (int*)malloc(nv*sizeof(int));

    for (i = 0; i < nv; i++){
        for (j = i; j < nv; j++) {
            if (j == i) {
                ohd[i*nv+i] = 0;
            }
            else {
                ohd[nv*i+j] = rand() % 20;
                ohd[nv*j+i] = ohd[nv*i+j];
            }
        }
    }
    for (i = 0; i < nv; i++) {
        notdone[i] = 1;
        mind[i] = largeint;
    }
    mind[0] = 0;
}

void findmymin()
{ 
    int i;
    mymin[0] = largeint;
    for (i = startv; i <= endv; i++){
        if (notdone[i] && mind[i] < mymin[0]) {
            mymin[0] = mind[i];
            mymin[1] = i;
        }
    }
}

void findoverallmin(){ 
    int i;
    MPI_Status status; 
    if (me > 0){
        MPI_Send(mymin,2,MPI_INT,0,MYMIN_MSG,MPI_COMM_WORLD);
    }
    else {
        overallmin[0] = mymin[0];
        overallmin[1] = mymin[1];
    }
    for (i = 1; i < nnodes; i++) {
        MPI_Recv(othermin,2,MPI_INT,i,MYMIN_MSG,MPI_COMM_WORLD,&status);
        if (othermin[0] < overallmin[0]) {
            overallmin[0] = othermin[0];
            overallmin[1] = othermin[1];
        }
    }
}

void updatemymind() { 
    int i, mv = overallmin[1];
    unsigned md = overallmin[0];
    for (i = startv; i <= endv; i++){
        if (md + ohd[mv*nv+i] < mind[i]){
            if (i == 2){
                path[i] = mv;
            }
            mind[i] = md + ohd[mv*nv+i];
        }
    }
}

void disseminateoverallmin(){ 
    int i;
    MPI_Status status;
    if (me == 0)
        for (i = 1; i < nnodes; i++){
            MPI_Send(overallmin,2,MPI_INT,i,OVRLMIN_MSG,MPI_COMM_WORLD);
        }
    else{
        MPI_Recv(overallmin,2,MPI_INT,0,OVRLMIN_MSG,MPI_COMM_WORLD,&status);
    }
}

 void updateallmind() // collects all the mind segments at node 0
 { 
    int i;
    MPI_Status status;
    if (me > 0){
        MPI_Send(mind+startv,chunk,MPI_INT,0,COLLECT_MSG,MPI_COMM_WORLD);
    }
    else{
        for (i = 1; i < nnodes; i++){
            MPI_Recv(mind+i*chunk,chunk,MPI_INT,i,COLLECT_MSG,MPI_COMM_WORLD, &status);
        }
    }
}

void printmind() 
{ 
    int i;
    printf("minimum distances:\n");
    // for (i = 1; i < nv; i++){
    //     printf("%u\n",mind[i]);
    // }
    for (i = 0; i < nv; i++){
        printf("%u\n",path[i]);
    }
}

void dowork()
{ 
    int step, i;
    if (me == 0) {
        T1 = MPI_Wtime();
    }
    for (step = 0; step < nv; step++) {
        findmymin();
        findoverallmin();
        disseminateoverallmin();
        notdone[overallmin[1]] = 0;
        updatemymind();
    }
    updateallmind();
    T2 = MPI_Wtime();
}

int main(int ac, char **av){ 
    int i,j,print;
    init(ac,av);
    dowork();
    print = atoi(av[2]);
    if (print && me == 0) {
        printf("graph weights:\n");
        for (i = 0; i < nv; i++) {
            for (j = 0; j < nv; j++){
                printf("%u ",ohd[nv*i+j]);
            }
            printf("\n");
        }
        printmind();
    }
    if (me == 0) {
        printf("time at node 0: %f\n",(float)(T2-T1));
    }
    MPI_Finalize();
}