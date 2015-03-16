#include "pa2.h"

LinkedList::LinkedList(){ // Default Constructor
	head = tail = temp = NULL;
}

LinkedList::LinkedList(int cap, int ind){ //Creates freeList
	head = new Node(cap);
	tail = head;
	head->value = ind;
	head->name = "FREE";
}

LinkedList::~LinkedList(){ //Destructor
	clear();
}

void LinkedList::clear(){ // Clears LinkedList object
	Node *cur = head;
	Node *next;
	while (cur != NULL){
		next = cur->next;
		delete cur;
		cur = next;
	}
	head = NULL;
}

int getSize(int n){ // Input: size in KB, Output: size in pages
	if (n <= 0){
		cout << "Invalid Input - Size must be a positive number!\n";
		return 0;
	}
	if (n > 128){
		cout << "Invalid Input - Size is too big!\n";
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

int LinkedList::getFrag(){ // Returns the external fragmentation (number of nodes in freeList)
	int count = 0;
	Node * cur = head;
	while (cur != NULL){
		count++;
		cur = cur->next;
	}
	return count;
}

int LinkedList::getIndexWorst(int pgSize){ // Uses worst fit algorithm to return the index where the program should be placed
	Node * cur = head;
	Node * max = head;
	while (cur->next != NULL){
		cur = cur->next;
		if (max->pgCap >= pgSize){
			if (cur->pgCap > max->pgCap  && cur->pgCap >= pgSize){
				max = cur;
			}
		}
		else{
			if (cur->pgCap >= pgSize){
				max = cur;
			}
		}
	}
	int toReturn = max->value;//Index to add program
	if (max->pgCap < pgSize){
		cout << "Not enough free space to add program!" << endl;
		return 0;
	}
	
	if (max->pgCap == pgSize){
		if (max->prev != NULL){
			temp = max->prev;
			temp->next = max->next;
			if (max->next != NULL){
				max->next->prev = temp;
			}
			if (max == tail){
				tail = temp;
			}
			delete max;
			return toReturn;
		}
		else if (max->next != NULL){
			temp = max->next;
			temp->prev = max->prev;
			if (max->prev != NULL){
				max->prev->next = temp;
			}
			if (max == head){
				head = temp;
			}
			delete max;
			return toReturn;
		}
		else{
			delete max;
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
		max->value += pgSize;
		max->pgCap -= pgSize;
		return toReturn;
	}
}

int LinkedList::getIndexBest(int pgSize){ // Uses best fit algorithm to return the index where the program should be placed
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
	
	if (min->pgCap == pgSize){ //Deletes node
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
			cout << "Memory will be filled!\n" << endl;
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

void LinkedList::addNode(int index, int pSize, string name){ //Adds programs to a specific location in usedList
	if (index == 0){
		return;
	}
	for (int i = index; i < index + pSize; i++){
		Node *newNode = new Node();
		newNode->value = i;
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
			while (cur != NULL && cur->next && cur->next->value < i){
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
	cout << "Program " << name << " added successfully, " << pSize << " page(s) used\n" << endl;
}

void LinkedList::delProg(string progName, int pages){ //Deletes programs of the specified name
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
				head->prev = NULL;
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
	cout << "Program " << progName << " successfully killed, " << pages << " page(s) reclaimed\n" << endl;
}

void LinkedList::consolidate(int ind, int numToDel){ //Adds memory back to freeList, and defragments the nodes
	Node * cur = head;
	bool iso = true;
	while (cur->value < ind && cur->next != NULL){
		cur = cur->next;
	}
	//Check all possibilities
	if (cur->prev){
		Node * p = cur->prev;
		if (ind < cur->value){
			if (p->value + p->pgCap == ind){
				p->pgCap += numToDel;
				iso = false;
			}
			else if (ind + numToDel == cur->value){
				cur->value = ind;
				cur->pgCap += numToDel;
				iso = false;
			}
			if (cur->value == p->value + p->pgCap){
				p->pgCap += cur->pgCap;
				if (cur->next){
					Node * n = cur->next;
					p->next = n;
					n->prev = p;
					delete cur;
				}
				else{
					p->next = NULL;
					delete cur;
				}
			}
		}
		else if (cur->value + cur->pgCap == ind){
			cur->pgCap += numToDel;
			iso = false;
		}

	}
	else{ //Only one node
		if (cur->value == ind + numToDel){
			cur->value = ind;
			cur->pgCap += numToDel;
			iso = false;
		}
		else if (ind == cur->value + cur->pgCap){
			cur->pgCap += numToDel;
			iso = false;
		}
	}	
	if (iso){
		Node * newNode = new Node;
		newNode->value = ind;
		newNode->pgCap = numToDel;
		newNode->name = "FREE";
		if (cur->value < ind){
			newNode->prev = cur;
			if (cur->next){
				Node * n = cur->next;
				cur->next = newNode;
				newNode->next = n;
				n->prev = newNode;
			}
			else{
				cur->next = newNode;
				newNode->prev = cur;
				tail = newNode;
			}
		}
		else{
			newNode->next = cur;
			if (cur->prev){
				Node * p = cur->prev;
				p->next = newNode;
				newNode->prev = p;
				cur->prev = newNode;
			}
			else{
				cur->prev = newNode;
				head = newNode;
			}
			
		}
	}
}

void LinkedList::traverse_and_print(){
	const char fill = ' ';
	const int width = 10;

	Node *cur = head;
	if (head == NULL){
		for (int i = 1; i <= 32; i++){
			cout << left << setw(width) << setfill(fill) << "FREE";
		}
		return;
	}

	cout << "MEMORY: \n" << endl;

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

int LinkedList::getVal(string prog){ // Returns the starting index of the chain of programs of the same name
	Node * cur = head;
	while (cur->name != prog){
		cur = cur->next;
	}
	return cur->value;
}

int LinkedList::checkNum(string progName){ //Returns the number of programs to be deleted
	if (head == NULL){
		cout << "Program '" << progName << "' does not exist!\n" << endl;
		return 0;
	}
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
		cout << "Program " << progName << " does not exist!\n" << endl;
	}
	return count;
}

void LinkedList::newList(int cap, int ind){ //Re-creates the freeList after all memory is used and a program is deleted
	head->value = ind;
	head->pgCap = cap;
	head = tail;
	head->next = NULL;
	head->prev = NULL;
}

bool LinkedList::alreadyRunning(string name){ //Checks if a program is already running
	bool toReturn = false;
	Node * cur = head;
	while (cur != NULL){
		if (cur->name == name){
			toReturn = true;
		}
		cur = cur->next;
	}
	return toReturn;
} 

int main(int argc, char**argv){
	if (argc != 2 || (strcmp(argv[1], "best") != 0 && strcmp(argv[1], "worst") != 0)){
	return 0;
	}
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
			if (isFreeSpace){ //If there is free space
				cout << "\nProgram Name: ";
				cin >> pName;
				cout << "\nProgram Size (KB): ";
				cin >> kbSize;
				pSize = getSize(kbSize); //Size in pages
				if (pSize != 0){
					bool running = usedList.alreadyRunning(pName); //Checks if already running
					if (running){
						cout << "Program '" << pName << "' is already running!\n" << endl;
					}
					else{ // uses specified algorithm
						int addIndex;
						if (strcmp(argv[1], "best") != 0){
							addIndex = freeList.getIndexWorst(pSize);
						}
						else
							addIndex = freeList.getIndexBest(pSize);
						usedList.addNode(addIndex, pSize, pName);
					}
				}
			}
			else
				cout << "No free space left - cannot add program\n" << endl;
		}

		if (choice == 2){
			cout << "Program Name: ";
			cin >> pName;
			int numToDel = usedList.checkNum(pName);
			if (numToDel > 0){
				int ind = usedList.getVal(pName); // Index of first program in program block to delete
				usedList.delProg(pName, numToDel);
				if (!isFreeSpace){ // If there was no free space prior to deleting, create new freeList
					freeList.newList(numToDel, ind); // with these parameters
					isFreeSpace = true; // Memory now has free space
				}
				else
					freeList.consolidate(ind, numToDel); // If memory already had some free space
			}
		}

		if (choice == 3){
			if (!isFreeSpace){
				cout << "There are no fragments - Memory is full\n" << endl;
			}
			else{
				cout << "There are " << freeList.getFrag() << " external fragments\n" << endl;
			}
		}

		if (choice == 4){
			usedList.traverse_and_print();
		}
	}
	usedList.clear();
	return 0;
}
