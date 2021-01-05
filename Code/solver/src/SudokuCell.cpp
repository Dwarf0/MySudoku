#include "SudokuCell.h"

#include <QFile>
#include <QTextStream>
#include <QBrush>


SudokuCell::SudokuCell(const SudokuCell * s)
{
	_row = s->_row;
	_col = s->_col;
	_value = s->_value;
	_isInitialValue = s->_isInitialValue;
	_isValid = s->_isValid;
	_possibleValues = s->_possibleValues;
}

void SudokuCell::setValue(int value)
{
	if (value < 0 || value > 9) {
		QString mess = QString("Error: trying to set an invalid value (%1) for SudokuCell at (%2,%3).")
								.arg(QString::number(value), QString::number(_row), QString::number(_col));
		qWarning(mess.toStdString().c_str());
	}
	if (_value != value) {
		_value = value;
		if (_value == 0) {
			_possibleValues = { 1, 2, 3, 4 ,5 , 6, 7, 8, 9 };
		}
	}
}

void SudokuCell::setInitialValue(int value)
{
	_value = value;
	if (_value) {
		_isInitialValue = true;
		_possibleValues = { _value };
	}
}

void SudokuCell::reset()
{
	_value = 0;
	_isInitialValue = false;
	_isValid = true;
	_possibleValues = { 1, 2, 3, 4 ,5 , 6, 7, 8, 9 };
}
