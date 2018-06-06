#include <windows.h>
#include <string.h>
#include "picture.h"
#include "queue.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment (lib, "winmm.lib")
#pragma comment(lib,"msimg32.lib")
#include <iostream>
using namespace std;



LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

HINSTANCE hinst;
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInst,LPSTR lpszCmdLine,int nCmdShow)
{
	HWND hwnd;
	MSG Msg;
	hinst=hInstance;
	WNDCLASS wndclass; 
	char lpszClassName[] = "窗口";
	char lpszTitle[]= "PopStar";
	
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground =(HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = "MYMENUNAME";
	wndclass.lpszClassName = lpszClassName;
	
	if(!RegisterClass(&wndclass))
	{
		MessageBeep(0) ;
		return FALSE ;
	}
	hwnd=CreateWindow(
		lpszClassName,
		lpszTitle,
		WS_OVERLAPPEDWINDOW& ~WS_MAXIMIZEBOX& ~WS_THICKFRAME,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		854,
		500,
		NULL,
		NULL,
		hInstance,
		NULL);
	
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while(GetMessage(&Msg,NULL,0,0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}


static int scene=1;
static int score1=0,score2=0,score,oldscore1=0,oldscore2=0,oldscore;
static int over=0;
static int pause=0;
static int snow=0;
static int type=0;
int m,n,count;
int JPX(HWND,WORD);
int JPY(HWND,WORD);

struct snow  
{  
    int x;  
    int y;  
    BOOL exist;  
}drop[50];
/*
struct flystar  
{  
int x;       //星光所在的x坐标  
int y;       //星光所在的y坐标  
int vx;      //星光x方向的速度  
int vy;      //星光y方向的速度  
int lasted;  //星光存在的时间  
BOOL exist;  //星光是否存在  
}flystar[50];
*/


LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM  wParam,LPARAM lParam)
{
	static WORD x,y,Y;                          //定义表示坐标的变量
	static HDC hdcmem;
    static HBITMAP hbitmap[100];
	BITMAP bitmap;
	
    RECT rect;
	
	char a1[20],a2[20],aa[20];
	HDC hdc;
	PAINTSTRUCT ps;
	int i,j,k,a,b,num,length,amount;
	
	static int p[10][10];
	
	cPoint point,e,g;
	HBRUSH hBrush;
	HFONT hf;
	TEXTMETRIC tm;
	SIZE size;
	char *s1="Gather same color,more the better.";
    char *s2="More bonus with less block at the end of stage.";
	char *s3="高分攻略";
	char *s9="Popstar游戏一直是长盛不衰的消除游戏，又是一款通杀男女老少的手游，";
	char *s10="消灭星星的游戏规则很简单，玩家只要点击两颗相同颜色的星星就可消灭";
	char *s11="星星，并获得分数!首先，popstar高分有2条途径，1条是通过积累多";
	char *s12="个星星一起消除来获得高分。还有1条是获得最后的奖励，最后剩下的";
	char *s13="星星越少，得到的奖励越多，如果一个也不剩的话，最后能获得2000";
	char *s14="分的奖励，这2000的奖励是非常多的，是得高分最重要的保证。注意，玩";
    char *s15="popstar是很需要耐心的，需要详细的计算，并不是说脑子好用就能得高分。";
	
	char *s17="PopStar是一款非常易上手的消除类游戏，游戏的规则简单：只需点击两个或";
	char *s18="两个以上颜色相同的方块即可消除,没有时间限制.游戏虽然简单，但想要玩出";
	char *s19="很高的分数还是要耗费不少脑细胞的，拿Popstar开动脑筋打发时间也是不错的";
	char *s20="选择。如果喜欢消除游戏的话，PopStar是一个不错的选择.";
	
    char *s21="游戏特色";
    char *s22="- 点击两个或更多相同的星星";
    char *s23="提示：";
    char *s24="- 你同时消去的星星越多，就能得到越高分";
    char *s25="- 最后剩下的星星越少，得到的奖励越多";
    char *s26="- 你需要经过仔细思考，才能得到更高的分数";
	char *s27="每次分数=颜色相同的星星数目*(5*(颜色相同的星星数目-1))";
	char *s28="奖励分=2000-100*最后剩下的星星数目";
	char *s29="结束后单击右键继续！游戏时可暂停。清除数据是指把最高分置零";
	
	char fs1[100],fs2[100],fs[100];
	char jfs1[100],jfs2[100],jfs[100];
	switch(message)
	{
	case WM_CREATE:                                              //初始化
		hdc=GetDC(hwnd);
		Y=10;
		
		hdcmem=CreateCompatibleDC(hdc);
		
		hbitmap[1]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP0));
		
		hbitmap[2]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP1));
		hbitmap[3]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP2));
		hbitmap[4]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP3));
		hbitmap[5]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP4));
		hbitmap[6]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP5));
		
		hbitmap[7]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP6));
		
		hbitmap[8]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP7));
		hbitmap[9]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP8));
		hbitmap[10]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP9));
		hbitmap[11]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP10));
		hbitmap[12]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP11));
		hbitmap[13]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP12));
		
		hbitmap[14]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP13));
		
		hbitmap[15]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP14));
		hbitmap[16]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP15));
		hbitmap[17]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP16));
		
		hbitmap[21]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP21));
		hbitmap[22]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP22));
		hbitmap[23]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP23));
		hbitmap[24]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP24));
		hbitmap[25]=LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP25));
		
		srand((unsigned)time(NULL));
		for(i=0;i<10;i++)                  //初始化星星
			for(j=0;j<10;j++)
			{
				k=(int)rand()%5+2;
				p[i][j]=k;
			}	
			//获得最高分
			oldscore1 = ::GetPrivateProfileInt("info","score1",0,".//info.ini");
			oldscore2 = ::GetPrivateProfileInt("info","score2",0,".//info.ini");
			oldscore = ::GetPrivateProfileInt("info","score",0,".//info.ini");
			
			break;
	case WM_MOUSEMOVE:                          //处理鼠标移动消息
		x=LOWORD(lParam);                       //获得光标所在位置的坐标值
		y=HIWORD(lParam);
		break;
	case WM_RBUTTONDOWN:                          //结束后点击右键返回继续游戏
		if(scene==2&&over==1)
		{
			Sleep(500);
			scene=1;
			over=0;
			pause=0;
			score1=0;
			score2=0;
			score=0;
			InvalidateRect(hwnd,NULL,FALSE);
		}
		break;
	case WM_LBUTTONDOWN:
		//		xx=LOWORD(lParam);                       //获得光标所在位置的坐标值
		//		yy=HIWORD(lParam);
		if(scene==1)
		{
			if((x>300&&x<476)&&(y>190&&y<226))       //新游戏开始
			{
				for(i=0;i<10;i++)                  //初始化星星
					for(j=0;j<10;j++)
					{
						k=(int)rand()%5+2;
						p[i][j]=k;
					}
					score1=0;
					score2=0;
					score=0;
					snow=0;
					type=(int)rand()%4+1;
					PlaySound("ready_go.wav", NULL, SND_FILENAME | SND_ASYNC);
					Sleep(1500);
					PlaySound(NULL, NULL, SND_PURGE) ;
					scene=2;
					pause=1;
					x=y=NULL;
					InvalidateRect(hwnd,NULL,FALSE);
			}
			if((x>295 && x<490) && (y>240 && y<300))                   //清除数据判定
			{
				if(MessageBox(0,"确定要清除数据，清除后无法恢复","提示",MB_YESNO)==IDYES)
				{
					::WritePrivateProfileString("info","score1","0",".//info.ini"); 
					::WritePrivateProfileString("info","score2","0",".//info.ini");
					MessageBox(0,"清除数据成功","提示",0);
					
					oldscore1=0;
					oldscore2=0;
					oldscore=0;
				}
			}
			if((x>30&&x<180)&&(y>130&&y<200))                       //进入help界面
			{
				x=y=NULL;
				Sleep(500);
				scene=3;
				snow=1;
				InvalidateRect(hwnd,NULL,FALSE);
			}
			if((x>590&&x<830)&&(y>130&&y<190))                       //进入about界面
			{
				x=y=NULL;
				Sleep(500);
				scene=4;
				snow=1;
				InvalidateRect(hwnd,NULL,FALSE);
			}
			if(pause==1)
			{
				if((x>610&&x<710)&&(y>270&&y<380))                  //点击继续按钮，继续游戏
				{
					x=y=NULL;
					PlaySound(NULL, NULL, SND_PURGE);
					Sleep(500);
					scene=2;
					snow=0;
					InvalidateRect(hwnd,NULL,FALSE);
				}
			}
		}
		if(scene==2)
		{
			if((x>131&&x<171)&&(y>206&&y<246)&&over==0)                 //暂停按钮，点击后初始界面出现继续按钮
			{
				x=y=NULL;
				pause=1;
				Sleep(500);
				scene=1;
				snow=0;
				InvalidateRect(hwnd,NULL,FALSE);
			}	
			num=0;
			length=0;
			amount=0;
			InitQueue();
			InitQueue2();
			a=JPX(hwnd,x);
			b=JPY(hwnd,y);
			n=p[a][b];
			if(n!=0&&x>385&&x<835&&y>12&&y<462)                   //点击星星进行判定
			{
				point.x=e.x=a;
				point.y=e.y=b;
				EnQueue(point);                             //将选中的星星入队列
				num++;
				EnQueue2(point);
				while(DeQueue(e))                             //每出队列，判定其四周的星星是否与其颜色相同，若相同，则入队列
					//for(f=Q.front->next;f!=NULL;f=f->next)
				{
					if(e.x>0)
					{
						if(p[e.x-1][e.y]==n)
						{
							point.x=e.x-1;
							point.y=e.y;
							EnQueue(point);
							EnQueue2(point);
							num++;
						}
					}
					if(e.y>0)
					{
						if(p[e.x][e.y-1]==n)
						{
							point.x=e.x;
							point.y=e.y-1;
							EnQueue(point);
							EnQueue2(point);
							num++;
						}
					}
					if(e.y<9)
					{
						if(p[e.x][e.y+1]==n)
						{
							point.x=e.x;
							point.y=e.y+1;
							EnQueue(point);
							EnQueue2(point);
							num++;
						}
					}
					if(e.x<9)
					{
						if(p[e.x+1][e.y]==n)
						{
							point.x=e.x+1;
							point.y=e.y;
							EnQueue(point);
							EnQueue2(point);
							num++;
						}
					}
					p[e.x][e.y]=0;
					InvalidateRect(hwnd,NULL,FALSE);
				}
				if(num>1)                                      //判断选中的相同颜色的星星大于一个，若是，计算分数，若不是，不进行处理
				{
					score1=score1+num*(5*(num-1));
					PlaySound("select.wav", NULL, SND_FILENAME | SND_ASYNC);
                    if((num*(5*(num-1)))>oldscore1)
					{
						oldscore1=num*(5*(num-1));
						itoa(oldscore1,a1,10);
						::WritePrivateProfileString("info","score1",a1,".//info.ini"); 
					}
					num=0;
					Sleep(800);
					PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);
					PlaySound("broken.wav", NULL, SND_FILENAME | SND_ASYNC );
					//InvalidateRect(hwnd,NULL,FALSE);
				}
				if(num==1)
				{
					p[e.x][e.y]=n;
					num=0;
					DeQueue2(e);
				}
				x=y=NULL;
				InvalidateRect(hwnd,NULL,FALSE);	
				if(type==1)
				{
					for(int c=0;c<10;c++)                          //消除后判断是否有星星悬空，有则落下
					{
						int j;
						for(int i=0;i<10;i++)   //x  列
							for(int m=9;m>0;m--)   //y  行
							{
								if(p[i][m]==0)
								{   for( j=m;j>0;j--)
								{
									p[i][j] =p[i][j-1];
								}
								p[i][j]=0;
								}
								InvalidateRect(hwnd,NULL,FALSE);
							}
							//InvalidateRect(hwnd,NULL,FALSE);
					}
					
					for(int d=0;d<10;d++)
					{
						for( i=0;i<10;i++)  //有左空列                            //判断是否有整列空，有则其右边的列依次左移
							//for(int j=0;j<10;j++)
							if(p[i][0]==0&&p[i][1]==0&&p[i][2]==0&&
								p[i][3]==0&&p[i][4]==0&&p[i][5]==0&&p[i][6]==0&&
								p[i][7]==0&&p[i][8]==0&&p[i][9]==0)
								for(int j=i;j<9;j++)
								{
									// if(p[9][j+1] !=0)//表示最后几列不为空时
									{
										p[j][0] =p[j+1][0];   p[j+1][0]=0;
										p[j][1] =p[j+1][1];   p[j+1][1]=0;
										p[j][2] =p[j+1][2];   p[j+1][2]=0;
										p[j][3] =p[j+1][3];   p[j+1][3]=0;
										p[j][4] =p[j+1][4];   p[j+1][4]=0;
										p[j][5] =p[j+1][5];   p[j+1][5]=0;
										p[j][6] =p[j+1][6];   p[j+1][6]=0;
										p[j][7] =p[j+1][7];   p[j+1][7]=0;
										p[j][8] =p[j+1][8];   p[j+1][8]=0;
										p[j][9] =p[j+1][9];   p[j+1][9]=0;
									}
									InvalidateRect(hwnd,NULL,FALSE);	
								}
								//InvalidateRect(hwnd,NULL,FALSE); 				
					}
				}
				if(type==2)
				{
					for(int c=0;c<10;c++)                          //消除后判断是否有星星悬空，有则落下
					{
						int j;
						for(int i=0;i<10;i++)   //x  列
							for(int m=9;m>0;m--)   //y  行
							{
								if(p[i][m]==0)
								{   for( j=m;j>0;j--)
								{
									p[i][j] =p[i][j-1];
								}
								p[i][j]=0;
								}
								InvalidateRect(hwnd,NULL,FALSE);
							}
							//InvalidateRect(hwnd,NULL,FALSE);
					}
					for(int d=0;d<10;d++)
					{
						for( i=9;i>0;i--)  //有右空列                            //判断是否有整列空，有则其右边的列依次左移
							//for(int j=0;j<10;j++)
							if(p[i][0]==0&&p[i][1]==0&&p[i][2]==0&&
								p[i][3]==0&&p[i][4]==0&&p[i][5]==0&&p[i][6]==0&&
								p[i][7]==0&&p[i][8]==0&&p[i][9]==0)
								for(int j=i;j>0;j--)
								{
									// if(p[9][j+1] !=0)//表示最后几列不为空时
									{
										p[j][0] =p[j-1][0];   p[j-1][0]=0;
										p[j][1] =p[j-1][1];   p[j-1][1]=0;
										p[j][2] =p[j-1][2];   p[j-1][2]=0;
										p[j][3] =p[j-1][3];   p[j-1][3]=0;
										p[j][4] =p[j-1][4];   p[j-1][4]=0;
										p[j][5] =p[j-1][5];   p[j-1][5]=0;
										p[j][6] =p[j-1][6];   p[j-1][6]=0;
										p[j][7] =p[j-1][7];   p[j-1][7]=0;
										p[j][8] =p[j-1][8];   p[j-1][8]=0;
										p[j][9] =p[j-1][9];   p[j-1][9]=0;
									}
									InvalidateRect(hwnd,NULL,FALSE);	
								}
								//InvalidateRect(hwnd,NULL,FALSE); 				
					}
				}
				if(type==3)
				{
					for(int c=0;c<10;c++)                          //消除后判断是否有星星能向上
					{
						int j;
						for(int i=0;i<10;i++)   //x  列
							for(int m=0;m<9;m++)   //y  行
							{
								if(p[i][m]==0)
								{   for( j=m;j<9;j++)
								{
									p[i][j] =p[i][j+1];
								}
								p[i][j]=0;
								}
								InvalidateRect(hwnd,NULL,FALSE);
							}
							//InvalidateRect(hwnd,NULL,FALSE);
					}
					for(int d=0;d<10;d++)
					{
						for( i=0;i<10;i++)  //有左空列                            //判断是否有整列空，有则其右边的列依次左移
							//for(int j=0;j<10;j++)
							if(p[i][0]==0&&p[i][1]==0&&p[i][2]==0&&
								p[i][3]==0&&p[i][4]==0&&p[i][5]==0&&p[i][6]==0&&
								p[i][7]==0&&p[i][8]==0&&p[i][9]==0)
								for(int j=i;j<9;j++)
								{
									// if(p[9][j+1] !=0)//表示最后几列不为空时
									{
										p[j][0] =p[j+1][0];   p[j+1][0]=0;
										p[j][1] =p[j+1][1];   p[j+1][1]=0;
										p[j][2] =p[j+1][2];   p[j+1][2]=0;
										p[j][3] =p[j+1][3];   p[j+1][3]=0;
										p[j][4] =p[j+1][4];   p[j+1][4]=0;
										p[j][5] =p[j+1][5];   p[j+1][5]=0;
										p[j][6] =p[j+1][6];   p[j+1][6]=0;
										p[j][7] =p[j+1][7];   p[j+1][7]=0;
										p[j][8] =p[j+1][8];   p[j+1][8]=0;
										p[j][9] =p[j+1][9];   p[j+1][9]=0;
									}
									InvalidateRect(hwnd,NULL,FALSE);	
								}
								//InvalidateRect(hwnd,NULL,FALSE); 				
					}
				}
				if(type==4)
				{
					for(int c=0;c<10;c++)                          //消除后判断是否有星星能向上
					{
						int j;
						for(int i=0;i<10;i++)   //x  列
							for(int m=0;m<9;m++)   //y  行
							{
								if(p[i][m]==0)
								{   for( j=m;j<9;j++)
								{
									p[i][j] =p[i][j+1];
								}
								p[i][j]=0;
								}
								InvalidateRect(hwnd,NULL,FALSE);
							}
							//InvalidateRect(hwnd,NULL,FALSE);
					}
					for(int d=0;d<10;d++)
					{
						for( i=9;i>0;i--)  //有右空列                            //判断是否有整列空，有则其左边的列依次右移
							//for(int j=0;j<10;j++)
							if(p[i][0]==0&&p[i][1]==0&&p[i][2]==0&&
								p[i][3]==0&&p[i][4]==0&&p[i][5]==0&&p[i][6]==0&&
								p[i][7]==0&&p[i][8]==0&&p[i][9]==0)
								for(int j=i;j>0;j--)
								{
									// if(p[9][j+1] !=0)//表示最后几列不为空时
									{
										p[j][0] =p[j-1][0];   p[j-1][0]=0;
										p[j][1] =p[j-1][1];   p[j-1][1]=0;
										p[j][2] =p[j-1][2];   p[j-1][2]=0;
										p[j][3] =p[j-1][3];   p[j-1][3]=0;
										p[j][4] =p[j-1][4];   p[j-1][4]=0;
										p[j][5] =p[j-1][5];   p[j-1][5]=0;
										p[j][6] =p[j-1][6];   p[j-1][6]=0;
										p[j][7] =p[j-1][7];   p[j-1][7]=0;
										p[j][8] =p[j-1][8];   p[j-1][8]=0;
										p[j][9] =p[j-1][9];   p[j-1][9]=0;
									}
									InvalidateRect(hwnd,NULL,FALSE);	
								}
								//InvalidateRect(hwnd,NULL,FALSE); 				
					}
				}
				
				
				
				InitQueue1();
				for(i=0;i<10;i++)                    //判断剩下的星星能否继续消除，不能则结束游戏
				{
					for(j=0;j<10;j++)
					{
						g.x=i;
						g.y=j;
						if(p[i][j]!=0)
						{
							EnQueue1(g);
							length++;
						}
					}
				}
				amount=length;
				while(DeQueue1(g))
				{
					
					if(p[g.x][g.y]==p[g.x-1][g.y] || p[g.x][g.y]==p[g.x][g.y-1] || p[g.x][g.y]==p[g.x+1][g.y] || p[g.x][g.y]==p[g.x][g.y+1])
						over=0;
					else
						length--;	
				}
				if(length==0)
				{
					x=y=NULL;
					//Sleep(1000);
					over=1;
					pause=0;
					//for(i=0;i<50;i++)
					//	drop[i].y=0;
					PlaySound("game_over.wav",NULL, SND_FILENAME | SND_ASYNC);
					InvalidateRect(hwnd,NULL,FALSE);
					if(amount>20)
						score2=0;
					else
						score2=2000-amount*100;
				}
				if(score2>oldscore2)
				{
					oldscore2=score2;
					itoa(oldscore2,a2,10);
					::WritePrivateProfileString("info","score2",a2,".//info.ini"); 
				}
				score=score1+score2;
				if(score>oldscore)
				{
					oldscore=score;
					itoa(score,aa,10);
					::WritePrivateProfileString("info","score",aa,".//info.ini");
				}
				InvalidateRect(hwnd,NULL,FALSE);		
			}
			
		}
		if(scene==3)
		{
			if((x>635&&x<835)&&(y>320&&y<440))                //返回初始界面
			{
				snow=0;
				x=y=NULL;
				Sleep(500);
				scene=1;
				InvalidateRect(hwnd,NULL,FALSE);
				
			}  	
		}
		if(scene==4)                                          //返回初始界面
		{
			if((x>635&&x<835)&&(y>320&&y<440))
			{
				snow=0;
				x=y=NULL;
				Sleep(500);
				scene=1;
				InvalidateRect(hwnd,NULL,FALSE);
				//PlaySound(NULL, NULL, SND_PURGE) ;
			}
		}		
		break;
	case WM_PAINT:
		hdc=BeginPaint(hwnd,&ps); 
		GetClientRect(hwnd, &rect);
		
		if(scene==1)
		{
			PlaySound("game_music.wav", NULL, SND_LOOP | SND_ASYNC);
			
			hBrush=CreateSolidBrush(RGB(0,0,0));           //整个屏幕涂黑
			SelectObject(hdc,hBrush);
			Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
			
			SelectObject(hdcmem,hbitmap[7]);               //输出PopStar图片
			GetObject(hbitmap[7],sizeof(BITMAP),&bitmap);
			StretchBlt(hdc,270,0,bitmap.bmWidth,bitmap.bmHeight,hdcmem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
			
			SelectObject(hdcmem,hbitmap[15]);               //输出清除数据图片
			GetObject(hbitmap[15],sizeof(BITMAP),&bitmap);
			StretchBlt(hdc,285,230,bitmap.bmWidth,bitmap.bmHeight,hdcmem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
			
			SelectObject(hdcmem,hbitmap[8]);               //输出开始图片
			GetObject(hbitmap[8],sizeof(BITMAP),&bitmap);
			StretchBlt(hdc,300,190,bitmap.bmWidth,bitmap.bmHeight,hdcmem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
			
			SelectObject(hdcmem,hbitmap[10]);              //输出city图
			GetObject(hbitmap[10],sizeof(BITMAP),&bitmap);
			SetStretchBltMode(hdc,COLORONCOLOR);
			StretchBlt(hdc,0,320,854,180,hdcmem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
			
			SelectObject(hdcmem,hbitmap[11]);                //输出Help图标
			GetObject(hbitmap[11],sizeof(BITMAP),&bitmap);
			StretchBlt(hdc,30,70,bitmap.bmWidth,bitmap.bmHeight,hdcmem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
			
			SelectObject(hdcmem,hbitmap[12]);               //输出About图标
			GetObject(hbitmap[12],sizeof(BITMAP),&bitmap);
			TransparentBlt(hdc,580,120,bitmap.bmWidth,bitmap.bmHeight,hdcmem,0,0,bitmap.bmWidth,bitmap.bmHeight,RGB(0,0,0));
			
			SetTextColor(hdc,RGB(255,255,255));              //输出文字
			SetBkColor(hdc,RGB(0,0,0));
			GetTextMetrics(hdc,&tm);
			GetTextExtentPoint32(hdc,s1,strlen(s1),&size);
			TextOut(hdc,280,295,s1,strlen(s1));
			TextOut(hdc,250,315,s2,strlen(s2));
			
			if(pause==1 && over==0)
			{
				SelectObject(hdcmem,hbitmap[17]);               //输出继续图标
				GetObject(hbitmap[17],sizeof(BITMAP),&bitmap);
				TransparentBlt(hdc,580,240,bitmap.bmWidth,bitmap.bmHeight,hdcmem,0,0,bitmap.bmWidth,bitmap.bmHeight,RGB(0,0,255));
			}
			
        }
		
		else if(scene==2)
		{
			SelectObject(hdcmem,hbitmap[1]);                          //画背景
			GetObject(hbitmap[1],sizeof(BITMAP),&bitmap);
			SetStretchBltMode(hdc,COLORONCOLOR);
			StretchBlt(hdc,0,0,bitmap.bmWidth,bitmap.bmHeight,hdcmem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 
			if(pause==1)                                               //显示暂停按钮
			{
				SelectObject(hdcmem,hbitmap[16]);
				GetObject(hbitmap[16],sizeof(BITMAP),&bitmap);
				TransparentBlt(hdc,125,200,bitmap.bmWidth*2,bitmap.bmHeight*2,hdcmem,0,0,bitmap.bmWidth,bitmap.bmHeight,RGB(0,255,0));
			}
			//BitBlt(hdc,0,0,bitmap.bmWidth,bitmap.bmHeight,hdcmem,0,0,SRCCOPY);
			for(i=0;i<10;i++)                                //显示初始化好的星星
				for(j=0;j<10;j++)
				{
					k=p[i][j];
					if(k>=2&&k<=6)
					{
						SelectObject(hdcmem,hbitmap[k]);
						GetObject(hbitmap[k],sizeof(BITMAP),&bitmap);
						StretchBlt(hdc,375+45*i+10,7+45*j+5,bitmap.bmWidth,bitmap.bmHeight,hdcmem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 
					}
				}
				hf=CreateFont(40,0,0,0,400,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"大号字");
				SetTextColor(hdc,RGB(255,255,255));              //输出分数
				SetBkColor(hdc,RGB(0,0,0));
				SelectObject(hdc,hf);
				GetTextMetrics(hdc,&tm);
				
				sprintf(fs1,"消除分数：%d",score1);
				GetTextExtentPoint32(hdc,fs1,strlen(fs1),&size);
				TextOut(hdc,0,0,fs1,strlen(fs1)); 
				
				sprintf(fs2,"奖励分数：%d",score2);
				GetTextExtentPoint32(hdc,fs2,strlen(fs2),&size);
				TextOut(hdc,0,40,fs2,strlen(fs2));
				
				sprintf(fs,"总分数：%d",score);
				GetTextExtentPoint32(hdc,fs,strlen(fs),&size);
				TextOut(hdc,0,80,fs,strlen(fs));
				
				sprintf(jfs1,"最高消除分数：%d",oldscore1);
				GetTextExtentPoint32(hdc,jfs1,strlen(jfs1),&size);
				TextOut(hdc,0,340,jfs1,strlen(jfs1)); 
				
				sprintf(jfs2,"最高奖励分数：%d",oldscore2);
				GetTextExtentPoint32(hdc,jfs2,strlen(jfs2),&size);
				TextOut(hdc,0,380,jfs2,strlen(jfs2));
				
				sprintf(jfs,"最高总分数：%d",oldscore);
				GetTextExtentPoint32(hdc,jfs,strlen(jfs),&size);
				TextOut(hdc,0,420,jfs,strlen(jfs));		
				
		}
		else if(scene==3)
		{
			PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);
			//PlaySound(NULL, NULL, SND_PURGE);
			
			hBrush=CreateSolidBrush(RGB(255,255,255));           //整个屏幕涂黑
			SelectObject(hdc,hBrush);
			Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);	
			
			SelectObject(hdcmem,hbitmap[13]);                //输出Return图标
			GetObject(hbitmap[13],sizeof(BITMAP),&bitmap);
			TransparentBlt(hdc,625,280,bitmap.bmWidth,bitmap.bmHeight,hdcmem,0,0,bitmap.bmWidth,bitmap.bmHeight,RGB(255,255,255));
			//StretchBlt(hdc,625,0,bitmap.bmWidth,bitmap.bmHeight,hdcmem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
			
			hf=CreateFont(30,0,0,0,700,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"大号字");	
			SelectObject(hdc,hf);
			SetTextColor(hdc,RGB(255,0,255));              //输出文字
			SetBkColor(hdc,RGB(255,255,255));
			GetTextMetrics(hdc,&tm);
			GetTextExtentPoint32(hdc,s3,strlen(s3),&size);
			TextOut(hdc,5,5,s3,strlen(s3));
			
			hf=CreateFont(20,0,0,0,400,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"大号字");	
			SelectObject(hdc,hf);
			SetTextColor(hdc,RGB(255,0,0));              //输出文字
			SetBkColor(hdc,RGB(255,255,255));
			GetTextMetrics(hdc,&tm);
			TextOut(hdc,30,35,s9,strlen(s9));
			TextOut(hdc,5,55,s10,strlen(s10));
			TextOut(hdc,5,75,s11,strlen(s11));
			TextOut(hdc,5,95,s12,strlen(s12));
			TextOut(hdc,5,115,s13,strlen(s13));
			TextOut(hdc,5,135,s14,strlen(s14));
			TextOut(hdc,5,155,s15,strlen(s15)); 
			
		}
		else if(scene==4)
		{	
			PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);
			//PlaySound(NULL, NULL, SND_PURGE);
			
			hBrush=CreateSolidBrush(RGB(0,0,0));           //整个屏幕涂白
			SelectObject(hdc,hBrush);
			Rectangle(hdc,0,0,854,500);
			
			SelectObject(hdcmem,hbitmap[13]);                //输出Return图标
			GetObject(hbitmap[13],sizeof(BITMAP),&bitmap);
			TransparentBlt(hdc,625,280,bitmap.bmWidth,bitmap.bmHeight,hdcmem,0,0,bitmap.bmWidth,bitmap.bmHeight,RGB(255,255,255));
			
			hf=CreateFont(30,0,0,0,700,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"大号字");	
			SelectObject(hdc,hf);
			SetTextColor(hdc,RGB(255,0,255));              //输出文字
			SetBkColor(hdc,RGB(0,0,0));
			GetTextMetrics(hdc,&tm);
			GetTextExtentPoint32(hdc,s3,strlen(s3),&size);
			TextOut(hdc,5,95,s21,strlen(s21));
			
			hf=CreateFont(20,0,0,0,700,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"大号字");	
			SelectObject(hdc,hf);
			SetTextColor(hdc,RGB(255,0,255));              //输出文字
			SetBkColor(hdc,RGB(0,0,0));
			GetTextMetrics(hdc,&tm);
			GetTextExtentPoint32(hdc,s3,strlen(s3),&size);
			TextOut(hdc,5,145,s23,strlen(s23));
			
			hf=CreateFont(20,0,0,0,400,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"大号字");	
			SelectObject(hdc,hf);
			SetTextColor(hdc,RGB(255,0,0));              //输出文字
			SetBkColor(hdc,RGB(0,0,0));
			GetTextMetrics(hdc,&tm);
			//Y=tm.tmHeight+tm.tmExternalLeading;
			TextOut(hdc,30,5,s17,strlen(s17));
			TextOut(hdc,5,25,s18,strlen(s18));
			TextOut(hdc,5,45,s19,strlen(s19));
			TextOut(hdc,5,65,s20,strlen(s20));
			
			TextOut(hdc,5,125,s22,strlen(s22));
			TextOut(hdc,5,165,s24,strlen(s24));
			TextOut(hdc,5,185,s25,strlen(s25));
			TextOut(hdc,5,205,s26,strlen(s26));
			
			SetTextColor(hdc,RGB(0,0,255));              //输出文字
			SetBkColor(hdc,RGB(0,0,0));
			GetTextMetrics(hdc,&tm);
			TextOut(hdc,5,240,s27,strlen(s27));
			TextOut(hdc,5,270,s28,strlen(s28));
			TextOut(hdc,5,300,s29,strlen(s29));		
			
		}
		if(over==1)
		{
			SelectObject(hdcmem,hbitmap[9]);
			GetObject(hbitmap[9],sizeof(BITMAP),&bitmap);
			TransparentBlt(hdc,270,200,bitmap.bmWidth,bitmap.bmHeight,hdcmem,0,0,bitmap.bmWidth,bitmap.bmHeight,RGB(26,24,7));
			
			if(count<50)  //当粒子数小于50时，产生新的粒子，设定每个粒子的属性值  
			{  
				drop[count].x = rand()%850; //将粒子的X坐标设为窗口中水平方向上的任意位置  
				drop[count].y = 0;    //将每个粒子的Y坐标都设为"0"，即从窗口上沿往下落  
				drop[count].exist = true; //设定粒子存在  
				count++;   //每产生一个粒子后进行累加计数  
			}
			
			//首先判断粒子是否存在，若存在，进行透明贴图操作
			for(i=0;i<50;i++)  
			{  
				InvalidateRect(hwnd,NULL,FALSE);
				Sleep(1);  
				if(drop[i].exist) 
				{  
					int w=(int)rand()%5+21;
					SelectObject(hdcmem,hbitmap[w]);
					GetObject(hbitmap[w],sizeof(BITMAP),&bitmap);
					TransparentBlt(hdc,drop[i].x,drop[i].y,20,20,hdcmem,0,0,9,9,RGB(255,255,255));			
					if(rand()%2==0)  
						drop[i].x+=5;  
					else   
						drop[i].x-=5;  
					drop[i].y+=10;  
					if(drop[i].y > 500)  
					{  
						drop[i].x = rand()%850;  
						drop[i].y = 0;  
					}  
				}	
				
			}
		}
		
		
		
		
		
		/*        if(snow==1)
		{
		if(count<50)  //当粒子数小于50时，产生新的粒子，设定每个粒子的属性值  
		{  
		drop[count].x = rand()%850; //将粒子的X坐标设为窗口中水平方向上的任意位置  
		drop[count].y = 0;    //将每个粒子的Y坐标都设为"0"，即从窗口上沿往下落  
		drop[count].exist = true; //设定粒子存在  
		count++;   //每产生一个粒子后进行累加计数  
		}
		//首先判断粒子是否存在，若存在，进行透明贴图操作
		for(i=0;i<50;i++)  
		{  
		Sleep(3);	
		if(drop[i].exist)  
		{	
		SelectObject(hdcmem,hbitmap[14]);
		GetObject(hbitmap[14],sizeof(BITMAP),&bitmap);
		TransparentBlt(hdc,drop[i].x,drop[i].y,20,20,hdcmem,0,0,bitmap.bmWidth,bitmap.bmHeight,RGB(0,255,0));
		if(rand()%2==0)  
		drop[i].x+=5;  
		else   
		drop[i].x-=5;  
		drop[i].y+=10;  
		if(drop[i].y > 500)  
		{  
		drop[i].x = rand()%850;  
		drop[i].y = 0;  
		} 
		}
		InvalidateRect(hwnd,NULL,FALSE);
		}
		}
		*/
		DeleteObject(hBrush);
		DeleteObject(hf);
		
		ReleaseDC(hwnd, hdc);
		EndPaint(hwnd,&ps);
		break;
		case WM_KEYDOWN:                     //按键消息    
            if(wParam==VK_ESCAPE)            //按下【Esc】键  
                PostQuitMessage(0);  
            break;			
		case WM_DESTROY:
			for(i=0;i<100;i++)
				DeleteObject(hbitmap[i]);
			
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd,message,wParam,lParam);
			break;
	}
	return 0;
}

//判断鼠标点击了哪个星星
int JPX(HWND hwnd, WORD x)
{
	for(m=0;m<10;m++)
		if(x>=375+45*m+10&&x<375+45*m+44+10)
			return m;
}

int JPY(HWND hwnd, WORD y)
{
	for(n=0;n<10;n++)
		if(y>=7+45*n+5&&y<7+45*n+44+5)
			return n;
}
