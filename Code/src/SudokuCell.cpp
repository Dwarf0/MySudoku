#include "SudokuCell.h"

#include <QFile>
#include <QTextStream>
#include <QBrush>

#include "SudokuModel.h"


void SudokuCell::setValue(int value)
{
	if (_value != value) {
		_value = value;
		if (_value == 0) {
			_possibleValues = { 1, 2, 3, 4 ,5 , 6, 7, 8, 9 };
			updatePossibleValues();
		} else {
			_possibleValues.clear();
			updateIsValid();
		}
		if (_emitEnabled) emit valueChanged(value);
	}
}

void SudokuCell::reset()
{
	_value = 0;
	_isInitialValue = false;
	_isValid = false;
	_possibleValues = { 1, 2, 3, 4 ,5 , 6, 7, 8, 9 };
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
									&& (squareRow != _row || squareCol != _col);
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
		return;
	}
	if (_possibleValues.size() == 1) {
		return;
	}
	bool changed = false;
	// TODO : si à l'issue d'un filtre, on ne se retrouve avec qu'une seule valeur possible, 
	//        alors les autres filtres ne devraient pas être lancés
	changed |= directValueFilter();
	changed |= indirectValueFilter();
	changed |= groupValueFilter();
	changed |= noChoiceFilter();

	// TODO : quand implémenté, ajouter un check du bool "autofill" de SudokuModel. 
	//	      Si true et si _possibleValues n'a qu'un élément, faire un setValue

	if (changed && _emitEnabled) emit possibleValuesChanged();
}

bool SudokuCell::directValueFilter()
{
	bool changed = false;
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		changed |= _possibleValues.remove(_model->data(_model->index(_row, i), SudokuModel::CellValueRole).toInt());
		changed |= _possibleValues.remove(_model->data(_model->index(i, _col), SudokuModel::CellValueRole).toInt());

		int squareRow = (_row / 3) * 3 + i % 3;
		int squareCol = (_col / 3) * 3 + i / 3;
		changed |= _possibleValues.remove(_model->data(_model->index(squareRow, squareCol), SudokuModel::CellValueRole).toInt());
	}
	return changed;
}

bool SudokuCell::indirectValueFilter()
{
	bool changed = false;
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
					changed |= _possibleValues.remove(possibleValue);
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
					changed |= _possibleValues.remove(possibleValue);
				}
			}
		}
	}
	return changed;
}

bool SudokuCell::groupValueFilter()
{
	bool changed = false;
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		int squareRow = (_row / 3) * 3 + i % 3;
		int squareCol = (_col / 3) * 3 + i / 3;

		QString sameRowCellPossibleValues = _model->data(_model->index(_row, i), SudokuModel::PossibleValuesRole).toString();
		QString sameColCellPossibleValues = _model->data(_model->index(i, _col), SudokuModel::PossibleValuesRole).toString();
		QString sameSquareCellPossibleValues = _model->data(_model->index(squareRow, squareCol), SudokuModel::PossibleValuesRole).toString();
		int sameRowPossible = 0;
		int sameColPossible = 0;
		int sameSquarePossible = 0;
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (_col != j && _model->data(_model->index(_row, j), SudokuModel::PossibleValuesRole).toString() == sameRowCellPossibleValues) {
				++sameRowPossible;
			}
			if (_row != j && _model->data(_model->index(j, _col), SudokuModel::PossibleValuesRole).toString() == sameColCellPossibleValues) {
				++sameColPossible;
			}
			int tmpSquareRow = (_row / 3) * 3 + j % 3;
			int tmpSquareCol = (_col / 3) * 3 + j / 3;
			if ((_row != tmpSquareRow || _col != tmpSquareCol) 
				&& _model->data(_model->index(tmpSquareRow, tmpSquareCol), SudokuModel::PossibleValuesRole).toString() == sameSquareCellPossibleValues) {
				++sameSquarePossible;
			}
		}
		if (sameRowCellPossibleValues.split(" ").size() == sameRowPossible) {
			foreach(QString v, sameRowCellPossibleValues.split(" ")) {
				changed |= _possibleValues.remove(v.toInt());
			}
		}
		if (sameColCellPossibleValues.split(" ").size() == sameColPossible) {
			foreach(QString v, sameColCellPossibleValues.split(" ")) {
				changed |= _possibleValues.remove(v.toInt());
			}
		}
		if (sameSquareCellPossibleValues.split(" ").size() == sameSquarePossible) {
			foreach(QString v, sameSquareCellPossibleValues.split(" ")) {
				changed |= _possibleValues.remove(v.toInt());
			}
		}
	}
	return changed;
}

bool SudokuCell::noChoiceFilter()
{
	bool changed = false;
	QSet<int> tmpPossibleValue = _possibleValues;
	foreach(int possibleValue, tmpPossibleValue) {
		bool onlyPlaceOnRow = true;
		bool onlyPlaceOnCol = true;
		bool onlyPlaceInSquare = true;
		for (int i = 0; i < SUDOKU_SIZE; ++i) {
			QString sameRowCellPossibleValues = _model->data(_model->index(_row, i), SudokuModel::PossibleValuesRole).toString();
			if(sameRowCellPossibleValues.contains(QString::number(possibleValue))
				&& i != _col)
				onlyPlaceOnRow = false;
			QString sameColCellPossibleValues = _model->data(_model->index(i, _col), SudokuModel::PossibleValuesRole).toString();
			if (sameColCellPossibleValues.contains(QString::number(possibleValue))
				&& i != _row)
				onlyPlaceOnCol = false;
			
			int squareRow = (_row / 3) * 3 + i % 3;
			int squareCol = (_col / 3) * 3 + i / 3;
			QString sameSquareCellPossibleValues = _model->data(_model->index(squareRow, squareCol), SudokuModel::PossibleValuesRole).toString();
			if (sameSquareCellPossibleValues.contains(QString::number(possibleValue))
				&& (squareRow != _row || squareCol != _col))
				onlyPlaceInSquare = false;
		}
		if (onlyPlaceOnRow || onlyPlaceOnCol || onlyPlaceInSquare) {
			changed = _possibleValues == QSet<int>{ possibleValue };
			_possibleValues = { possibleValue };
			break;
		}
	}
	return changed;
}
