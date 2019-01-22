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
    default :
      printf("unhadled signal\n");
      break;
  }
}

void connection(int idMsgQ, msgAction actionMsg){
  int idMsgQG = msgget(70, 0660);
  if (idMsgQG == -1)
  {
    printf("msg opening error\n");
    exit(1);
  }
  actionMsg.mtype = 3;
  actionMsg.action = idMsgQ;
  asgsnd(idMsgQG, &actionMsg, sizeof(int), 0);
  if(msgrcv(idMsgQG, &actionMsg, sizeof(int), idMsgQ, 0) && actionMsg < 0) {
	  printf("Connection error");
  } 
  
  

  
  //connection à la file de message privée 
}

void disconnection(){


//envoi message de confirmation de livraison
//déconnexion à la file de message privée
//déconnexion des sémaphores
}

void test(){
  signal(SIGINT, signal_handle);
  signal(SIGALRM, signal_handle);
  int key = 80;
  int size = sizeof(int)*5;

  int id = shmget(key, size, 0660 | IPC_CREAT);
  if (id == -1){
    perror("Shared memory creation error!\n");
    exit(EXIT_FAILURE);
  }
  alarm(1);
  long i =0;
  while(1 && run){
    i++;
    if (i==1000000){
      printf("%d\n",i);
      i=0;
    }
  }


}

void order(){
 // print please wait
    //down(coursiers);
//envoi de la commande
    //up(coursiers);
}
/*Envoi du message normalisé demandant l’offre.
 */
void askOffer(){}

/*Envoi du message normalisé demandant des informations.
*/
void askInfo(){}

int main(int idUser) {
  signal(SIGINT, signal_handle);  
  //envoi d’un message normalisé pour ouvrir une connection
  int key = 255;
  //lecture de la key IPC répondu par Hubert
  int idMsgQ = msgget(key, 0660 | IPC_CREAT);
  if (idMsgQ == -1)
  {
    printf("msg creation error\n");
    exit(1);
  }
  msgAction actionMsg;
  msgName nameMsg;
  msgId idMsg;
  
  int sem = open_semaphore(70);

  //connection();
 // askInfo();
 // sendOrder();
 // disconnection();

  //test();
  
  msgctl(idMsgQ, IPC_RMID, 0);
  return 0;
}

