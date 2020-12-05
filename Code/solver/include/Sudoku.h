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

	// getters
	bool isFilled() const;
	bool isValid() const;

	int getCellValue(int r, int c) const { return _values[r][c]->getValue(); }
	QSet<int> getCellPossibleValues(int r, int c) const { return _values[r][c]->getPossibleValues(); }
	bool isCellValid(int r, int c) const;
	bool isCellInitialValue(int r, int c) const { return _values[r][c]->isInitialValue(); }

	//setters
	void reset();
	// TODO : faire un "setValue(QVector<QVector<int> >)" ?

	void setCellValue(int r, int c, int val);
	void setCellValid(int r, int c, bool valid) { _values[r][c]->setValid(valid); }
	void setCellInitialValue(int r, int c, int val);
	void setCellPossibleValues(int r, int c, QSet<int> possibleValues);
	
	int loadFromCsv(QString filepath);
	void updateValidity();

	//
	QString toString() const;
	void print() const;

private:
	SudokuCell *_values[SUDOKU_SIZE][SUDOKU_SIZE];

#ifdef _DEBUG
	SudokuCell *_solutionValues[SUDOKU_SIZE][SUDOKU_SIZE];
	void checkForFilterError();
#endif
};


#endif // SUDOKU_H
