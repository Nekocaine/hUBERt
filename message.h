#ifndef MESSAGE_H_
#define MESSAGE_H_


typedef struct msgbuf {
  long mtype;
  int action;
} msgAction;

typedef struct msgbuf1 {
  long mtype;
  char mtext[30];
  int id;
  int nb;
} msgName;

typedef struct msgbuf2 {
  long mtype;
  int id;
  int nb;
} msgId;

#endif
