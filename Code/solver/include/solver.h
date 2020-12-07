#ifndef SOLVER_H
#define SOLVER_H

#include "Sudoku.h"

namespace solver{

	enum FilterTypes {
		Direct=1, 
		Indirect=2,
		Group=4,
		HiddenGroup=8,
		NoChoice = 16,
	};

	bool DLL_EXPORT updatePossibleValues(Sudoku *sudoku, int filterType = 0);

	bool DLL_EXPORT applyFilterOnCell(Sudoku *sudoku, int filterType, int row, int col);
	QSet<int> DLL_EXPORT directValueFilter(Sudoku *sudoku, int row, int col);
	QSet<int> DLL_EXPORT indirectValueFilter(Sudoku *sudoku, int row, int col);
	QSet<int> DLL_EXPORT groupValueFilter(Sudoku *sudoku, int row, int col);
	QSet<int> DLL_EXPORT hiddenGroupValueFilter(Sudoku *sudoku, int row, int col);
	QSet<int> DLL_EXPORT noChoiceFilter(Sudoku *sudoku, int row, int col);

	void DLL_EXPORT createCombinations(QSet<int> inList, QSet<int> toAdd, QList<QSet<int> > &outList);
}
#endif