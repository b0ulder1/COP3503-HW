#include <iostream>
#include <cstddef>
#include <string>
#include<iomanip>
#include<cstring>
#include <stdlib.h>

using namespace std;

int intFrag = 0;
bool isFreeSpace = true;

class Node{
	friend class LinkedList;
private:

	int value;
	Node * next;
	Node * prev;
	string name;
	int pgCap;

public:

	Node(void)
		: next(NULL), prev(NULL)
	{ }

	Node(int cap)
		: pgCap(cap), next(NULL), prev(NULL)
	{ }

	Node(int val, string name)
		: value(val), next(NULL), prev(NULL), name(name)
	{ }

	int getvalue(void){
		return value;
	}

	string getName(void){
		return name;
	}

	Node* getNext(void){
		return next;
	}

	Node* getPrev(void){
		return prev;
	}

	void append(int n);

	void decreaseValue();

	void increaseValue();

};

class LinkedList{
	friend class Node;
private:
	Node * head;
	Node * tail;
	Node * temp;
public:
	LinkedList(void);

	LinkedList(int val, int ind);

	~LinkedList(void); //Destructor

	void traverse_and_print();

	void addNode(int n, int pSize, string name);

	void delProg(string prog, int pages);

	int checkNum(string progName);

	int getVal(string prog);

	int getIndexBest(int pgSize);

	int getIndexWorst(int pgSize);

	void consolidate(int ind, int numToDel);

	void newList(int cap, int ind);

	int getFrag();

	void clear();

	bool alreadyRunning(string name);
};
