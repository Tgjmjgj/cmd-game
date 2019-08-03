#pragma once
#pragma comment(lib, "winmm.lib")

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include <iomanip>
#include <ctime>
#include <mscoree.h>
#include <crtdbg.h>
#include <queue>

using namespace std;

#define UP 72
#define LEFT 75
#define DOWN 80
#define RIGHT 77
#define ESC 27

#define MENU_BUTTON 2

#define ON 1
#define OFF 0

#define YES 1
#define NO 0

enum GameStage
{	MENU,
	STAGE_ONE,
	DEATH,
	END,
	SECOND_CHANCE
};

enum BackgroundMusic
{	MenuMario,
	StdSonic_1,
	CemeteryJacson,
	Death
};

struct lpParametr
{	int x;
	int y;
	char symb;
};

struct PlayerInfo
{	COORD PlayerPos;
	COORD OldPos;
	wchar_t Symb;
};

struct Node
{	COORD Coord;
	Node *Next;
	Node(COORD cd, Node* n)
	{ Coord=cd; Next=n; };
};

struct List
{	Node *Head;
	List() {Head=NULL;};
	bool Delete();
	bool Insert(COORD New);
};

List* CreateList();