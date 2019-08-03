#include "stdafx.h"

const char Disc[2]= "E";		// Дириктория диска
CRITICAL_SECTION cs;			// Семафора
GameStage GAME_STAGE_INDEX;		// Весьма важная штука
PlayerInfo PlInf;				// Информация о текущем местоположении игрока
queue<COORD> MoveList;			// Очередь на обработку перемещений персонажа
bool Fount=OFF;			// Включен ли фонтан
bool InZoneFount=NO;	// Игрок в зоне слышимости фонтана
bool FMThr=OFF;			// Включен поток звука фонтана
List* LAVA=CreateList();	// Динамический список координат клеток с лавой
BackgroundMusic BM;			// Текущая фоновая музыка
bool Cem=NO;			// Игрок на кладбище

DWORD WINAPI Stage1MusicThread(void *lpPar)
{	char Buf[40]="";
	char Dir[30]="://Console/1.wav";
	strcat_s(Buf,Disc);
	strcat_s(Buf,Dir);
	PlaySoundA(Buf, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	while(GAME_STAGE_INDEX==STAGE_ONE && BM==StdSonic_1);
	PlaySoundA(0,0,0);
	ExitThread(0);
}

DWORD WINAPI MenuMusicThread(void *lpPar)
{	char Buf[40]="";
	char Dir[30]="://Console/menu.wav";
	strcat_s(Buf,Disc);
	strcat_s(Buf,Dir);
	PlaySoundA(Buf, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	while(GAME_STAGE_INDEX==MENU && BM==MenuMario);
	PlaySoundA(0,0,0);
	ExitThread(0);
}

DWORD WINAPI CreditsMusicThread(void *lpPar)
{	char BUF[40]="";
	char DIR[30]="://Console/Credits.wav";
	strcat_s(BUF,Disc);
	strcat_s(BUF,DIR);
	PlaySoundA(BUF, NULL, SND_FILENAME | SND_ASYNC);
	while(GAME_STAGE_INDEX==DEATH && BM==Death);
	PlaySoundA(0,0,0);
	ExitThread(0);
}

DWORD WINAPI CemeteryCrossBlink(void *lpPar)
{	COORD cursorPos;
	HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	List* Grave=CreateList();
	for(cursorPos.X=32; cursorPos.X<35; cursorPos.X+=2)
		for(cursorPos.Y=18; cursorPos.Y<25; cursorPos.Y+=2)
			Grave->Insert(cursorPos);
	cursorPos.X=37;
	cursorPos.Y=20;
	Grave->Insert(cursorPos);
	cursorPos.Y=22;
	Grave->Insert(cursorPos);
	bool col=true;
	while(GAME_STAGE_INDEX==STAGE_ONE && BM==CemeteryJacson)
	{	Sleep(200);
		Node* Cross=Grave->Head;
		EnterCriticalSection( &cs);
		if(col==true)
			while(Cross!=NULL)
			{	cursorPos=Cross->Coord;
				SetConsoleTextAttribute(consoleOutput, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
				SetConsoleCursorPosition(consoleOutput, cursorPos);
				printf("%c",197);
				Cross=Cross->Next;
			}
		else
			while(Cross!=NULL)
			{	cursorPos=Cross->Coord;
				SetConsoleTextAttribute(consoleOutput, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
				SetConsoleCursorPosition(consoleOutput, cursorPos);
				printf("%c",197);
				Cross=Cross->Next;
			}
		LeaveCriticalSection( &cs);
		col=!col;
    }
	Node* Cross=Grave->Head;
	while(Cross!=NULL)
	{	cursorPos=Cross->Coord;
		SetConsoleTextAttribute(consoleOutput, BACKGROUND_RED );
		SetConsoleCursorPosition(consoleOutput, cursorPos);
		printf("%c",197);
		Cross=Cross->Next;
	}
	ExitThread(0);
}

DWORD WINAPI CemeteryMusicThread(void *lpPar)
{	char BufB[40]="";
	char DirB[30]="://Console/ThrillerBegin.wav";
	strcat_s(BufB,Disc);
	strcat_s(BufB,DirB);
	PlaySoundA(BufB, NULL, NULL);
	DWORD CemeteryBlink;
	HANDLE CemeteryBlinkHandle=CreateThread(0, 0, CemeteryCrossBlink, 0, 0, &CemeteryBlink);
	char Buf[40]="";
	char Dir[30]="://Console/Thriller2.wav";
	strcat_s(Buf,Disc);
	strcat_s(Buf,Dir);
	PlaySoundA(Buf, NULL, SND_FILENAME | SND_ASYNC);
	while(GAME_STAGE_INDEX==STAGE_ONE && BM==CemeteryJacson);
	PlaySound(0,0,0);
	ExitThread(0);
}

DWORD WINAPI FountainWatherSound(void *lpPar)
{	char BUF[50]="play ";
	char DIR[40]="://Console/FountainWather.wav";
	strcat_s(BUF,Disc);
	strcat_s(BUF,DIR);
	mciSendStringA(BUF, NULL, 0, NULL);
	while(InZoneFount==YES && Fount==ON);
	char BUF2[50]="close ";
	strcat_s(BUF2,Disc);
	strcat_s(BUF2,DIR);
	mciSendStringA(BUF2, NULL, 0, 0);
	FMThr=OFF;
	ExitThread(0);
}

DWORD WINAPI WaitAfterDeath(void *lpPar)
{	HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursorPos;
	cursorPos.X=0;
	cursorPos.Y=60;
	EnterCriticalSection( &cs);
	SetConsoleCursorPosition(consoleOutput,cursorPos);
	SetConsoleTextAttribute(consoleOutput,NULL);
	char Buf[100];
	gets_s(Buf);
	LeaveCriticalSection( &cs);
	if(strcmp(Buf,"SecondChance")==0 || strcmp(Buf,"secondchance")==0 || strcmp(Buf,"SECONDCHANCE")==0 || strcmp(Buf,"Second Chance")==0 || 
	strcmp(Buf,"Second chance")==0 || strcmp(Buf,"second chance")==0 || strcmp(Buf,"second_chance")==0 || strcmp(Buf,"SECOND_CHANCE")==0 || 
	strcmp(Buf,"Second_Chance")==0 || strcmp(Buf,"Second_chance")==0 )
		GAME_STAGE_INDEX=SECOND_CHANCE;
	else
		GAME_STAGE_INDEX=END;
	ExitThread(0);
}

DWORD WINAPI PlayersDeath(void *lpPar)
{	HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	char Buff[40]="play ";
	char Dir[30]="://Console/Shipenie.wav";
	strcat_s(Buff,Disc);
	strcat_s(Buff,Dir);
	mciSendStringA(Buff, NULL, 0, NULL);
	if(PlInf.Symb==' ')
		SetConsoleTextAttribute(consoleOutput,NULL);
	else 
	{	if(PlInf.Symb==176)
			SetConsoleTextAttribute(consoleOutput,BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		else 
		{	if(PlInf.Symb==178)
				SetConsoleTextAttribute(consoleOutput, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			else
			{	if(PlInf.PlayerPos.Y>3)
					SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				else
					SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
			}
		}
	}
	SetConsoleCursorPosition(consoleOutput,PlInf.PlayerPos);
	putchar(PlInf.Symb);
	char Buffer[40]="play ";
	char Direct[30]="://Console/Pain.wav";
	strcat_s(Buffer,Disc);
	strcat_s(Buffer,Direct);
	mciSendStringA(Buffer, NULL, 0, NULL);
	GAME_STAGE_INDEX=DEATH;
	Sleep(500);
	DWORD CreditsThread;
	HANDLE CreditsHandle=CreateThread(0, 0, CreditsMusicThread, NULL, 0, &CreditsThread);
	BM=Death;
	DWORD ChanceThread;
	HANDLE ChanceHandle=CreateThread(0, 0, WaitAfterDeath, NULL, 0, &ChanceThread);
	int i=0;
	while(GAME_STAGE_INDEX==DEATH || GAME_STAGE_INDEX==END)
	{	if(i==60 || GAME_STAGE_INDEX==END)
		{	PlaySoundA(NULL,NULL,NULL);
			char Buffer[40]="play ";
			char Direct[30]="://Console/GameOver.wav";
			strcat_s(Buffer,Disc);
			strcat_s(Buffer,Direct);
			mciSendStringA(Buffer, NULL, 0, NULL);
			Sleep(2000);
			exit(0);
		}
	Sleep(1000);
	i++;
	}
	ExitThread(0);
}

DWORD WINAPI LavaBlink(void *lpPar)
{	COORD cursorPos;
	HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	bool col=true;
	while(GAME_STAGE_INDEX==STAGE_ONE)
	{	Sleep(300);
		EnterCriticalSection( &cs);
		Node* lv=LAVA->Head;
		if(col==true)
			while(lv!=NULL)
			{	cursorPos=lv->Coord;
				SetConsoleTextAttribute(consoleOutput, FOREGROUND_RED | BACKGROUND_RED | BACKGROUND_INTENSITY);
				SetConsoleCursorPosition(consoleOutput, cursorPos);
				printf("%c",'.');
				if(cursorPos.X==PlInf.PlayerPos.X && cursorPos.Y==PlInf.PlayerPos.Y)
				{	LeaveCriticalSection( &cs);
					DWORD PlayersDeathThread;
					HANDLE PlayersDeathHandle=CreateThread(0, 0, PlayersDeath, 0, 0, &PlayersDeathThread);
					WaitForSingleObject(PlayersDeathHandle, 70000);
					ExitThread(0);
				}
				lv=lv->Next;
			}
		else
			while(lv!=NULL)
			{	cursorPos=lv->Coord;
				SetConsoleTextAttribute(consoleOutput, FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED);
				SetConsoleCursorPosition(consoleOutput, cursorPos);
				printf("%c",'.');
				if(cursorPos.X==PlInf.PlayerPos.X && cursorPos.Y==PlInf.PlayerPos.Y)
				{	LeaveCriticalSection( &cs);
					DWORD PlayersDeathThread;
					HANDLE PlayersDeathHandle=CreateThread(0, 0, PlayersDeath, 0, 0, &PlayersDeathThread);
					WaitForSingleObject(PlayersDeathHandle, 70000);
					ExitThread(0);
				}
				lv=lv->Next;
			}
		LeaveCriticalSection( &cs);
		col=!col;
    }
	ExitThread(0);
}

DWORD WINAPI LavaTraps(void *lpPar)
{	HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD lavaPos;
	COORD deletePos;
	lavaPos.X=62;
	deletePos.X=62;
	deletePos.Y=27;
	LAVA->Insert(deletePos);
	deletePos.Y++;
	LAVA->Insert(deletePos);
	while(GAME_STAGE_INDEX==STAGE_ONE)
	{	for(lavaPos.Y=15; lavaPos.Y<29 && GAME_STAGE_INDEX==STAGE_ONE; lavaPos.Y++)
		{	LAVA->Delete();
			LAVA->Delete();
			LAVA->Insert(lavaPos);
			lavaPos.Y++;;
			LAVA->Insert(lavaPos);
			EnterCriticalSection( &cs);
			SetConsoleTextAttribute(consoleOutput, NULL);
			if(lavaPos.Y==16)
				deletePos.Y=27;
			else
				deletePos.Y=lavaPos.Y-3;
			SetConsoleCursorPosition(consoleOutput, deletePos);
			puts(" ");
			deletePos.Y++;
			SetConsoleCursorPosition(consoleOutput, deletePos);
			puts(" ");
			LeaveCriticalSection( &cs);
			Sleep(200);
		}
	}
	ExitThread(0);
}

DWORD WINAPI Fountain(void *lpPar)
{	char Buf[40]="play ";
	char Dir[30]="://Console/Fountain.wav";
	strcat_s(Buf,Disc);
	strcat_s(Buf,Dir);
	mciSendStringA(Buf, NULL, NULL, NULL);
	HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursorPos;
	lpParametr *Par=(lpParametr*)lpPar;
	int i=1;
	int Dm=5;
	srand((unsigned)time(NULL));
	cursorPos.X=Par->x;
	cursorPos.Y=Par->y;
	while(GAME_STAGE_INDEX==STAGE_ONE)
	{	Sleep(200);
		if(Fount==OFF)
		{	mciSendStringA(Buf, NULL, NULL, NULL);
			ExitThread(0);
		}
		EnterCriticalSection( &cs);
		switch(i%2)
		{	case 0 :	
			{	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE | FOREGROUND_INTENSITY | FOREGROUND_BLUE);
				SetConsoleCursorPosition(consoleOutput, cursorPos);
				printf("%c", Par->symb);
				break;
			}
			case 1 :
			{	SetConsoleTextAttribute( consoleOutput, BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_BLUE);
				SetConsoleCursorPosition(consoleOutput, cursorPos);
				printf("%c", Par->symb);
				break;
			}
		}
		LeaveCriticalSection( &cs);
		i++;
		if(i%5==0)
		{	COORD NewPos;
			char *Buf=new char();
			DWORD cbRead;
			int DmIter=Dm;
			int Count=0;
			do
			{	do
				{	if(Count%2==0)
					{	NewPos.X=Par->x-2-Count/2+(rand()%(DmIter+(DmIter%2)-1));
						NewPos.Y=Par->y-2-Count/2+(rand()%2+((DmIter+(DmIter%2)-1)-1))%(DmIter+(DmIter%2)-1);
					}
					if(Count%2==1)
					{	NewPos.Y=Par->y-2-Count/2+(rand()%(DmIter-DmIter%2));
						NewPos.X=Par->x-2-Count/2+(rand()%2+((DmIter+(DmIter%2)-1)-1))%(DmIter+(DmIter%2)-1);
					}
					ReadConsoleOutputCharacterW(consoleOutput, (LPWSTR)Buf, 1, NewPos, &cbRead);
					Count++;
					DmIter++;
				}
				while(PlInf.PlayerPos.X==NewPos.X  && PlInf.PlayerPos.Y==NewPos.Y);
			}
			while(*Buf==-111 && DmIter<20);
			EnterCriticalSection( &cs);
			SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE | FOREGROUND_INTENSITY | FOREGROUND_BLUE);
			SetConsoleCursorPosition(consoleOutput, NewPos);
			printf("%c", Par->symb);
			LeaveCriticalSection( &cs);
		}
	}
	ExitThread(0);
}

DWORD WINAPI Cemetery(void *lpPar)
{	BM=StdSonic_1;
	PlaySound(0,0,0);
	for(int i=0; i<2000; i++)
	{	Sleep(10);
		if(Cem==NO && GAME_STAGE_INDEX==STAGE_ONE)
		{	BM=CemeteryJacson;
			Sleep(10);
			BM=StdSonic_1;
			ExitThread(0);
		}
	}
	BM=CemeteryJacson;
	DWORD CemeteryMusic;
	HANDLE CemeteryMusicHandle=CreateThread(0, 0, CemeteryMusicThread, 0, 0, &CemeteryMusic);
	ExitThread(0);
}

DWORD WINAPI ThreadBlink(void* Par)
{	HANDLE consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);	
	COORD cursorPos;
	lpParametr *New=(lpParametr*)Par;
	int i=1;
	cursorPos.X=New->x;
	cursorPos.Y=New->y;	
	while( i>0 )
	{	EnterCriticalSection( &cs );
		SetConsoleCursorPosition(consoleOutput, cursorPos);
		switch(i%3)
		{	case 0 :	
			{	SetConsoleTextAttribute( consoleOutput, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_RED);	
				printf("%c", New->symb);
				Sleep(400);
				break;
			}
			case 1 :
			{	SetConsoleTextAttribute( consoleOutput, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_GREEN);
				printf("%c", New->symb);
				Sleep(400);
				break;
			}
			case 2 :
			{	SetConsoleTextAttribute( consoleOutput, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_BLUE);
				printf("%c", New->symb);
				Sleep(400);
				break;
			}
		}
		LeaveCriticalSection(&cs);
		i++;
		if(GAME_STAGE_INDEX!=MENU)
			ExitThread(0);
	}
	puts("А вот эта надпись никогда не появится на экране)");
	return 0;
}

DWORD WINAPI Background(void* lpParametr)
{	srand((unsigned)time(NULL));
	HANDLE consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursorPos;
	int lucky, i=1;
	while( i>0 )
	{	lucky=rand()%6;
		if( lucky==0 || lucky==1 )
		{	EnterCriticalSection( &cs );
			do
				cursorPos.X=rand()%80;
			while( cursorPos.X>19 && cursorPos.X<60 );
			cursorPos.Y=rand()%29;
			SetConsoleCursorPosition(consoleOutput, cursorPos);
			SetConsoleTextAttribute( consoleOutput, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY );
			puts(" ");
			LeaveCriticalSection( &cs );
			Sleep(100);
		}
		if( lucky==2 )	
		{	EnterCriticalSection( &cs );
			do
				cursorPos.Y=rand()%29;
			while( cursorPos.Y>5 && cursorPos.Y<24 );
			cursorPos.X=rand()%80;
			SetConsoleCursorPosition(consoleOutput, cursorPos);
			SetConsoleTextAttribute( consoleOutput, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY );
			puts(" ");
			LeaveCriticalSection( &cs );
			Sleep(100);
		}
		if( lucky==3 || lucky==4 )
		{	EnterCriticalSection( &cs );
			do
				cursorPos.X=rand()%80;
			while( cursorPos.X>19 && cursorPos.X<60 );
			cursorPos.Y=rand()%29;
			SetConsoleCursorPosition(consoleOutput, cursorPos);
			SetConsoleTextAttribute( consoleOutput, NULL );
			puts(" ");
			LeaveCriticalSection( &cs );
			Sleep(100);
		}
		if( lucky==5 )	
		{	EnterCriticalSection( &cs );
			do
				cursorPos.Y=rand()%29;
			while( cursorPos.Y>5 && cursorPos.Y<24 );
			cursorPos.X=rand()%80;
			SetConsoleCursorPosition(consoleOutput, cursorPos);
			SetConsoleTextAttribute( consoleOutput, NULL );
			puts(" ");
			LeaveCriticalSection( &cs );
			Sleep(100);
		}
		if(GAME_STAGE_INDEX!=MENU)
			ExitThread(0);
	}
	return 0;
}

void GenericLava()
{	COORD cursorPos;
	DWORD cbRead;
	char *Buf=new char();
	HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	for(cursorPos.Y=0; cursorPos.Y<30; cursorPos.Y++)
		for(cursorPos.X=0; cursorPos.X<80; cursorPos.X++)
		{	ReadConsoleOutputCharacterW(consoleOutput, (LPWSTR)Buf, 1, cursorPos, &cbRead);
			if(*Buf=='.')
				LAVA->Insert(cursorPos);
		}
}

int* MenuFunction(int *Menu, char Symb)
{	int i=0;
	while( Menu[i]!=1 )
			i++;
	if(Symb==UP)
	{	if( i==0 )
			return Menu;
		Menu[i-1]=Menu[i];
		Menu[i]=0;
		return Menu;
	}
	if(Symb==DOWN)
	{	if( i==(MENU_BUTTON-1) )
			return Menu;
		Menu[i+1]=Menu[i];
		Menu[i]=0;
		return Menu;
	}
	return NULL;
}

void BuildFirstStageMap()
{	HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursorPos;
	SetConsoleCP(1251);
	int **CoordMatr=new int*[80];
	for(int i=0; i<80; i++)
		CoordMatr[i]=new int[30];
	char Buffer[40]="";
	char Dir[30]="://Console/ObjectMatr.txt";
	strcat_s(Buffer,Disc);
	strcat_s(Buffer,Dir);
	FILE *in=fopen(Buffer, "rt");
	char Buf;
    for(int i=0; i<80; i++)
    {	for(int j=0; j<31; j++)
		{	Buf=fgetc(in);
			if(j==30) break;
			CoordMatr[i][j]=Buf;
      }
    }
	for(int i=0; i<80; i++)
		for(int j=0; j<30; j++)
		{	if(CoordMatr[i][j]=='1')
			{	SetConsoleTextAttribute(consoleOutput, NULL);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf(" ");
				continue;
			}
			if(CoordMatr[i][j]=='2')
			{	SetConsoleTextAttribute(consoleOutput,BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",176);
				continue;
			}
			if(CoordMatr[i][j]=='3')
			{	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",201);
				continue;
			}
			if(CoordMatr[i][j]=='4')
			{	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",187);
				continue;
			}
			if(CoordMatr[i][j]=='5')
			{	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",199);
				continue;
			}
			if(CoordMatr[i][j]=='6')
			{	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",182);
				continue;
			}
			if(CoordMatr[i][j]=='7')
			{	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",210);
				continue;
			}
			if(CoordMatr[i][j]=='8')
			{	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",221);
				continue;
			}
			if(CoordMatr[i][j]=='9')
			{	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",222);
				continue;
			}
			if(CoordMatr[i][j]=='a')
			{	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",220);
				continue;
			}
			if(CoordMatr[i][j]=='b')
			{	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("/");
				continue;
			}
			if(CoordMatr[i][j]=='c')
			{	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("\\");
				continue;
			}
			if(CoordMatr[i][j]=='d')
			{	SetConsoleTextAttribute(consoleOutput, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",219);
				continue;
			}
			if(CoordMatr[i][j]=='e')
			{	SetConsoleTextAttribute(consoleOutput, FOREGROUND_INTENSITY | FOREGROUND_GREEN | BACKGROUND_GREEN);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",5);
				continue;
			}
			if(CoordMatr[i][j]=='f')
			{	SetConsoleTextAttribute(consoleOutput,FOREGROUND_INTENSITY | FOREGROUND_GREEN | BACKGROUND_GREEN);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",6);
				continue;
			}
			if(CoordMatr[i][j]=='g')
			{	SetConsoleTextAttribute(consoleOutput,FOREGROUND_INTENSITY | FOREGROUND_GREEN | BACKGROUND_GREEN);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",177);
				continue;
			}
			if(CoordMatr[i][j]=='h')
			{	SetConsoleTextAttribute(consoleOutput,FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_GREEN);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",179);
				continue;
			}
			if(CoordMatr[i][j]=='i')
			{	SetConsoleTextAttribute(consoleOutput,FOREGROUND_GREEN | BACKGROUND_GREEN);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",22);
				continue;
			}
			if(CoordMatr[i][j]=='j')
			{	SetConsoleTextAttribute(consoleOutput,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",178);
				continue;
			}
			if(CoordMatr[i][j]=='k')
			{	SetConsoleTextAttribute(consoleOutput,FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",219);
				continue;
			}
			if(CoordMatr[i][j]=='l')
			{	SetConsoleTextAttribute(consoleOutput,BACKGROUND_RED);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("%c",197);
				continue;
			}
			if(CoordMatr[i][j]=='m')
			{	SetConsoleTextAttribute(consoleOutput,BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf("^");
				continue;
			}
			if(CoordMatr[i][j]=='n')
			{	SetConsoleTextAttribute(consoleOutput,BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_INTENSITY);
				cursorPos.X=i;
				cursorPos.Y=j;
				SetConsoleCursorPosition(consoleOutput,cursorPos);
				printf(".");
				continue;
			}
		}
}

void Step(char Direction)
{	COORD NextPosition;
	DWORD cbRead;
	char *Buf=new char();
	HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	if(Direction==UP || Direction==DOWN || Direction==RIGHT || Direction==LEFT)
	{	if(Direction==UP)
		{	NextPosition.Y=PlInf.PlayerPos.Y-1;
			NextPosition.X=PlInf.PlayerPos.X;
		}
		else 
		{	if(Direction==DOWN)
			{	NextPosition.Y=PlInf.PlayerPos.Y+1;
				NextPosition.X=PlInf.PlayerPos.X;
			}
			else
			{	if(Direction==RIGHT)
				{	NextPosition.Y=PlInf.PlayerPos.Y;
					NextPosition.X=PlInf.PlayerPos.X+1;
				}
				else
				{	NextPosition.Y=PlInf.PlayerPos.Y;
					NextPosition.X=PlInf.PlayerPos.X-1;
				}
			}
		}
		ReadConsoleOutputCharacterW(consoleOutput, (LPWSTR)Buf, 1, NextPosition, &cbRead);
		if(*Buf==' ' || *Buf==-120 || *Buf==-111 || *Buf==-109 || *Buf==60)
		{	if(PlInf.Symb==' ')
				SetConsoleTextAttribute(consoleOutput,NULL);
			else 
			{	if(PlInf.Symb==176)
					SetConsoleTextAttribute(consoleOutput,BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				else 
				{	if(PlInf.Symb==178)
						SetConsoleTextAttribute(consoleOutput, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					else
					{	if(PlInf.PlayerPos.Y>3)
							SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						else
							SetConsoleTextAttribute(consoleOutput, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
					}
				}
			}
			SetConsoleCursorPosition(consoleOutput,PlInf.PlayerPos);
			putchar(PlInf.Symb);
			SetConsoleCursorPosition(consoleOutput,NextPosition);
			switch(*Buf)
			{	case ' '  :
				{	SetConsoleTextAttribute(consoleOutput, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
					PlInf.Symb=' ';
					char Buff[40]="play ";
					char Dir[30]="://Console/Step1.wav";
					strcat_s(Buff,Disc);
					strcat_s(Buff,Dir);
					mciSendStringA(Buff, NULL, 0, NULL);
					break;
				}
				case -120 :
				{	if(NextPosition.Y>3)
						SetConsoleTextAttribute(consoleOutput, BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
					else
						SetConsoleTextAttribute(consoleOutput, BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
					PlInf.Symb=219;
					char Buff[40]="play ";
					char Dir[30]="://Console/Step2.wav";
					strcat_s(Buff,Disc);
					strcat_s(Buff,Dir);
					mciSendStringA(Buff, NULL, 0, NULL);
					break;
				}
				case 60:
				{	if(NextPosition.Y>3)
						SetConsoleTextAttribute(consoleOutput, BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
					else
						SetConsoleTextAttribute(consoleOutput, BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
					PlInf.Symb=219;
					char Buff[40]="play ";
					char Dir[30]="://Console/CLICK.wav";
					strcat_s(Buff,Disc);
					strcat_s(Buff,Dir);
					mciSendStringA(Buff, NULL, 0, NULL);
					break;
				}
				case -111 :
				{	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
					PlInf.Symb=176;
					char Buff[40]="play ";
					char Dir[30]="://Console/Swim.wav";
					strcat_s(Buff,Disc);
					strcat_s(Buff,Dir);
					mciSendStringA(Buff, NULL, 0, NULL);
					break;
				}
				case -109 :
				{	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
					PlInf.Symb=178;
					char Buff[40]="play ";
					char Dir[30]="://Console/Step1.wav";
					strcat_s(Buff,Disc);
					strcat_s(Buff,Dir);
					mciSendStringA(Buff, NULL, 0, NULL);
					break;
				}
			}
			putchar(2);
			PlInf.OldPos=PlInf.PlayerPos;
			PlInf.PlayerPos=NextPosition;	
			return;
		}
		if(*Buf=='.')
		{	DWORD PlayersDeathThread;
			HANDLE PlayersDeathHandle=CreateThread(0, 0, PlayersDeath, 0, 0, &PlayersDeathThread);
			LeaveCriticalSection(&cs);
			WaitForSingleObject(PlayersDeathHandle, 70000);
 			return;
		}
		return;
	}
	return;
}

DWORD WINAPI Events(void *lpPar)
{	COORD cursorBuffer;
	HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD FountainWatherThread;
	DWORD LavaTrapsThread;
	HANDLE LavaTrapsHandle=CreateThread(0, 0, LavaTraps, 0, 0, &LavaTrapsThread);
	cursorBuffer.X=11;
	cursorBuffer.Y=5;
	EnterCriticalSection( &cs);
	SetConsoleTextAttribute(consoleOutput, BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	SetConsoleCursorPosition(consoleOutput,cursorBuffer);
	printf("%c",15);
	cursorBuffer.X=64;
	cursorBuffer.Y=26;
	SetConsoleTextAttribute(consoleOutput, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	SetConsoleCursorPosition(consoleOutput,cursorBuffer);
	printf("%c",15);
	LeaveCriticalSection( &cs);
	for(;GAME_STAGE_INDEX==STAGE_ONE;)
	{
		while (MoveList.empty() && GAME_STAGE_INDEX == STAGE_ONE)
		{
			Sleep(100);
		}
		cursorBuffer=MoveList.front();
		MoveList.pop();
		if(cursorBuffer.X==18 && cursorBuffer.Y==15)
		{	DWORD FountainThread;
			HANDLE FountainHandle;
			if(Fount==OFF)
			{		lpParametr Para;
					Para.x=18;
					Para.y=15;
					Para.symb=(char)176;
				FountainHandle=CreateThread(0, 0, Fountain, &Para, 0, &FountainThread);
			}
			Fount=!Fount;
		}
		if(cursorBuffer.X>12 && cursorBuffer.X<25 && cursorBuffer.Y>5 && cursorBuffer.Y<24)
		{	InZoneFount=YES;
			if(Fount==ON && FMThr!=ON)
			{	HANDLE FountainWatherHandle=CreateThread(0, 0, FountainWatherSound, 0, 0, &FountainWatherThread);
				FMThr=ON;
			}
		}
		else
			InZoneFount=NO;
		if(cursorBuffer.X>31 && cursorBuffer.X<38 && cursorBuffer.Y>17 && cursorBuffer.Y<25)
		{	if(Cem==NO)
			{	DWORD CemeteryThread;
				HANDLE CemeteryHandle=CreateThread(0, 0, Cemetery, 0, 0, &CemeteryThread);
				Cem=YES;
			}
		}
		else 
		{	if(Cem==YES)
			{	Cem=NO;
				if(BM!=CemeteryJacson)
				{	BM=CemeteryJacson;
					Sleep(50);
					BM=StdSonic_1;
					DWORD MusicThread;
					HANDLE MusicHandle=CreateThread(0, 0, Stage1MusicThread, 0, 0, &MusicThread);
				}
			}
		}
	}
	ExitThread(0);
}

void Menu()
{	setlocale(LC_ALL, "Russian");
	DWORD SoundThread;
	HANDLE SoundHandle=CreateThread(0, 0, MenuMusicThread, NULL, 0, &SoundThread);
	BM=MenuMario;
	HANDLE consoleOutput=GetStdHandle(STD_OUTPUT_HANDLE); 
	COORD cursorPos;
	SetConsoleTextAttribute(consoleOutput, NULL);
	int i=0;
	while( i<(80*30) )
	{	printf(" ");
		i++;
	}
	cursorPos.X=65;
	cursorPos.Y=29;
	SetConsoleCursorPosition(consoleOutput, cursorPos);
	SetConsoleTextAttribute(consoleOutput, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	puts("версия: 0.00");
	SetConsoleTextAttribute(consoleOutput, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	int count=6;
	while(count<24)
	{	cursorPos.X=20;
		cursorPos.Y=count;
		SetConsoleCursorPosition(consoleOutput, cursorPos);
		int i=0;
		while(i<40)
		{	printf(" ");
			i++;
		}
		count++;
	}
	cursorPos.X=38;
	cursorPos.Y=13;
	SetConsoleCursorPosition(consoleOutput, cursorPos);
	puts("ИГРАТЬ");
	cursorPos.X=38;
	cursorPos.Y=15;
	SetConsoleCursorPosition(consoleOutput, cursorPos);
	puts("ВЫХОД");
	cursorPos.X=0;
	cursorPos.Y=0;
	SetConsoleCursorPosition(consoleOutput, cursorPos);
	DWORD Thread1, Thread2, BackgroundThread;
	HANDLE Handle1, Handle2, BackgroundHandle;
	BackgroundHandle=CreateThread(0,0,Background,NULL,NULL,&BackgroundThread);
	lpParametr *stdPar1 = new lpParametr;
					stdPar1->x=37;
					stdPar1->y=13;
					stdPar1->symb=0020;
	lpParametr *stdPar2 = new lpParametr;
					stdPar2->x=44;
					stdPar2->y=13;
					stdPar2->symb=0021;
	lpParametr *Par1 = new lpParametr;
					Par1->x=37;
					Par1->y=15;
					Par1->symb=0020;
	lpParametr *Par2 = new lpParametr;
					Par2->x=43;
					Par2->y=15;
					Par2->symb=0021;
	Handle1=CreateThread(0,0,ThreadBlink,(void*)stdPar1,CREATE_SUSPENDED, &Thread1);
	Handle2=CreateThread(0,0,ThreadBlink,(void*)stdPar2,CREATE_SUSPENDED, &Thread2);
	ResumeThread(Handle1);
	ResumeThread(Handle2);
	char Key;
	int* Menu=new int[MENU_BUTTON];
	Menu[0]=1;
	for(int i=1; i<MENU_BUTTON; i++)
		Menu[i]=0;
	do
	{	cursorPos.X=0;
		cursorPos.Y=0;
		SetConsoleCursorPosition(consoleOutput, cursorPos);
		SetConsoleTextAttribute( consoleOutput, NULL);
		Key=_getch();
		SetConsoleTextAttribute( consoleOutput, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		switch(Key)
		{	case UP:	
			{	char Buf[40]="play ";
				char Dir[30]="://Console/click1.wav";
				strcat_s(Buf,Disc);
				strcat_s(Buf,Dir);
				mciSendStringA(Buf, NULL, 0, NULL);
				TerminateThread(Handle1,0);
				TerminateThread(Handle2,0);
				Handle1=CreateThread(0, 0, ThreadBlink, (void*)stdPar1, CREATE_SUSPENDED, &Thread1);
				Handle2=CreateThread(0, 0, ThreadBlink, (void*)stdPar2, CREATE_SUSPENDED, &Thread2);
				EnterCriticalSection( &cs);
				SetConsoleTextAttribute(consoleOutput, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
				cursorPos.X=37;
				cursorPos.Y=15;
				SetConsoleCursorPosition(consoleOutput, cursorPos);
				puts(" ");
				cursorPos.X=43;
				SetConsoleCursorPosition(consoleOutput, cursorPos);
				puts(" ");
				LeaveCriticalSection( &cs);
				ResumeThread(Handle1);
				ResumeThread(Handle2);
				Menu=MenuFunction(Menu,Key);
				break;
			}
			case DOWN:
			{	char Buf[40]="play ";
				char Dir[30]="://Console/click2.wav";
				strcat_s(Buf,Disc);
				strcat_s(Buf,Dir);
				mciSendStringA(Buf, NULL, 0, NULL);
				TerminateThread(Handle1,0);
				TerminateThread(Handle2,0);
				Handle1 = CreateThread(0, 0, ThreadBlink, (void*)Par1, CREATE_SUSPENDED, &Thread1);
				Handle2 = CreateThread(0, 0, ThreadBlink, (void*)Par2, CREATE_SUSPENDED, &Thread2);
				EnterCriticalSection( &cs );
				SetConsoleTextAttribute(consoleOutput, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
				cursorPos.X=37;
				cursorPos.Y=13;
				SetConsoleCursorPosition(consoleOutput, cursorPos);
				puts(" ");
				cursorPos.X=44;
				SetConsoleCursorPosition(consoleOutput, cursorPos);
				puts(" ");
				LeaveCriticalSection( &cs );
				ResumeThread(Handle1);
				ResumeThread(Handle2);
				Menu=MenuFunction(Menu,Key);
				break;
			}
			case 13 :
			{	if(Menu[0]==1)
				{	GAME_STAGE_INDEX=STAGE_ONE;
					SetConsoleTextAttribute(consoleOutput, NULL);
					cursorPos.X=0;
					cursorPos.Y=0;
					SetConsoleCursorPosition(consoleOutput, cursorPos);
					int i=0;
					while( i<(80*30) )
					{	printf(" ");
						i++;
					}
					return;
				}
				if(Menu[1]==1)
				{	TerminateThread(Handle1,0);
					TerminateThread(Handle2,0);
					TerminateThread(Background,0);
					TerminateThread(MenuMusicThread,0);
					exit(0);
				}
			}
		}
	}
	while(Key!=ESC);
	TerminateThread(Handle1,0);
	TerminateThread(Handle2,0);
	TerminateThread(Background,0);
	TerminateThread(MenuMusicThread,0);
	exit(1);
}

void Stage1()
{	HANDLE consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursorPos;
	BuildFirstStageMap();
	GenericLava();
	second_chance:
	DWORD GlobalEventsThread;
	HANDLE GrobalEventsHandle=CreateThread(0, 0, Events, 0, 0, &GlobalEventsThread);
	GAME_STAGE_INDEX=STAGE_ONE;
	DWORD MusicThread;
	HANDLE MusicHandle=CreateThread(0, 0, Stage1MusicThread, NULL, NULL, &MusicThread);
	BM=StdSonic_1;
	DWORD LavaThread;
	HANDLE LavaHandle=CreateThread(0, 0, LavaBlink, NULL, NULL, &LavaThread);
	EnterCriticalSection( &cs);
	cursorPos.X=1;
	cursorPos.Y=14;
	SetConsoleCursorPosition(consoleOutput,cursorPos);
	SetConsoleTextAttribute(consoleOutput, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	putchar(2);
	LeaveCriticalSection( &cs);
	PlInf.PlayerPos=cursorPos;
	PlInf.OldPos=cursorPos;
	PlInf.Symb=' ';
	char Key;
	do
	{	cursorPos.X=0;
		cursorPos.Y=0;
		SetConsoleCursorPosition(consoleOutput, cursorPos);
		Key=_getch();
		if(Key==UP || Key==DOWN || Key==RIGHT || Key==LEFT)
		{	MoveList.push(PlInf.PlayerPos);
			EnterCriticalSection( &cs);
			Step(Key);
			LeaveCriticalSection( &cs);
		}
	}
	while(Key!=ESC && GAME_STAGE_INDEX!=SECOND_CHANCE);
	if(GAME_STAGE_INDEX==SECOND_CHANCE)
		goto second_chance;
}

void main()

{	CONSOLE_CURSOR_INFO CCI;
	CCI.bVisible=false;
    CCI.dwSize=1;
	InitializeCriticalSection( &cs );
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CCI);
	Menu();
	Stage1();
}