#include <iostream>
#include <cstddef>
#include <string>
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace std;

class Node{
	friend class LinkedList;
private:

	int _value;
	Node * next;
	string name;

public:

	Node(void)
		: next(NULL)
	{ }

	Node(int val)
		: _value(val), next(NULL)
	{ }

	Node(int val, string name)
		: _value(val), next(NULL), name(name)
	{ }

	Node(int val, Node* next)	//Not sure if this is used?
		: _value(val), next(next)
	{ }

	int getValue(void){
		return _value;
	}

	string getName(void){
		return name;
	}

	Node* getNext(void){
		return next;
	}

	void append(int n);

	void decreaseValue();

};

class LinkedList{
	friend class Node;
private:
	Node * head;
	Node * tail;
	Node * temp;
public:
	LinkedList(void);
	LinkedList(int val);
	~LinkedList(void); //Destructor

	void traverse_and_print();

	void addNode(int n);

	void addNode(int n, string name);

	void delNode(string prog, int num);

	int checkNum(string progName);

	void clear();

};

LinkedList::LinkedList(){
	head = tail = temp = NULL;
}

LinkedList::LinkedList(int val){	//for freeList?
	head = new Node(val);
	tail = head;
}

LinkedList::~LinkedList(){
	clear();
}

void LinkedList::clear(){
	Node *cur = head;
	while (cur != NULL){
		Node *next = cur->next;
		delete cur;
		cur = next;
	}
	head = NULL;
}

void LinkedList::addNode(int n){
	Node *newNode = new Node;
	newNode->_value = n;
	newNode->next = NULL;
	
	Node *cur = head;

	if (cur == NULL){
		head = newNode;
		tail = newNode;
		return;
	}
	else{
		while (cur){
			if (cur->next == NULL){
				cur->next = newNode;
				tail = cur->next;
				return;
			}
			cur = cur->next;
		}
	}
}

void LinkedList::addNode(int n, string name){
	Node *newNode = new Node;
	newNode->_value = n;
	newNode->next = NULL;
	newNode->name = name;

	Node *cur = head;
	if (head == NULL){
		head = newNode;
		tail = newNode;
		return;
	}
	else{
		while (cur){
			if (cur->next == NULL){
				cur->next = newNode;
				tail = cur->next;
				return;
			}
			cur = cur->next;
		}
	}
}

int getSize(int n){
	if (n <= 0){
		cout << "Invalid Input - Size must be a positive number!";
		return 0;
	}
	else{
		return (n + 3) / 4;
	}
	//find internal fragmentation -- store to global variable
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

void LinkedList::delNode(string prog, int num){
	Node * del = NULL;

	Node * cur = head;

	if (head->name == prog){ //Check head node
		del = head;
		head = del->next;
		delete del;
		return;
	}

	while (cur->next != NULL && cur->next->name != prog){
		cur = cur->next;
	}
	if (cur == tail){
		cout << "No such program exists!" << endl;
	}
	else{
		del = cur->next;
		if (del == tail){
			tail = cur;
		}
		temp = del->next;
		delete del;
		cur->next = temp;
	}
	cout << "Tail: " << tail->_value << endl;
}

void LinkedList::traverse_and_print(){
	Node *p = head;
	if (head == NULL){
		cout << "The List is Empty!" << endl;
		return;
	}
	cout << "MEMORY: " << endl;
	while (p != NULL){
		cout << "	" << p->name << "(" << p->_value << " KB)	";
		p = p->next;
	}
}

void Node::append(int n){
	for (int i = 1; i <= n; i++){
		cout << "	FREE	";
	}
}

void Node::decreaseValue(){
	--_value;
}

int main(){
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Node freeMem(32);
	cout << "Created node 'freeMem'" << endl;
	cout << "freeMem: " << freeMem.getValue() << " pages free." << endl;
	
	LinkedList usedList;
	cout << "Created list 'usedList'" << endl;
	
	int choice, kbSize, pSize;
	string pName;

	while (cin){
		cout << "1 - Add Program" << endl
			<< "2 - Kill Program" << endl
			<< "3 - Fragmentation" << endl
			<< "4 - Print Memory" << endl 
			<< "5 - Exit" << endl;
		cout << "Choice: ";
		cin >> choice;
		if (choice == 5){
			break;
		}

		if (choice == 1){
			cout << "\nProgram Name: ";
			cin >> pName;
			cout << "\nProgram Size (KB): ";
			cin >> kbSize;
			pSize = getSize(kbSize);
			for (int i = 0; i < pSize; i++){
				usedList.addNode(kbSize, pName);
				freeMem.decreaseValue();
			}

		}

		if (choice == 2){
			cout << "\nProgram Name: ";
			cin >> pName;
			int numToDel = usedList.checkNum(pName);
			cout << numToDel << " pages will be killed " << endl;
		}

		if (choice == 4){
			usedList.traverse_and_print();
			freeMem.append(freeMem.getValue());
		}
	}
	usedList.clear();
	return 0;
	_CrtDumpMemoryLeaks();

}

