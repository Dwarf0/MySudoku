
#include "Sudoku.h"

enum FilterTypes {
	Direct, 
	Indirect,
	Group,
	HiddenGroup
};

// bool updatePossibleValues(Sudoku *sudoku);

bool applyFilter(Sudoku *sudoku, int filterType);
QSet<int>  directValueFilter(Sudoku *sudoku, int row, int col);
QSet<int>  indirectValueFilter(Sudoku *sudoku, int row, int col);
QSet<int>  hiddenGroupValueFilter(Sudoku *sudoku, int row, int col);

QSet<int>  groupValueFilter(Sudoku *sudoku, int row, int col);
QSet<int>  noChoiceFilter(Sudoku *sudoku, int row, int col);

void createCombinations(QSet<int> inList, QSet<int> toAdd, QList<QSet<int> > &outList);
