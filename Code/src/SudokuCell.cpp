#include "SudokuCell.h"

#include <QFile>
#include <QTextStream>
#include <QBrush>

#include "SudokuModel.h"

SudokuCell::SudokuCell(SudokuModel *model) {
	_isInitialValue = false;
	_isValid = false;
	_value = 0;
	_row = -1;
	_col = -1;
	_model = model;
}

SudokuCell::~SudokuCell() {
}

void SudokuCell::setValue(int value)
{
	if (_value != value) {
		_value = value;
		updateIsValid();
		emit valueChanged(value);
	}
}

void SudokuCell::reset()
{
	_value = 0;
	_isInitialValue = false;
	_isValid = false;
	_possibleValues = { 1, 2, 3, 4 ,5 ,6 , 7, 8, 9 };
}

QList<int> SudokuCell::getPossibleValues() const
{
	QList<int> values;
	foreach(int v, _possibleValues) {
		values.append(v);
	}
	qSort(values);
	return values;
}

void SudokuCell::updateCell() {
	updateIsValid();
	updatePossibleValues();
}

void SudokuCell::updateIsValid()
{
	bool valid = true;

	if (_value != 0) {
		for (int i = 0; i < SUDOKU_SIZE; ++i) {
			bool alreadyOnRow = _model->data(_model->index(_row, i), SudokuModel::CellValueRole) == _value && i != _col;
			bool alreadyOnCol = _model->data(_model->index(i, _col), SudokuModel::CellValueRole) == _value && i != _row;

			int squareRow = (_row / 3) * 3 + i % 3;
			int squareCol = (_col / 3) * 3 + i / 3;
			bool alreadyOnSquare = _model->data(_model->index(squareRow, squareCol), SudokuModel::CellValueRole) == _value 
									&& squareRow != _row && squareCol != _col;
			if (alreadyOnRow || alreadyOnCol || alreadyOnSquare) {
				valid = false;
				break;
			}
		}
	}

	_isValid = valid;
}

void SudokuCell::updatePossibleValues()
{
	if (_value > 0) {
		_possibleValues.clear();
	}
	else {
		_possibleValues = { 1, 2, 3, 4 ,5 ,6 , 7, 8, 9 };
		for (int i = 0; i < SUDOKU_SIZE; ++i) {
			_possibleValues.remove(_model->data(_model->index(_row, i), SudokuModel::CellValueRole).toInt());
			_possibleValues.remove(_model->data(_model->index(i, _col), SudokuModel::CellValueRole).toInt());

			int squareRow = (_row / 3) * 3 + i % 3;
			int squareCol = (_col / 3) * 3 + i / 3;
			_possibleValues.remove(_model->data(_model->index(squareRow, squareCol), SudokuModel::CellValueRole).toInt());
		}
	}
	// TODO : quand implémenté, ajouter un check du bool "autofill" de SudokuModel. 
	//	      Si true et si _possibleValues n'a qu'un élément, faire un setValue
}

