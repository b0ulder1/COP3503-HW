#include <iostream>
#include <cstddef>
#include <string>
#include<iomanip>
#include<cstring>
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace std;

int intFrag = 0;
int extFrag = 0;
int masterIndex = 1;
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
		: next(NULL)
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

	void addNode(int n);

	void addNode(int n, int pSize, string name);

	void delProg(string prog, int pages);

	int checkNum(string progName);

	int getVal(string prog);

	bool atTail(string name);
	
	int getIndexBest(int pgSize);

	int getIndexWorst(int pgSize);

	void consolidate(int ind, int numToDel);

	void newList(int cap, int ind);

	void clear();

};

LinkedList::LinkedList(){
	head = tail = temp = NULL;
}

LinkedList::LinkedList(int cap, int ind){ //Creates freeList
	head = new Node(cap);
	tail = head;
	head->value = ind;
	head->name = "FREE";
}

LinkedList::~LinkedList(){
	clear();
}

void LinkedList::clear(){
	Node *cur = head;
	Node *next;
	while (cur != NULL){
		next = cur->next;
		delete cur;
		cur = next;
	}
	head = NULL;
}

int getSize(int n){
	if (n <= 0){
		cout << "Invalid Input - Size must be a positive number!";
		return 0;
	}
	if (n > 128){
		cout << "Invalid Input - Size is too big!";
		return 0;
	}

	else{
		int s = (n + 3) / 4;
		if (n % 4 == 0){
			return s;
		}
		else{
			intFrag += (4 - (n % 4));
			return s;
		}
	}
}

int LinkedList::getIndexWorst(int pgSize){ //Worst fit
	Node * cur = head;
	Node * max = head;
	while (cur->next != NULL){
		cur = cur->next;
		if (cur->pgCap > max->pgCap){
			max = cur;

		}
	}
	int toReturn = max->value;//Index to add program
	if (max->pgCap < pgSize){
		cout << "Not enough free space to add program!" << endl;
		return 0;
	}
	cout << "max index: " << max->value << endl;

	if (max->pgCap == pgSize){
		return toReturn;
		delete max;
	}
	else{
		max->value += pgSize;
		max->pgCap -= pgSize;
		return toReturn;
	}
}

int LinkedList::getIndexBest(int pgSize){ //Best fit
	Node * cur = head;
	Node * min = head;
	while (cur->next != NULL){
		cur = cur->next;
		if (min->pgCap >= pgSize){
			if (cur->pgCap < min->pgCap  && cur->pgCap >= pgSize){
				min = cur;
			}
		}
		else{
			if (cur->pgCap >= pgSize){
				min = cur;
			}
		}
	}
	
	int toReturn = min->value;//Index to add program
	if (min->pgCap < pgSize){
		cout << "Not enough free space to add program!" << endl;
		return 0;
	}
	cout << "min index: " << min->value << endl;

	if (min->pgCap == pgSize){
		if (min->prev != NULL){
			temp = min->prev;
			temp->next = min->next;
			if (min->next != NULL){
				min->next->prev = temp;
			}
			if (min == tail){
				tail = temp;
			}
			delete min;
			return toReturn;
		}
		else if (min->next != NULL){
			temp = min->next;
			temp->prev = min->prev;
			if (min->prev != NULL){
				min->prev->next = temp;
			}
			if (min == head){
				head = temp;
			}
			delete min;
			return toReturn;
		}
		else{
			delete min;
			head = new Node(0);
			tail = head;
			head->value = 0;
			head->name = "FREE";
			cout << "No free space left!" << endl;
			isFreeSpace = false;
			return toReturn;
		}
	}
	else{
		min->value += pgSize;
		min->pgCap -= pgSize;
		return toReturn;
	}
}

void LinkedList::addNode(int index, int pSize, string name){
	if (index == 0){
		return;
	}
	for (int i = index; i < index + pSize; i++){
		Node *newNode = new Node;
		newNode->value = i;
		newNode->next = NULL;
		newNode->prev = NULL;
		newNode->name = name;
		newNode->pgCap = pSize;
		Node *cur = head;

		if (head == NULL){ // If list is empty
			head = newNode;
			tail = newNode;
			head->value = i;
		}
		else if (head->value > i){
			cur->prev = newNode;
			newNode->next = cur;
			head = newNode;
		}
		else{
			while (cur != NULL && cur->next && cur->next->value < i){// cur->next OR cur->next != NULL ???
				cur = cur->next;
			}
				if (cur->next == NULL){ // At Tail
					cur->next = newNode;
					tail = newNode;
					newNode->next = NULL;
					newNode->prev = cur;
					cur = cur->next;
				}
				else{
					temp = cur->next;
					cur->next = newNode;
					newNode->prev = cur;
					newNode->next = temp;
					temp->prev = newNode;
				}
			
		}
	}
	
}

void LinkedList::delProg(string progName, int pages){
	for (int i = 0; i < pages; i++){
		Node * del = head;
		while (del->name != progName){
			del = del->next;
		}
		if (del == head){
			if (del->next == NULL){
				clear();
				LinkedList usedList;
				return;
			}
			else{
				head = del->next;
				delete del;
			}
		}
		else{
			Node * cur = del->prev;
			if (del->next != NULL){
				Node * nxt = del->next;
				cur->next = nxt;
				nxt->prev = cur;
			}
			else{
				cur->next = NULL;
				tail = cur;
			}
			delete del;
		}
	}
}

void LinkedList::consolidate(int ind, int numToDel){
	Node * cur = head;
	bool iso = true;
	while (cur->value < ind && cur->next != NULL){
		cur = cur->next;
	}
	if (cur->value == ind + numToDel){
		cur->value = ind;
		cur->pgCap += numToDel;
		iso = false;
	}
	if (cur->prev && cur->prev->value + cur->prev->pgCap == ind){
		if (cur->prev == head){
			cur->value = head->value;
			cur->pgCap += head->pgCap;
			head = cur;
			delete cur->prev;
		}
		else{
			Node * p = cur->prev->prev;
			cur->value = cur->prev->value;
			cur->pgCap += cur->prev->pgCap;
			p->next = cur;
			delete cur->prev;
			cur->prev = p;
		}
		iso = false;
	}
	if (cur->value + cur->pgCap == ind){
		cur->pgCap += numToDel;
		iso = false;
	}
	if (iso){
		Node * newNode = new Node;
		newNode->value = ind;
		newNode->pgCap = numToDel;
		newNode->name = "FREE";
		newNode->prev = cur->prev;
		cur->prev = newNode;
		newNode->next = cur;
		if (newNode->prev == NULL){
			head = newNode;
		}
	}
}

void LinkedList::traverse_and_print(){
	const char fill = ' ';
	const int width = 10;

	Node *cur = head;
	if (head == NULL){
		cout << "The List is Empty!" << endl;
		for (int i = 1; i <= 32; i++){
			cout << left << setw(width) << setfill(fill) << "FREE";
		}
		return;
	}

	cout << "MEMORY: " << "Tail: " << tail->name <<endl;

	for (int i = 1; i <= 32; i++){
		if (cur->value == i){
			cout << left << setw(width) << setfill(fill) <<
				cur->name;
			if (cur->next != NULL)
				cur = cur->next;
		}
		else
			cout << left << setw(width) << setfill(fill) <<
			"FREE";
	}
}

int LinkedList::getVal(string prog){
	Node * cur = head;
	while (cur->name != prog){
		cur = cur->next;
	}
	return cur->value;
}

int LinkedList::checkNum(string progName){
	Node * cur = head;
	int count = 0;
	while (cur != tail){
		if (cur->name == progName){
			count++;
		}
		cur = cur->next;
	}
	if (cur->name == progName){
		count++;
	}
	if (count == 0){
		cout << "Program " << progName << " does not exist!" << endl;
	}
	return count;
}

void LinkedList::newList(int cap, int ind){
	head->value = ind;
	head->pgCap = cap;
}

int main(int argc, char**argv){
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	/*
	if (argc != 2 || (argv[1] != "best" && argv[1] != "worst")){
		return;
	}
	*/
	LinkedList freeList(32, 1);
	LinkedList usedList;
	int choice, kbSize, pSize;
	string pName;

	while (cin){
		cout << "1 - Add Program" << endl
			<< "2 - Kill Program" << endl
			<< "3 - Fragmentation" << endl
			<< "4 - Print Memory" << endl
			<< "5 - Exit\n" << endl;
		cout << "Choice: ";
		cin >> choice;
		if (choice == 5){
			break;
		}

		if (choice == 1){
			if (isFreeSpace){
				cout << "\nProgram Name: ";
				cin >> pName;
				cout << "\nProgram Size (KB): ";
				cin >> kbSize;
				pSize = getSize(kbSize);
				int addIndex = freeList.getIndexBest(pSize);
				usedList.addNode(addIndex, pSize, pName);
			}
			else
				cout << "No free space left - cannot add program" << endl;
		}

		if (choice == 2){
			cout << "\nProgram Name: ";
			cin >> pName;
			int numToDel = usedList.checkNum(pName);
			if (numToDel > 0){
				int ind = usedList.getVal(pName);
				cout << "\n" << numToDel << " pages will be killed starting at index " << ind << endl;
				usedList.delProg(pName, numToDel);
				if (!isFreeSpace){
					freeList.newList(numToDel, ind);
					isFreeSpace = true;
				}
				else
					freeList.consolidate(ind, numToDel);
			}
		}
		if (choice == 4){
			usedList.traverse_and_print();
		}

			
	}
	usedList.clear();
	return 0;
	_CrtDumpMemoryLeaks();

}
