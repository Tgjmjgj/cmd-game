#include "stdafx.h"

List* CreateList()
{	List *Ret=new List;
	return Ret;
}

bool List::Delete()
{	Node *Buf=Head;
	if(Head!=NULL)
	{	if(Head->Next==NULL)
		{	delete Buf;
			Head=NULL;
			return true;
		}
		while(Buf->Next->Next!=NULL)
			Buf=Buf->Next;
		delete Buf->Next;
		Buf->Next=NULL;
		return true;
	}
	return false;
}

bool List::Insert(COORD New)
{	Node *Buf=Head;
	if(Buf==NULL)
	{	Head=new Node(New,NULL);
		return true;
	}
	else
	{	while(Buf->Next!=NULL)
			Buf=Buf->Next;
		Buf->Next=new Node(New,NULL);
		return true;
	}
}
   
    

