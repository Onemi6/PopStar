#define OK 1
#define ERROR 0
#define OVERFLOW -2




typedef struct cPoint
{
   int x;
   int y;
}cpoint;

typedef struct QNode       //链式存储结构。
{
  //int data;
	cpoint pp;
  struct QNode *next;
}QNode ,*QueuePtr;

typedef struct
{
QueuePtr front;
QueuePtr rear;
//int length;
}LinkQueue;

LinkQueue Q;

int InitQueue()
{
 Q.front =new(QNode);
 Q.rear =Q.front ;
 //Q.length=0;
 if(!Q.front ) return (OVERFLOW);
 Q.front ->next =NULL;
 return OK;
}

int EnQueue(cpoint e)
{
 QueuePtr p;
 p=new(QNode);
 if(!p)return(OVERFLOW);
 //p->data=e;
 p->pp.x=e.x;
 p->pp.y=e.y;
 p->next =NULL;
 Q.rear ->next =p;
 Q.rear =p;
 //Q.length++;
 return OK;
}
int DeQueue(cpoint &e)
//cpoint DeQueue()
{
QueuePtr p;
if(Q.front ==Q.rear )  return ERROR;//队空时
p=Q.front ->next ;
e.x=p->pp.x ;
e.y=p->pp.y ;
Q.front ->next=p ->next ;
if(Q.rear ==p) Q.rear =Q.front ;
//printf("%d  ",e);
delete(p);
//Q.length--;
return OK;
}


LinkQueue QQ;

int InitQueue1()
{
	QQ.front =new(QNode);
	QQ.rear =QQ.front ;
	//Q.length=0;
	if(!QQ.front ) return (OVERFLOW);
	QQ.front ->next =NULL;
	return OK;
}

int EnQueue1(cpoint e)
{
	QueuePtr p;
	p=new(QNode);
	if(!p)return(OVERFLOW);
	//p->data=e;
	p->pp.x=e.x;
	p->pp.y=e.y;
	p->next =NULL;
	QQ.rear ->next =p;
	QQ.rear =p;
	//Q.length++;
	return OK;
}
int DeQueue1(cpoint &e)
//cpoint DeQueue()
{
	QueuePtr p;
	if(QQ.front ==QQ.rear )  return ERROR;//队空时
	p=QQ.front ->next ;
	e.x=p->pp.x ;
	e.y=p->pp.y ;
	QQ.front ->next=p ->next ;
	if(QQ.rear ==p) QQ.rear =QQ.front ;
	//printf("%d  ",e);
	delete(p);
	//Q.length--;
	return OK;
}


LinkQueue QQQ;

int InitQueue2()
{
	QQQ.front =new(QNode);
	QQQ.rear =QQQ.front ;
	//Q.length=0;
	if(!QQQ.front ) return (OVERFLOW);
	QQQ.front ->next =NULL;
	return OK;
}

int EnQueue2(cpoint e)
{
	QueuePtr p;
	p=new(QNode);
	if(!p)return(OVERFLOW);
	//p->data=e;
	p->pp.x=e.x;
	p->pp.y=e.y;
	p->next =NULL;
	QQQ.rear ->next =p;
	QQQ.rear =p;
	//Q.length++;
	return OK;
}
int DeQueue2(cpoint &e)
//cpoint DeQueue()
{
	QueuePtr p;
	if(QQ.front ==QQ.rear )  return ERROR;//队空时
	p=QQQ.front ->next ;
	e.x=p->pp.x ;
	e.y=p->pp.y ;
	QQQ.front ->next=p ->next ;
	if(QQQ.rear ==p) QQQ.rear =QQ.front ;
	//printf("%d  ",e);
	delete(p);
	//Q.length--;
	return OK;
}