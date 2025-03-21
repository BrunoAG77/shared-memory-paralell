//Bruno Antico Galin 10417318
//Ismael de Sousa Silva 10410870
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

void ClientProcess(int *sharedMem);

int main(int argc, char *argv[]){
     int ShmID;
     int *ShmPTR;
     pid_t pid;
     int status;

     if (argc != 5) {
          printf("Usage: %s <value1> <value2> <value3> <value4>\n", argv[0]);
          exit(1);
     }

     ShmID = shmget(IPC_PRIVATE, 4 * sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }

     printf("Server has received shared memory of four integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if ((long)ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     printf("Server has attached the shared memory...\n");

     ShmPTR[0] = atoi(argv[1]);
     ShmPTR[1] = atoi(argv[2]);
     ShmPTR[2] = atoi(argv[3]);
     ShmPTR[3] = atoi(argv[4]);
     printf("Server has filled %d %d %d %d in shared memory...\n", ShmPTR[0], ShmPTR[1], ShmPTR[2], ShmPTR[3]);

     printf("Server prints initial values: %d %d %d %d\n", ShmPTR[0], ShmPTR[1], ShmPTR[2], ShmPTR[3]);

     pid = fork();
     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) {
          ClientProcess(ShmPTR);
          exit(0);
     }

     wait(&status);
     printf("Server has detected the completion of its child...\n");

     printf("Server performs operation on shared memory...\n");
     ShmPTR[0] = ShmPTR[0] + 10;
     printf("Server finishes with final value for the first element: %d\n", ShmPTR[0]);

     shmdt((void *)ShmPTR);
     printf("Server has detached shared memory...\n");

     shmctl(ShmID, IPC_RMID, NULL);
     printf("Server has removed shared memory...\n");

     printf("Server exits...\n");
     exit(0);
}

void ClientProcess(int *sharedMem){
     printf("Client process started\n");

     printf("Client found values: %d %d %d %d\n", sharedMem[0], sharedMem[1], sharedMem[2], sharedMem[3]);
     sharedMem[0] = sharedMem[0] * 2;
     printf("Client modified the first value to: %d\n", sharedMem[0]);

     printf("Client process finished\n");
}
