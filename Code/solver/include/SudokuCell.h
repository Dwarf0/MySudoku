#ifndef SUDOKUCELL_H
#define SUDOKUCELL_H

#include <QObject>
#include <iostream>
#include <qset.h>

#include "globals.h"

class SudokuModel;

class SudokuCell : public QObject
{
public:
	SudokuCell(int row = -1, int col = -1) :
		_row(row), _col(col), _value(0), _isInitialValue(false), _isValid(true), _possibleValues({ 1, 2, 3, 4 ,5 ,6 , 7, 8, 9 })
	{}

	~SudokuCell() {}

	void setRow(int row) { _row = row; }
	void setColumn(int col) { _col = col; }
	void setPossibleValues(QSet<int> possibleValues) { _possibleValues = possibleValues; }

	void setValue(int value);
	void setInitialValue(int value);
	void reset();

	int getValue() const { return _value; }
	int isValid() const { return _isValid; }
	int isInitialValue() const { return _isInitialValue; }
	QSet<int> getPossibleValues() const { return _possibleValues;  }

	void setValid(bool valid) { _isValid = valid; }

private:

	bool _isInitialValue;
	bool _isValid;
	int _value;
	QSet<int> _possibleValues;

	int _row;
	int _col;
};

#endif // SUDOKUCELL_H
