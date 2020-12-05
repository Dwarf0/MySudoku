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

	bool updatePossibleValues(Sudoku *sudoku, int filterType = 0);

	bool applyFilterOnCell(Sudoku *sudoku, int filterType, int row, int col);
	QSet<int> directValueFilter(Sudoku *sudoku, int row, int col);
	QSet<int> indirectValueFilter(Sudoku *sudoku, int row, int col);
	QSet<int> groupValueFilter(Sudoku *sudoku, int row, int col);
	QSet<int> hiddenGroupValueFilter(Sudoku *sudoku, int row, int col);
	QSet<int> noChoiceFilter(Sudoku *sudoku, int row, int col);

	void createCombinations(QSet<int> inList, QSet<int> toAdd, QList<QSet<int> > &outList);
}
#endif