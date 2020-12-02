#include "SudokuCell.h"

#include <QFile>
#include <QTextStream>
#include <QBrush>


void SudokuCell::setValue(int value)
{
	if (_value != value) {
		_value = value;
		if (_value == 0) {
			_possibleValues = { 1, 2, 3, 4 ,5 , 6, 7, 8, 9 };
		} else {
			_possibleValues.clear();
		}
	}
}

void SudokuCell::setInitialValue(int value)
{
	_value = value;
	if(_value)
		_isInitialValue = true;
}

void SudokuCell::reset()
{
	_value = 0;
	_isInitialValue = false;
	_isValid = true;
	_possibleValues = { 1, 2, 3, 4 ,5 , 6, 7, 8, 9 };
}
