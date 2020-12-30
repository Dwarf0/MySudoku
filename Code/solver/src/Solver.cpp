#include "solver.h"
#include <iostream>


void Solver::solve(Sudoku *sudoku)
{
	bool changed = true;
	int allFilters = 0;
	for (int i = 0; i < _filters.size(); ++i) {
		allFilters |= 1 << i;
	}
	while (changed) {
		sudoku->autofill();
		changed = updatePossibleValues(sudoku, allFilters);
	}
	sudoku->autofill();
	if (sudoku->isSolved()) {
		return;
	}

	int row = -1;
	int col = -1;
	QSet<int> possibleValues = {1, 2, 3, 4, 5, 6, 7 ,8 , 9};
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			QSet<int> tmp = sudoku->getCellPossibleValues(i, j);
			if (tmp.size() < possibleValues.size() && sudoku->getCellValue(i, j) == 0) {
				row = i;
				col = j;
				possibleValues = tmp;
			}
		}
	}
	
	foreach(int value, possibleValues) {
		Sudoku clone;
		clone = *sudoku;

		clone.setCellValue(row, col, value);
		clone.setCellPossibleValues(row, col, { value });
		solve(&clone);
		if (clone.isSolved()) {
			*sudoku = clone;
			std::cout << "A supposition had to be made to solve the Sudoku" << std::endl;
			return;
		}
	}
	if (false) {
		sudoku->print();
	}
}

bool Solver::updatePossibleValues(Sudoku *sudoku, int filterType/*=0*/)
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

QSet<int> Solver::applyFiltersOnCell(Sudoku *sudoku, int filterType, int row, int col)
{
	bool changed = false;
	QSet<int> possibleValues = sudoku->getCellPossibleValues(row, col);

	QHashIterator<FilterTypes, Filter> it(_filters);
	while (it.hasNext()) {
		it.next();
		if (filterType & it.key()) {
			possibleValues.intersect(it.value().filter(sudoku, row, col));
		}
	}

	return possibleValues;
}
