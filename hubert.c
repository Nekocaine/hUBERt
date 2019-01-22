#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include "message.h"
#include "semaphore.h"

void msgGetNamedStockToResto(int resto, int idMsgQR, msgAction actionMsg, msgName nameMsg, int prod[][5], char prodName[][5][30]){
	actionMsg.mtype = 1;
	actionMsg.action = 1;
	msgsnd(idMsgQR, &actionMsg, sizeof(int), 0);
	while (msgrcv(idMsgQR, &nameMsg, sizeof(int)*2+sizeof(char)*30, 2, 0) && nameMsg.id >= 0){
		prod[resto][nameMsg.id] = nameMsg.nb;
		sprintf(prodName[resto][nameMsg.id],"%s",nameMsg.mtext);
	}
}

void msgGetStockResto(int resto,int idMsgQR, msgAction actionMsg, msgId idMsg, int prod[][5]){
	actionMsg.mtype = 1;
	actionMsg.action = 2;
	msgsnd(idMsgQR, &actionMsg, sizeof(int), 0);
	while (msgrcv(idMsgQR, &idMsg, sizeof(int)*2, 2, 0) && idMsg.id >= 0){
		prod[resto][idMsg.id] = idMsg.nb;
	}
}

void msgStartCommandToResto(int resto,int idMsgQR, msgAction actionMsg){
	actionMsg.mtype = 1;
	actionMsg.action = 3;
	msgsnd(idMsgQR, &actionMsg, sizeof(int), 0);
}

void msgCommandToResto(int resto,int idMsgQR, msgId idMsg, int id, int nb){
	idMsg.mtype = 1;
	idMsg.id = id;
	idMsg.nb = nb;
	msgsnd(idMsgQR, &idMsg, sizeof(int)*2, 0);
}

void msgEndCommandToResto(int resto,int idMsgQR,msgId idMsg){
	msgCommandToResto(resto,idMsgQR, idMsg,-1,0);
}

int main() {
  int nResto = 3;
  int key = 70;
  int idMsgQR[nResto];
  int idMsgQCG = msgget(key, 0660 | IPC_CREAT);
  int i;
  int sem = create_semaphore(key);
  init_semaphore(sem, 4);
  for (i=0;i<nResto;i++){
		idMsgQR[i]= msgget(key+10*(i+1), 0660);
		if (idMsgQR ==  -1) {
			printf("msg openning error\n");
			exit(1);
		}
  }
  const char *name[nResto];
  name[0] = "Tricatel";
  name[1] = "Chez Gusteau!";
  name[2] = "Flynn's";
  char prodName[nResto][5][30];
  int firstTime = 1;
  int prod[nResto][5];
  msgAction actionMsg;
  msgName nameMsg;
  msgId idMsg;

  int type = 0;
  int resto = 0;
  int y;

  while (scanf("%d", &type) && type >= 0) {
    switch (type){
			case 1: 
				msgGetNamedStockToResto(resto, idMsgQR[resto], actionMsg, nameMsg, prod, prodName);
				msgGetNamedStockToResto(1, idMsgQR[1], actionMsg, nameMsg, prod, prodName);
				msgGetNamedStockToResto(2, idMsgQR[2], actionMsg, nameMsg, prod, prodName);
				break;
			case 2: 
				msgGetStockResto(resto,idMsgQR[resto], actionMsg, idMsg, prod);
				msgGetStockResto(1,idMsgQR[1], actionMsg, idMsg, prod);
				msgGetStockResto(2,idMsgQR[2], actionMsg, idMsg, prod);
				break;
			case 3: 
				msgStartCommandToResto(resto,idMsgQR[resto], actionMsg);
				msgCommandToResto(resto,idMsgQR[resto], idMsg,4,4);
				msgEndCommandToResto(resto,idMsgQR[resto], idMsg);
				msgGetStockResto(resto,idMsgQR[resto], actionMsg, idMsg, prod);
				break;
			default:
				break;      
		}
		for (i=0; i<3;i++){
			printf("\n%s:\n\n",name[i]);
			for (y=0; y<5;y++){
				printf("%s: %d\n",prodName[i][y],prod[i][y]);
			}
		}
  }
  msgctl(idMsgQCG, IPC_RMID, 0);
  remove_semaphore(sem);
  return 0;
}
