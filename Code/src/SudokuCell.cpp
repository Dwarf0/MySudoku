﻿#include "SudokuCell.h"

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
		directValueFilter();
		indirectValueFilter();
	}
	// TODO : quand implémenté, ajouter un check du bool "autofill" de SudokuModel. 
	//	      Si true et si _possibleValues n'a qu'un élément, faire un setValue
}

void SudokuCell::directValueFilter()
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		_possibleValues.remove(_model->data(_model->index(_row, i), SudokuModel::CellValueRole).toInt());
		_possibleValues.remove(_model->data(_model->index(i, _col), SudokuModel::CellValueRole).toInt());

		int squareRow = (_row / 3) * 3 + i % 3;
		int squareCol = (_col / 3) * 3 + i / 3;
		_possibleValues.remove(_model->data(_model->index(squareRow, squareCol), SudokuModel::CellValueRole).toInt());
	}
}

void SudokuCell::indirectValueFilter()
{
	QSet<int> tmpPossibleValue = _possibleValues;
	foreach(int possibleValue, tmpPossibleValue) {
		int squarePosRow = (_row / 3) * 3;
		int squarePosCol = (_col / 3) * 3;
		// Boucle autour des carrés
		for (int i = 0; i < SUDOKU_SIZE; i += 3) {
			// si on n'est pas sur le carré de la cellule courante
			if (i != squarePosRow) {
				// on regarde si parmi les cellules de ce carré, 
				bool isOnlyInCol = true;
				for (int j = 0; j < SUDOKU_SIZE; ++j) {
					int squareRow = (i / 3) * 3 + j % 3;
					int squareCol = (_col / 3) * 3 + j / 3;

					// si la valeur est déjà présente dans le carré
					int curCellValue = _model->data(_model->index(squareRow, squareCol), SudokuModel::CellValueRole).toInt();
					if (curCellValue == possibleValue) {
						isOnlyInCol = false;
						break;
					}

					QString possibleValues = _model->data(_model->index(squareRow, squareCol), SudokuModel::PossibleValuesRole).toString();
					// si la possibleValue est possible sur d'autres colonnes que la notre
					if (possibleValues.contains(QString::number(possibleValue))
						&& squareCol != _col) {
						isOnlyInCol = false;
						break;
					}
				}
				// si non (i.e. la possibleValue ne peut être que sur notre colonne),
				if (isOnlyInCol) {
					// alors notre valeur n'est finalement pas possible
					_possibleValues.remove(possibleValue);
				}
			}
			if (i != squarePosCol) {
				// on regarde si parmi les cellules de ce carré, 
				bool isOnlyInRow = true;
				for (int j = 0; j < SUDOKU_SIZE; ++j) {
					int squareRow = (_row / 3) * 3 + j % 3;
					int squareCol = (i / 3) * 3 + j / 3;

					// si la valeur est déjà présente dans le carré
					int curCellValue = _model->data(_model->index(squareRow, squareCol), SudokuModel::CellValueRole).toInt();
					if (curCellValue == possibleValue) {
						isOnlyInRow = false;
						break;
					}

					QString possibleValues = _model->data(_model->index(squareRow, squareCol), SudokuModel::PossibleValuesRole).toString();
					// si la possibleValue est possible sur d'autres colonnes que la notre
					if (possibleValues.contains(QString::number(possibleValue))
						&& squareRow != _row) {
						isOnlyInRow = false;
						break;
					}
				}
				// si non (i.e. la possibleValue ne peut être que sur notre colonne),
				if (isOnlyInRow) {
					// alors notre valeur n'est finalement pas possible
					_possibleValues.remove(possibleValue);
				}
			}
		}
	}
}