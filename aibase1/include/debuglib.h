#ifndef MYDEBUGLIB
#define MYDEBUGLIB


#include "Logger.h"
#include "Assert.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>


//  Set up constants to allow us to switch from runtime to debugtime
// builds easily
#ifdef DEBUGVERSION
#define NewMalloc LibMalloc
#define NewFree LibFree
#define EnterFunction EnterFunc
#define LeaveFunction LeaveFunc
#define InitDebugLib InitDebugLibOn
#define Delete LibDelete
#define New LibNew
#else
#define NewMalloc WrapperMalloc
#define NewFree WrapperFree
#define EnterFunction WrapperEnterFunc
#define LeaveFunction WrapperLeaveFunc
#define InitDebugLib InitDebugLibOff
#define Delete WrapperDelete
#define New WrapperNew
#endif

#ifndef NULL
#define NULL 0
#endif

#define VOIDPTR void*&



//generic linked list class, used in multiple places
template<class T>
class LibLinkedList{
	class Node{
	public:
		T data;
		Node* next;
		Node* prev;
	};
	int size;
	Node *current,*head,*tail;
	void CopyAll(LibLinkedList&);
	void DeleteAll();
public:
	LibLinkedList();
	LibLinkedList(LibLinkedList&);
	LibLinkedList& operator= (LibLinkedList&);
	~LibLinkedList();
	int InsertBefore(T);
	int InsertBefore(T,int);
	int InsertAfter(T);
	int InsertAfter(T,int);
	int Remove();
	int Remove(int);
	int Get(T&);
	int Get(T&,int);
	void Goto(int);
	void First();
	void Last();
	void Next();
	void Prev();
	int Size();
};

//a class to hold cumulative information about a named ptr
class NamedStats{
	char* name;  //name of pointer
	int last;  //1 if last was an alloc, 0 if dealloc
	unsigned int numAllocs;   //number of times named ptr was allocated
	unsigned int numDeallocs;  //number of times it was deallocated
	unsigned int amountAllocated;  //amount of memory allocated in total
	void CopyAll(NamedStats&);  //copies a class to another instance
	void DeleteAll();  //deletes all dynamic memory
public:
	NamedStats(char*);  //constructors and Big 3
	NamedStats();
	NamedStats(NamedStats&);
	NamedStats& operator= (NamedStats&);
	~NamedStats();
	char* GetName();  //get functions
	unsigned int GetNumAllocs();
	unsigned int GetTotalAllocs();
	unsigned int GetNumDeallocs();
	int GetLast();
	void Dealloc(); //tells named location you have freed last alloc
	void Alloc(int);  //tells named location you have allocated it
};



//class to hold info about an allocated ptr
class PtrData{
	void* data;    //actual ptr allocated
	unsigned int size;   //amount allocated
	char *name;  
	void CopyAll(PtrData&);   //helper functions
	void DeleteAll();
public:
	PtrData(void*,int,char*);  //constructors and big 3
	PtrData();
	PtrData(PtrData&);
	PtrData& operator = (PtrData&);
	~PtrData();
	unsigned int GetSize();  //get functions
	void* GetData();
	char* GetName();
};


//our malloc functions.
int LibMalloc(unsigned int,VOIDPTR,char* name=NULL);
void* LibMalloc(unsigned int,char* name=NULL);
// wrapper func, should just call malloc
int WrapperMalloc(unsigned int,VOIDPTR,char* name=NULL);
void* WrapperMalloc(unsigned int,char* name=NULL);
//our free function
int LibFree(void*,char* name=NULL);
//wrapper for free
int WrapperFree(void*,char* name=NULL);
//starts debug library
#if 0
int InitDebugLib(FILE*,int);
int InitDebugLib(std::ostream*,int);
#endif
int InitDebugLib(void);
//free up lib resources
void FreeDebugLib();
//add a new function to the stack
void EnterFunc(char*);
//remove one from stack
void LeaveFunc();
//wrappers for above two
void WrapperEnterFunc(char* func);
void WrapperLeaveFunc();
//free all memory allocations not made by this library
void FreeAll();
//Make a comment in the debugging log
void NoteEvent(char*);

/////////////////////////////////////////////////////////////
extern LibLinkedList<NamedStats> NamedPtrData;  //list of all named ptrs and their stats
extern LibLinkedList<PtrData> AllocatedPtrs;  //a list of all allocated ptrs
extern LibLinkedList<char*> FunctionStack;  //names of all functions currently in.
//std::ostream *LogStream;  //stream to write all outputs to
//FILE *LogFileHandle;  //handle to write all file outputs to
extern int DebugLibOptions;  //the options selected in the debug library   Currently- -1= not set, 0=use FILE* 1=use stream
//int OutputType=-1;
extern unsigned int totalAllocation,numAllocations,numDeallocations;
////////////////////////////////////////////////////////////

//New.  hook for new to get around broken compilers
template <class T> T* LibNew(unsigned int num ,T* junk,char* name=NULL)
{
	int x;
	T* data=new T[num];
	if(data==NULL){  //error in allocation
		DL((ERROR, "调用operaotr new分配%d bytes内存失败.\n",num*sizeof(T)));
		return data;
	}
	numAllocations++;  //update total stats
	totalAllocation+=num*sizeof(T);
	if(name!=NULL){  //pointer is named
		NamedStats NamedData(name);
		for(x=0;x<NamedPtrData.Size();x++){  //check if it is a previously used name
			NamedPtrData.Get(NamedData,x);
			//if(stricmp(NamedData.GetName(),name)==0) //names match
			if(strcasecmp(NamedData.GetName(),name)==0) //names match
				break;
		}
		if(x==NamedPtrData.Size()){  //no match. init and add to list
			NamedStats insertStats(name);
			insertStats.Alloc(num*sizeof(T));
			NamedPtrData.InsertBefore(insertStats);
		}
		else{ //match
			if(NamedData.GetLast()==1){  //alloc after alloc may be a leak
				DL((ERROR, "[%s]指针未被释放而分配多次, 这将造成内存泄露.\n",name));
			}
			NamedData.Alloc(num*sizeof(T));  //update stats
			NamedPtrData.Remove(x);
			NamedPtrData.InsertBefore(NamedData);
		}
	}
	PtrData insertPtr(data,num*sizeof(T),name);  //add to big list of pointers
	AllocatedPtrs.InsertBefore(insertPtr);
	DL((ERROR, "指针[%s]分配内存(Size %d bytes).\n",name!=NULL?name:"",num*sizeof(T)));
	return data;
}

template <class T> T* LibNew(unsigned int num ,char* name=NULL)
{
	int x;
	T* data=new T[num];
	if(data==NULL){  //error in allocation
		DL((ERROR, "调用operaotr new分配%d bytes内存失败.\n",num*sizeof(T)));
		return data;
	}
	numAllocations++;  //update total stats
	totalAllocation+=num*sizeof(T);
	if(name!=NULL){  //pointer is named
		NamedStats NamedData(name);
		for(x=0;x<NamedPtrData.Size();x++){  //check if it is a previously used name
			NamedPtrData.Get(NamedData,x);
			//if(stricmp(NamedData.GetName(),name)==0) //names match
			if(strcasecmp(NamedData.GetName(),name)==0) //names match
				break;
		}
		if(x==NamedPtrData.Size()){  //no match. init and add to list
			NamedStats insertStats(name);
			insertStats.Alloc(num*sizeof(T));
			NamedPtrData.InsertBefore(insertStats);
		}
		else{ //match
			if(NamedData.GetLast()==1){  //alloc after alloc may be a leak
				DL((ERROR, "[%s]指针未被释放而分配多次, 这将造成内存泄露.\n",name));
			}
			NamedData.Alloc(num*sizeof(T));  //update stats
			NamedPtrData.Remove(x);
			NamedPtrData.InsertBefore(NamedData);
		}
	}
	PtrData insertPtr(data,num*sizeof(T),name);  //add to big list of pointers
	AllocatedPtrs.InsertBefore(insertPtr);
	DL((ERROR, "指针[%s]分配内存(Size %d bytes).\n",name!=NULL?name:"",num*sizeof(T)));
	return data;
}

template <class T> T* WrapperNew(unsigned int num,T* junk,char* name=NULL)
{
	return new T[num];
}

template <class T> T* WrapperNew(unsigned int num,char* name=NULL)
{
	return new T[num];
}

//Delete function.  A hack to get delete calls to work.  Should be
//called before using the delete keyword.
template<class T> void LibDelete(T* ptr,char* name=NULL)
{
	unsigned int x;
	PtrData temp;
	if(ptr==NULL){  //free a null
		DL((APP,"释放了NULL指针\n"));
		return;
	}
	for(x=0;x< (unsigned int)AllocatedPtrs.Size();x++){ //get ptr data from list
		AllocatedPtrs.Get(temp,x);
		if(temp.GetData()==ptr)
			break;
	}
	if(x==(unsigned int)AllocatedPtrs.Size()){ //not in list, error
		DL((APP,"企图释放非new的指针[%s].\n",name!=NULL?name:""));
		return;
	}
	numDeallocations++;
	delete []ptr;
	AllocatedPtrs.Remove(x);
	if(name!=NULL){  //named pointer
		NamedStats tempStats(NULL);
		for(x=0;x<(unsigned int)NamedPtrData.Size();x++){ //find data with that name
			NamedPtrData.Get(tempStats,x);
			//if(stricmp(tempStats.GetName(),name)==0)
			if(strcasecmp(tempStats.GetName(),name)==0)
				break;
		}
		if(x==(unsigned int)NamedPtrData.Size()){   //no ptr withb that name.  Programmer typo or mismatch
			DL((APP,"指针[%s]从来未被分配过.\n",name!=NULL?name:""));
		}
		else{  //dealloc data
			tempStats.Dealloc();
			NamedPtrData.Remove(x);
			NamedPtrData.InsertBefore(tempStats);
		}

	}
	DL((APP,"指针[%s]被释放.\n",name!=NULL?name:""));
	return;
}

template<class T> void WrapperDelete(T* ptr,char*name=NULL)
{
	delete []ptr;
}

//show statistics for all memory names and the total program
void ShowDebugStatistics();
//shows statistics for a name
void ShowDebugStatistics(char*);


#endif

