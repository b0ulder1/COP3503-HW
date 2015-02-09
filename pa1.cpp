#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <crtdbg.h>
#define CRTDBG_MAP_ALLOC
using namespace std;

int main() {

cout << "Enter length of square: ";
int length;
cin >> length;

//Initializing the square
int** square = new int*[length]; 
for (int row = 0; row < length; row++){
	square[row] = new int[length];
for (int col = 0; col < length; col++){
	square[row][col] = 0;
}}

//Populate Square
int row = 0, column = length/2;
int num = 1, finish = length*length + 1;

while (num != finish){
	row == -1 ? row = length - 1 : row = row;
	row == length ? row = 0 : row = row;
	column == -1 ? column = length-1 : column = column;
	column == length ? column = 0 : column = column;
	int nextc = column + 1;
	nextc == -1 ? nextc = length - 1 : nextc = nextc;
	nextc == length ? nextc = 0 : nextc = nextc;
	int nextr = row - 1;
	nextr == -1 ? nextr = length - 1 : nextr = nextr;
	nextr == length ? nextr = 0 : nextr = nextr;
	if (square[row][column] == 0){
		square[row][column] = num;
		num++;
	}
	if (square[nextr][nextc] == 0){
		row--; column++;
	}
	else
		row++;
}

//Printing the square
const char separator = ' ';
const int numWidth = 4;
for (int r = 0; r < length; r++){
	for (int c = 0; c < length; c++){
		if (c != length - 1)
			cout << right << setw(numWidth) << setfill(separator) << square[r][c];
		else
			cout << right << setw(numWidth) << setfill(separator) << square[r][c] << "\n";
	}
}

//Check Sums
cout << "Checking the sums of every row: ";
int sum = 0;
for (int row1 = 0; row1 < length; row1++){
	for (int col1 = 0; col1 < length; col1++){
		sum = sum + square[row1][col1];
		if (col1 == length - 1){
			cout << sum << " ";
			sum = 0;
		}
	}
}
cout << "\nChecking the sums of every column: ";
for (int col2 = 0; col2 < length; col2++){
	for (int row2 = 0; row2 < length; row2++){
		sum += square[row2][col2];
		if (row2 == length - 1){
			cout << sum << " ";
			sum = 0;
		}
	}
}
cout << "\nChecking the sums of every diagonal: ";
//Main Diagonal
for (int diagr = 0, diagc = 0; diagr < length; diagr++, diagc++){
	sum += square[diagr][diagc];
	if (diagr == length - 1){
		cout << sum << " ";
		sum = 0;
	}
}
//Off Diagonal
for (int diagr1 = 0, diagc1 = length - 1; diagr1 < length; diagr1++, diagc1--){
	sum += square[diagr1][diagc1];
	if (diagr1 == length - 1){
		cout << sum << " ";
		sum = 0;
	}
}

delete square;
_CrtDumpMemoryLeaks();
}
