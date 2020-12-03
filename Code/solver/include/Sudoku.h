#ifndef SUDOKU_H
#define SUDOKU_H

#include <QObject>
#include <iostream>
#include <qset.h>

#include "globals.h"

#include <SudokuCell.h>


class Sudoku
{
public:
	Sudoku();

	~Sudoku();

	void reset();

	int getValue(int r, int c) { return _values[r][c]->getValue(); }
	QSet<int> getPossibleValues(int r, int c) { return _values[r][c]->getPossibleValues(); }

	bool isFilled();
	bool isValid();
	bool isInitialValue(int r, int c) { return _values[r][c]->isInitialValue(); }

	void setValue(int r, int c, int val);
	void setValid(int r, int c, bool valid) { _values[r][c]->setValid(valid); }
	void setInitialValue(int r, int c, int val) { _values[r][c]->setInitialValue(val); }
	void setPossibleValues(int r, int c, QSet<int> possibleValues);
	
	int loadFromCsv(QString filepath);
	void updateValidity();

private:

	SudokuCell *_values[SUDOKU_SIZE][SUDOKU_SIZE];
};


#endif // SUDOKU_H
