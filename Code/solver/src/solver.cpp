#include "solver.h"

/// TODO : Faire une fonction solve dont l'algo pourrait ressembler à :
//func solve{
//	while (changed) {
//		pour chaque filtre F,
//			changed |= updatePossibleValues avec filtre F
//	}
//	si le sudoku est résolu,
//		fin de la func
//	sinon, on récupère la cell avec le moins de possibleValues
//	pour chaque valeur V possible de la cell
//		on clone le sudoku actuel
//		on set la valeur de la cell du clone avec V
//		on rappelle solve sur le clone
//		si le sudoku est résolu,
//			on affecte les valeurs du clone au sudoku initial
//			fin de la func
//		sinon, la valeur utilisé précédemment n était pas la bonne
//}

bool solver::updatePossibleValues(Sudoku *sudoku, int filterType/*=0*/)
{
	// NB : We copy the given sudoku so we can update the sudoku's cells' possible values while still 
	// using the original possible values for the filters.
	// This is suboptimal for solving the Sudoku itself, but it's better user-wise as we won't skip/hide steps for the user

	bool changed = false;
	Sudoku clone;
	clone = *sudoku;

	// TODO : ajouter un "while( !converged ){ ... }" avec converged qui récupère le retour de applyFilter ?
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			QSet<int> curPossibleValues = clone.getCellPossibleValues(i, j);
			if (curPossibleValues.size() > 1) {
				QSet<int> newValues = applyFiltersOnCell(&clone, filterType, i, j);
				if (curPossibleValues != newValues) {
					changed |= true;
					sudoku->setCellPossibleValues(i, j, newValues);
				}
			}
		}
	}

	return changed;
}

QSet<int> solver::applyFiltersOnCell(Sudoku *sudoku, int filterType, int row, int col)
{
	bool changed = false;
	QSet<int> possibleValues = sudoku->getCellPossibleValues(row, col);

	if (filterType & Direct) {
		possibleValues.intersect(directValueFilter(sudoku, row, col));
	}
	if (filterType & Indirect) {
		possibleValues.intersect(indirectValueFilter(sudoku, row, col));
	}
	if (filterType & Group) {
		possibleValues.intersect(groupValueFilter(sudoku, row, col));
	}
	if (filterType & HiddenGroup) {
		possibleValues.intersect(hiddenGroupValueFilter(sudoku, row, col));
	}
	if (filterType & NoChoice) {
		possibleValues.intersect(noChoiceFilter(sudoku, row, col));
	}
	
	return possibleValues;
}

/// TODO : devrait aussi considérer les Cell vides mais qui n'ont qu'une seule valeur possible
QSet<int> solver::directValueFilter(const Sudoku *sudoku, int row, int col)
{
	int value = sudoku->getCellValue(row, col);
	QSet<int> possibleValues = sudoku->getCellPossibleValues(row, col);

	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		if(i != col)
			possibleValues.remove(sudoku->getCellValue(row, i));
		if (i != row)
			possibleValues.remove(sudoku->getCellValue(i, col));

		int squareRow = (row / 3) * 3 + i % 3;
		int squareCol = (col / 3) * 3 + i / 3;
		if(i != row || i != col)
			possibleValues.remove(sudoku->getCellValue(squareRow, squareCol));
	}

	return possibleValues;
}

QSet<int> solver::indirectValueFilter(const Sudoku *sudoku, int row, int col)
{
	int value = sudoku->getCellValue(row, col);
	QSet<int> possibleValues = sudoku->getCellPossibleValues(row, col);
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
					int curCellValue = sudoku->getCellValue(squareRow, squareCol);
					if (curCellValue == possibleValue) {
						isOnlyInCol = false;
						break;
					}

					QSet<int> values = sudoku->getCellPossibleValues(squareRow, squareCol);
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
					int curCellValue = sudoku->getCellValue(squareRow, squareCol);
					if (curCellValue == possibleValue) {
						isOnlyInRow = false;
						break;
					}

					QSet<int> values = sudoku->getCellPossibleValues(squareRow, squareCol);
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

	return possibleValues;
}

QSet<int> solver::hiddenGroupValueFilter(const Sudoku *sudoku, int row, int col)
{
	int value = sudoku->getCellValue(row, col);
	QSet<int> possibleValues = sudoku->getCellPossibleValues(row, col);
	
	QList<QSet<int> > allSets;
	createCombinations(QSet<int>(), possibleValues, allSets);
	qSort(allSets.begin(), allSets.end(), [](QSet<int > a, QSet<int > b) { return a.size() > b.size(); });
	foreach(QSet<int> curSet, allSets) {
		int nbCellsWithGrp = 0;
		int nbCellsWithPartOfGrp = 0;

		// row
		for (int i = 0; i < SUDOKU_SIZE; ++i) {
			int nbMatchValues = 0;
			QSet<int> values = sudoku->getCellPossibleValues(row, i);
			foreach(int curValue, curSet) {
				if (values.contains(curValue)) {
					++nbMatchValues;
				}
			}
			if (nbMatchValues == curSet.size()) {
				++nbCellsWithGrp;
			}
			if (nbMatchValues) {
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
			break;
		}

		// col
		for (int i = 0; i < SUDOKU_SIZE; ++i) {
			int nbMatchValues = 0;
			QSet<int> tmpPossibleValues = sudoku->getCellPossibleValues(i, col);
			foreach(int curValue, curSet) {
				if (tmpPossibleValues.contains(curValue)) {
					++nbMatchValues;
				}
			}
			if (nbMatchValues == curSet.size()) {
				++nbCellsWithGrp;
			}
			if (nbMatchValues) {
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
			break;
		}

		// square
		for (int i = 0; i < SUDOKU_SIZE; ++i) {
			int nbMatchValues = 0;
			int squareRow = (row / 3) * 3 + i % 3;
			int squareCol = (col / 3) * 3 + i / 3;
			QSet<int> tmpPossibleValues = sudoku->getCellPossibleValues(squareRow, squareCol);
			foreach(int curValue, curSet) {
				if (tmpPossibleValues.contains(curValue)) {
					++nbMatchValues;
				}
			}
			if (nbMatchValues == curSet.size()) {
				++nbCellsWithGrp;
			}
			if (nbMatchValues) {
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
			break;
		}
	}
	
	return possibleValues;
}

QSet<int> solver::groupValueFilter(const Sudoku *sudoku, int row, int col)
{
	int value = sudoku->getCellValue(row, col);
	QSet<int> possibleValues = sudoku->getCellPossibleValues(row, col);

	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		int squareRow = (row / 3) * 3 + i % 3;
		int squareCol = (col / 3) * 3 + i / 3;

		QSet<int> sameRowCellPossibleValues = sudoku->getCellPossibleValues(row, i);
		QSet<int> sameColCellPossibleValues = sudoku->getCellPossibleValues(i, col);
		QSet<int> sameSquareCellPossibleValues = sudoku->getCellPossibleValues(squareRow, squareCol);
		int sameRowPossible = 0;
		int sameColPossible = 0;
		int sameSquarePossible = 0;
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (col != j && sudoku->getCellPossibleValues(row, j) == sameRowCellPossibleValues) {
				++sameRowPossible;
			}
			if (row != j && sudoku->getCellPossibleValues(j, col) == sameColCellPossibleValues) {
				++sameColPossible;
			}
			int tmpSquareRow = (row / 3) * 3 + j % 3;
			int tmpSquareCol = (col / 3) * 3 + j / 3;
			if ((row != tmpSquareRow || col != tmpSquareCol)
				&& sudoku->getCellPossibleValues(tmpSquareRow, tmpSquareCol) == sameSquareCellPossibleValues) {
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

QSet<int> solver::noChoiceFilter(const Sudoku *sudoku, int row, int col)
{
	int value = sudoku->getCellValue(row, col);
	QSet<int> possibleValues = sudoku->getCellPossibleValues(row, col);

	QSet<int> tmpPossibleValue = possibleValues;
	foreach(int possibleValue, tmpPossibleValue) {
		bool onlyPlaceOnRow = true;
		bool onlyPlaceOnCol = true;
		bool onlyPlaceInSquare = true;
		for (int i = 0; i < SUDOKU_SIZE; ++i) {
			QSet<int> sameRowCellPossibleValues = sudoku->getCellPossibleValues(row, i);
			if (sameRowCellPossibleValues.contains(possibleValue) && i != col)
				onlyPlaceOnRow = false;

			QSet<int> sameColCellPossibleValues = sudoku->getCellPossibleValues(i, col);
			if (sameColCellPossibleValues.contains(possibleValue) && i != row)
				onlyPlaceOnCol = false;

			int squareRow = (row / 3) * 3 + i % 3;
			int squareCol = (col / 3) * 3 + i / 3;
			QSet<int> sameSquareCellPossibleValues = sudoku->getCellPossibleValues(squareRow, squareCol);
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

void solver::createCombinations(QSet<int> inList, QSet<int> toAdd, QList<QSet<int> > &outList)
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
