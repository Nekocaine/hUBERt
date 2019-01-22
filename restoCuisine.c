#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include "semaphore.h"
#include "message.h"


int run = 1;

void signal_handle(int sig){
  switch (sig) {
    case SIGINT:
      run = 0;
      break;
    case SIGALRM:
      break;
    default :
      printf("unhadled signal\n");
      break;
  }
}

int main ( int argc, char *argv[] ) {
  if (argc != 2){
	printf("restoCuisine [1..3]\n");
	exit(1);
  }
  if (atoi(argv[1])<1 || atoi(argv[1])>3){
	printf("restoCuisine [1..3]\n");
	exit(1);
  }
  
  int n;
  n = atoi( argv[1] );
	
  //Initialisation
  signal(SIGINT, signal_handle);
  signal(SIGALRM, signal_handle);
  int i, sem, pid, size, key, id;
  int* buffer;
  key = 70+10*n;
  size = 5;
  id = shmget(key, sizeof(int)*size, 0660 | IPC_CREAT);
  if (id == -1){
    perror("Shared memory creation error!\n");
    exit(EXIT_FAILURE);
  }
  sem = create_semaphore(key);
  init_semaphore(sem, 1);
  buffer = (int*) shmat(id,0,0);
  for (i = 0; i < size; i++) {
    buffer[i] = 0;
  }
  
  pid  = fork();
  //Resto
  if (pid) {
    //Init
    const char* name[5];
    msgAction actionMsg;
    msgName nameMsg;
    msgId idMsg;
    int idMsgQ, status;
    switch(n){
	  case 1:
		name[0] = "Plat de Spaghetti";
		name[1] = "Swedish Meatballs";
		name[2] = "Pizza";
		name[3] = "Bierre au beure";
		name[4] = "Chocobo à la broche";
		break;
	  case 2:
		name[0] = "Cervelle de metroid";
		name[1] = "Sushi de murloc";
		name[2] = "Omelette de Dofus";
		name[3] = "Cuisse de Pichu au curry";
		name[4] = "Pudding à l'arsenic";
		break;
	  case 3:
		name[0] = "Gnocchi de tamagotchi";
		name[1] = "Salade de mandragore";
		name[2] = "Mordor con carne";
		name[3] = "Porg roti à la broche";
		name[4] = "Nuka Cola";
		break;
	}
    idMsgQ = msgget(key, 0660 | IPC_CREAT);
    if (idMsgQ ==  -1) {
      printf("msg creation error\n");
      exit(1);
    }
    actionMsg.mtype = 1;
    
    //Core
    while (msgrcv(idMsgQ, &actionMsg, sizeof(int), 1, 0) && run){
      printf("%d\n", actionMsg.action);
      switch (actionMsg.action){
        case 1: 
          nameMsg.mtype = 2;
          down(sem);
          for(i=0;i<size;i++){
            sprintf(nameMsg.mtext, "%s", name[i]);
            nameMsg.id = i;
            nameMsg.nb = buffer[i];
            msgsnd(idMsgQ, &nameMsg, sizeof(int)*2+sizeof(char)*30, 0);
          }
          nameMsg.id = -1;
          msgsnd(idMsgQ, &nameMsg, sizeof(int)*2+sizeof(char)*30, 0);
          up(sem);
          break;
        case 2: 
          idMsg.mtype = 2;
          down(sem);
          for(i=0;i<size;i++){
            idMsg.id = i;
            idMsg.nb = buffer[i];

            msgsnd(idMsgQ, &idMsg, sizeof(int)*2, 0);
          }
          idMsg.id = -1;
          msgsnd(idMsgQ, &idMsg, sizeof(int)*2, 0);
          up(sem);
          break;
        case 3: 
          down(sem);
          idMsg.mtype = 1;
          while (msgrcv(idMsgQ, &idMsg, sizeof(int)*2, 1, 0) && idMsg.id >= 0 && run){
            buffer[idMsg.id] -= idMsg.nb;
          }
          up(sem);
          break;
        default:
          break;    
      }
      actionMsg.mtype = 1;
    }

    //End
    msgctl(idMsgQ, IPC_RMID, 0);
    wait(&status);
    shmdt(buffer);
    shmctl(id, IPC_RMID, 0);
    remove_semaphore(sem);
  } 
  
  //Cuisine
  else {
    do {
      //core
      down(sem);
      for (i = 0; i < 5; i++) {
        buffer[i]+=i;
      }
      up(sem);
      alarm(10);
      pause();
    } while(run);
    //End
    shmdt(buffer);
  }

  return 0;
}

