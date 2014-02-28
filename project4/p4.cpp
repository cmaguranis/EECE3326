// Declarations and functions for project #4

#include <iostream>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>
#include <vector>

using namespace std;

typedef int ValueType; // The type of the value in a cell
const int Blank = -1;  // Indicates that a cell is blank

const int SquareSize = 3;  //  The number of cells in a small square
//  (usually 3).  The board has
//  SquareSize^2 rows and SquareSize^2
//  columns.

const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

int numSolutions = 0;
int squareNumber(int i, int j);
int _atoi(char c) {
    return (c - '0');
}

class board
    // Stores the entire Sudoku board
{
public:
    board(int);
    void clear();
    void initialize(ifstream &fin);
    void print();
    bool isBlank(int, int);
    ValueType getCell(int, int);
    void setCell(int, int, ValueType);
    void printConflicts();
private:
    void initConflicts();
    bool checkConflict(int i, int j, ValueType v);
    // The following matrices go from 1 to BoardSize in each
    // dimension.  I.e. they are each (BoardSize+1) X (BoardSize+1)
    matrix<ValueType> value;

    vector< vector<bool> > rows;
    vector< vector<bool> > cols;
    vector< vector<bool> > sqs;

};

board::board(int sqSize)
    : value(BoardSize + 1, BoardSize + 1),
      rows(BoardSize + 1, vector<bool>(BoardSize + 1, 0)),
      cols(BoardSize + 1, vector<bool>(BoardSize + 1, 0)),
      sqs(BoardSize + 1,  vector<bool>(BoardSize + 1, 0))
      // Board constructor
{
}

void board::clear() {
}

void board::initialize(ifstream &fin)
// Read a Sudoku board from the input file.
{
    char ch;

    clear();
    for (int i = 1; i <= BoardSize; i++) {
        for (int j = 1; j <= BoardSize; j++) {
            fin >> ch;

            // If the read char is not Blank
            if (ch != '.') {
                setCell(i, j, _atoi(ch)); // Convert char to int
            }
        }
    }
}

int squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
    // Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
    // coordinates of the square that i,j is in.

    return SquareSize * ((i - 1) / SquareSize) + (j - 1) / SquareSize + 1;
}

ostream &operator<<(ostream &ostr, vector<int> &v)
// Overloaded output operator for vector class.
{
    for (int i = 0; i < v.size(); i++) {
        ostr << v[i] << " ";
    }
    ostr << endl;
    return ostr;
}

ValueType board::getCell(int i, int j)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
    if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize) {
        return value[i][j];
    } else {
        throw rangeError("bad value in getCell");
    }
}

void board::setCell(int i, int j, ValueType val)
// Set the value into a cell (i,j).  Throws an exception
// if bad values are passed.
{
    if (i >= 1 && i <= BoardSize &&
            j >= 1 && j <= BoardSize &&
            val >= 1 && val <= 9) {
        //if (checkConflict(i, j, val))
        rows[i].at(val) = true;
        cols[j].at(val) = true;
        sqs[squareNumber(i, j)].at(val) = true;

        value[i][j] = val;
    } else {
        throw rangeError("bad value in getCell");
    }
}

bool board::isBlank(int i, int j)
// Returns true if cell i,j is blank, and false otherwise.
{
    if (i < 1 || i > BoardSize || j < 1 || j > BoardSize) {
        throw rangeError("bad value in setCell");
    }
}

void board::print()
// Prints the current board.
{
    for (int i = 1; i <= BoardSize; i++) {
        if ((i - 1) % SquareSize == 0) {
            cout << " -";
            for (int j = 1; j <= BoardSize; j++) {
                cout << "---";
            }
            cout << "-";
            cout << endl;
        }
        for (int j = 1; j <= BoardSize; j++) {
            if ((j - 1) % SquareSize == 0) {
                cout << "|";
            }
            if (!isBlank(i, j)) {
                cout << " " << getCell(i, j) << " ";
            } else {
                cout << "   ";
            }
        }
        cout << "|";
        cout << endl;
    }

    cout << " -";
    for (int j = 1; j <= BoardSize; j++) {
        cout << "---";
    }
    cout << "-";
    cout << endl;
}

void board::printConflicts() {
    for (int i = 1; i <= BoardSize; i++) {
        if ((i - 1) % SquareSize == 0) {
            cout << " -";
            for (int j = 1; j <= BoardSize; j++) {
                cout << "---";
            }
            cout << "-";
            cout << endl;
        }
        for (int j = 1; j <= BoardSize; j++) {
            if ((j - 1) % SquareSize == 0) {
                cout << "|";
            }
            if (!isBlank(i, j)) {
                cout << " " << checkConflict(i, j, getCell(i, j)) << " ";
            } else {
                cout << "   ";
            }
        }
        cout << "|";
        cout << endl;
    }

    cout << " -";
    for (int j = 1; j <= BoardSize; j++) {
        cout << "---";
    }
    cout << "-";
    cout << endl;
}

bool board::checkConflict(int i, int j, ValueType v) {
    // if the number is already placed there, it is a conflict
    if (rows[i].at(v) == true ||
            cols[j].at(v) == true ||
            sqs[squareNumber(i, j)].at(v) == true) {
        return true;
    }
    return false;
}

int main() {
    ifstream fin;

    // Read the sample grid from the file.
    string fileName = "sudoku.txt";

    fin.open(fileName.c_str());
    if (!fin) {
        cerr << "Cannot open " << fileName << endl;
        exit(1);
    }

    try {
        board b1(SquareSize);

        //while (fin && fin.peek() != 'Z')
        //{
        b1.initialize(fin);
        b1.print();
        cout << endl << endl;
        b1.printConflicts();
        //}
    } catch  (indexRangeError &ex) {
        cout << ex.what() << endl;
        exit(1);
    }
}

