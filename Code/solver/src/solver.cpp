#include "solver.h"


bool applyFilter(Sudoku *sudoku, int filterType)
{
	bool changed = false;

	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			QSet<int> possibleValues;
			if (filterType == Direct)
				possibleValues = directValueFilter(sudoku, i, j);
			else if (filterType == Indirect)
				possibleValues = indirectValueFilter(sudoku, i, j);
			else if (filterType == Group)
				possibleValues = groupValueFilter(sudoku, i, j);
			else if (filterType == HiddenGroup)
				possibleValues = hiddenGroupValueFilter(sudoku, i, j);
			if (possibleValues != sudoku->getPossibleValues(i, j)) {
				sudoku->setPossibleValues(i, j, possibleValues);
				changed = true;
			}
		}
	}
	return changed;
}

//bool updatePossibleValues(Sudoku *sudoku)
//{
//	if (_value > 0) {
//		_possibleValues.clear();
//		return;
//	}
//	if (_possibleValues.size() == 1) {
//		return;
//	}
//	bool changed = false;
//	// TODO : si à l'issue d'un filtre, on ne se retrouve avec qu'une seule valeur possible, 
//	//        alors les autres filtres ne devraient pas être lancés
//	//changed |= directValueFilter();
//	//changed |= indirectValueFilter();
//	//changed |= groupValueFilter();
//	noChoiceFilter(); //changed |= noChoiceFilter();
//					  //changed |= hiddenGroupValueFilter();
//
//					  // TODO : quand implémenté, ajouter un check du bool "autofill" de SudokuModel. 
//					  //	      Si true et si _possibleValues n'a qu'un élément, faire un setValue
//
//	return changed;
//}

QSet<int> directValueFilter(Sudoku *sudoku, int row, int col)
{
	int value = sudoku->getValue(row, col);
	QSet<int> possibleValues = sudoku->getPossibleValues(row, col);

	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		possibleValues.remove(sudoku->getValue(row, i));
		possibleValues.remove(sudoku->getValue(i, col));

		int squareRow = (row / 3) * 3 + i % 3;
		int squareCol = (col / 3) * 3 + i / 3;
		possibleValues.remove(sudoku->getValue(squareRow, squareCol));
	}

	return possibleValues;
}

QSet<int> indirectValueFilter(Sudoku *sudoku, int row, int col)
{
	int value = sudoku->getValue(row, col);
	QSet<int> possibleValues = sudoku->getPossibleValues(row, col);
	QSet<int> tmpPossibleValue = possibleValues;

	foreach(int possibleValue, tmpPossibleValue) {
		int squarePosRow = (row / 3) * 3;
		int squarePosCol = (col / 3) * 3;
		// Boucle autour des carrés
		for (int i = 0; i < SUDOKU_SIZE; i += 3) {
			// si on n'est pas sur le carré de la cellule courante
			if (i != squarePosRow) {
				// on regarde si parmi les cellules de ce carré, 
				bool isOnlyInCol = true;
				for (int j = 0; j < SUDOKU_SIZE; ++j) {
					int squareRow = (i / 3) * 3 + j % 3;
					int squareCol = (col / 3) * 3 + j / 3;

					// si la valeur est déjà présente dans le carré
					int curCellValue = sudoku->getValue(squareRow, squareCol);
					if (curCellValue == possibleValue) {
						isOnlyInCol = false;
						break;
					}

					QSet<int> values = sudoku->getPossibleValues(squareRow, squareCol);
					// si la possibleValue est possible sur d'autres colonnes que la notre
					if (values.contains(possibleValue) && squareCol != col) {
						isOnlyInCol = false;
						break;
					}
				}
				// si non (i.e. la possibleValue ne peut être que sur notre colonne),
				if (isOnlyInCol) {
					// alors notre valeur n'est finalement pas possible
					possibleValues.remove(possibleValue);
				}
			}
			if (i != squarePosCol) {
				// on regarde si parmi les cellules de ce carré, 
				bool isOnlyInRow = true;
				for (int j = 0; j < SUDOKU_SIZE; ++j) {
					int squareRow = (row / 3) * 3 + j % 3;
					int squareCol = (i / 3) * 3 + j / 3;

					// si la valeur est déjà présente dans le carré
					int curCellValue = sudoku->getValue(squareRow, squareCol);
					if (curCellValue == possibleValue) {
						isOnlyInRow = false;
						break;
					}

					QSet<int> values = sudoku->getPossibleValues(squareRow, squareCol);
					// si la possibleValue est possible sur d'autres colonnes que la notre
					if (values.contains(possibleValue) && squareRow != row) {
						isOnlyInRow = false;
						break;
					}
				}
				// si non (i.e. la possibleValue ne peut être que sur notre colonne),
				if (isOnlyInRow) {
					// alors notre valeur n'est finalement pas possible
					possibleValues.remove(possibleValue);
				}
			}
		}
	}
	sudoku->setPossibleValues(row, col, possibleValues);
	return possibleValues;
}

QSet<int> hiddenGroupValueFilter(Sudoku *sudoku, int row, int col)
{
	int value = sudoku->getValue(row, col);
	QSet<int> possibleValues = sudoku->getPossibleValues(row, col);
	
	QList<QSet<int> > allSets;
	createCombinations(QSet<int>(), possibleValues, allSets);
	qSort(allSets.begin(), allSets.end(), [](QSet<int > a, QSet<int > b) { return a.size() > b.size(); });
	foreach(QSet<int> curSet, allSets) {
		int nbCellsWithGrp = 0;
		int nbCellsWithPartOfGrp = 0;
		for (int i = 0; i < SUDOKU_SIZE; ++i) {
			int nbMatchValues = 0;
			bool partOfGrp = false;
			QSet<int> values = sudoku->getPossibleValues(row, i);
			foreach(int curValue, curSet) {
				if (values.contains(curValue)) {
					++nbMatchValues;
				}
				if (values.contains(curValue)) {
					partOfGrp = true;
				}
			}
			if (nbMatchValues == curSet.size()) {
				++nbCellsWithGrp;
			}
			if (partOfGrp) {
				++nbCellsWithPartOfGrp;
			}
		}
		if (curSet.size() == nbCellsWithGrp && curSet.size() == nbCellsWithPartOfGrp) {
			// NB : si  curGrp.size() == cellsInNextGroup.size() == 1, on est dans le cas noChoiceFilter
			// NB : si curGrp == les possibleValues de chaque cell membre du groupe, on est dans le cas groupValueFilter
			possibleValues.clear();
			foreach(int val, curSet) {
				possibleValues.insert(val);
			}
			// TODO : Modifier la valeur du possibleValues des autres membres du groupe ?
			break;
		}

		for (int i = 0; i < SUDOKU_SIZE; ++i) {
			int nbMatchValues = 0;
			bool partOfGrp = false;
			QSet<int> tmpPossibleValues = sudoku->getPossibleValues(i, col);
			foreach(int curValue, curSet) {
				if (tmpPossibleValues.contains(curValue)) {
					++nbMatchValues;
				}
				if (tmpPossibleValues.contains(curValue)) {
					partOfGrp = true;
				}
			}
			if (nbMatchValues == curSet.size()) {
				++nbCellsWithGrp;
			}
			if (partOfGrp) {
				++nbCellsWithPartOfGrp;
			}
		}
		if (curSet.size() == nbCellsWithGrp && curSet.size() == nbCellsWithPartOfGrp) {
			// NB : si  curGrp.size() == cellsInNextGroup.size() == 1, on est dans le cas noChoiceFilter
			// NB : si curGrp == les possibleValues de chaque cell membre du groupe, on est dans le cas groupValueFilter
			possibleValues.clear();
			foreach(int val, curSet) {
				possibleValues.insert(val);
			}
			// TODO : Modifier la valeur du possibleValues des autres membres du groupe ?
			break;
		}

		for (int i = 0; i < SUDOKU_SIZE; ++i) {
			int nbMatchValues = 0;
			bool partOfGrp = false;
			int squareRow = (row / 3) * 3 + i % 3;
			int squareCol = (col / 3) * 3 + i / 3;
			QSet<int> tmpPossibleValues = sudoku->getPossibleValues(squareRow, squareCol);
			foreach(int curValue, curSet) {
				if (tmpPossibleValues.contains(curValue)) {
					++nbMatchValues;
				}
				if (tmpPossibleValues.contains(curValue)) {
					partOfGrp = true;
				}
			}
			if (nbMatchValues == curSet.size()) {
				++nbCellsWithGrp;
			}
			if (partOfGrp) {
				++nbCellsWithPartOfGrp;
			}
		}
		if (curSet.size() == nbCellsWithGrp && curSet.size() == nbCellsWithPartOfGrp) {
			// NB : si  curSet.size() == cellsInNextGroup.size() == 1, on est dans le cas noChoiceFilter
			// NB : si curSet == les possibleValues de chaque cell membre du groupe, on est dans le cas groupValueFilter
			possibleValues.clear();
			foreach(int val, curSet) {
				possibleValues.insert(val);
			}
			// TODO : Modifier la valeur du possibleValues des autres membres du groupe ?
			break;
		}
	}
	
	return possibleValues;
}

QSet<int> groupValueFilter(Sudoku *sudoku, int row, int col)
{
	int value = sudoku->getValue(row, col);
	QSet<int> possibleValues = sudoku->getPossibleValues(row, col);

	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		int squareRow = (row / 3) * 3 + i % 3;
		int squareCol = (col / 3) * 3 + i / 3;

		QSet<int> sameRowCellPossibleValues = sudoku->getPossibleValues(row, i);
		QSet<int> sameColCellPossibleValues = sudoku->getPossibleValues(i, col);
		QSet<int> sameSquareCellPossibleValues = sudoku->getPossibleValues(squareRow, squareCol);
		int sameRowPossible = 0;
		int sameColPossible = 0;
		int sameSquarePossible = 0;
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (col != j && sudoku->getPossibleValues(row, j) == sameRowCellPossibleValues) {
				++sameRowPossible;
			}
			if (row != j && sudoku->getPossibleValues(j, col) == sameColCellPossibleValues) {
				++sameColPossible;
			}
			int tmpSquareRow = (row / 3) * 3 + j % 3;
			int tmpSquareCol = (col / 3) * 3 + j / 3;
			if ((row != tmpSquareRow || col != tmpSquareCol)
				&& sudoku->getPossibleValues(tmpSquareRow, tmpSquareCol) == sameSquareCellPossibleValues) {
				++sameSquarePossible;
			}
		}
		if (sameRowCellPossibleValues.size() == sameRowPossible) {
			foreach(int v, sameRowCellPossibleValues) {
				possibleValues.remove(v);
			}
		}
		if (sameColCellPossibleValues.size() == sameColPossible) {
			foreach(int v, sameColCellPossibleValues) {
				possibleValues.remove(v);
			}
		}
		if (sameSquareCellPossibleValues.size() == sameSquarePossible) {
			foreach(int v, sameSquareCellPossibleValues) {
				possibleValues.remove(v);
			}
		}
	}
	return possibleValues;
}

QSet<int> noChoiceFilter(Sudoku *sudoku, int row, int col)
{
	int value = sudoku->getValue(row, col);
	QSet<int> possibleValues = sudoku->getPossibleValues(row, col);

	QSet<int> tmpPossibleValue = possibleValues;
	foreach(int possibleValue, tmpPossibleValue) {
		bool onlyPlaceOnRow = true;
		bool onlyPlaceOnCol = true;
		bool onlyPlaceInSquare = true;
		for (int i = 0; i < SUDOKU_SIZE; ++i) {
			QSet<int> sameRowCellPossibleValues = sudoku->getPossibleValues(row, i);
			if (sameRowCellPossibleValues.contains(possibleValue) && i != col)
				onlyPlaceOnRow = false;

			QSet<int> sameColCellPossibleValues = sudoku->getPossibleValues(i, col);
			if (sameColCellPossibleValues.contains(possibleValue) && i != row)
				onlyPlaceOnCol = false;

			int squareRow = (row / 3) * 3 + i % 3;
			int squareCol = (col / 3) * 3 + i / 3;
			QSet<int> sameSquareCellPossibleValues = sudoku->getPossibleValues(squareRow, squareCol);
			if (sameSquareCellPossibleValues.contains(possibleValue)
				&& (squareRow != row || squareCol != col))
				onlyPlaceInSquare = false;
		}
		if (onlyPlaceOnRow || onlyPlaceOnCol || onlyPlaceInSquare) {
			possibleValues = { possibleValue };
			break;
		}
	}
	return possibleValues;
}

void createCombinations(QSet<int> inList, QSet<int> toAdd, QList<QSet<int> > &outList)
{
	if (toAdd.empty()) {
		return;
	}
	QSet<int> tmpToAdd = toAdd;
	foreach(int value, tmpToAdd) {
		QSet<int> curList = inList;
		curList.insert(value);
		outList.append(curList);

		toAdd.remove(value);

		createCombinations(curList, toAdd, outList);
	}
}
