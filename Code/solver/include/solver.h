#ifndef SOLVER_H
#define SOLVER_H

#include "Sudoku.h"

namespace solver{

	/*! 
	* Enum containing the different filters available.
	*
	* Enum containing the different filters available. The enum values are set so that it can be directly used as a flag.
	*/
	enum FilterTypes {
		Direct=1, 
		Indirect=2,
		Group=4,
		HiddenGroup=8,
		NoChoice = 16,
	};

	/*!
	* Apply the given filters to the Sudoku.
	*
	* Update the possible values of the sudoku using given the filters. 
	* 'Applying a filter' means removing possible values from the cells of given Sudoku using differents strategies (or filters). 
	* filterType is to be used as a flag, each bit corresponding to a filter (cf FilterTypes).
	* It returns true if the filters applied removed at least one value in any of the cells of the Sudoku.
	* 
	* @param sudoku The sudoku to update
	* @param filterType The filters to be used 
	* @returns Returns true if the sudoku has been changed
	*/
	bool DLL_EXPORT updatePossibleValues(Sudoku *sudoku, int filterType = 0);

	/*!
	* Apply the given filters to one cell of the Sudoku.
	*
	* Update the possible values of the cell of the sudoku using given the filters.
	* 'Applying a filter' means removing possible values from the requested cell using differents strategies (or filters).
	* filterType is to be used as a flag, each bit corresponding to a filter (cf FilterTypes).
	* It returns true if the filters applied removed at least one value in any of the cells of the Sudoku.
	*
	* @param sudoku The sudoku to update
	* @param filterType The filters to be used 
	* @param row Row of the cell to apply the filter on 
	* @param col Column of the cell to apply the filter on 
	* @returns Returns true if the sudoku has been changed
	*/
	bool DLL_EXPORT applyFilterOnCell(Sudoku *sudoku, int filterType, int row, int col);

	/*!
	* Use the "direct" filter on a cell of the sudoku and return the filtered possible values
	*
	* The stategy of this filter is to simply apply the sudoku rules, i.e. an empty cell cannot have X in its possible values
	* if X is already either :
	* - in the same row,
	* - int the same column,
	* - in the same square
	*
	* @param sudoku The sudoku to update
	* @param row Row of the cell to apply the filter on 
	* @param col Column of the cell to apply the filter on 
	* @returns Returns the filtered set of possible values
	*/
	QSet<int> DLL_EXPORT directValueFilter(const Sudoku *sudoku, int row, int col);

	/*!
	* Use the "indirect" filter on a cell of the sudoku and return the filtered possible values
	*
	* The stategy of this filter is to consider that if, in a square, a value is only possible in one row,
	* then this value cannot be in the rest of the row (i.e. cannot be in this row but in the other squares).
	* The same logic applies for the column.
	*
	* @param sudoku The sudoku to update
	* @param row Row of the cell to apply the filter on
	* @param col Column of the cell to apply the filter on
	* @returns Returns the filtered set of possible values
	*/
	QSet<int> DLL_EXPORT indirectValueFilter(const Sudoku *sudoku, int row, int col);

	/*!
	* Use the "group" filter on a cell of the sudoku and return the filtered possible values
	*
	* The stategy of this filter is to consider that if a group of N cells in a row contains the exact same N possible values,
	* then the rest of the cells of the row cannot contain these N possible values.
	* 
	* The same logic applies for the row and the square.
	*
	* @param sudoku The sudoku to update
	* @param row Row of the cell to apply the filter on
	* @param col Column of the cell to apply the filter on
	* @returns Returns the filtered set of possible values
	*/
	QSet<int> DLL_EXPORT groupValueFilter(const Sudoku *sudoku, int row, int col);

	/*!
	* Use the "hiddenGroup" filter on a cell of the sudoku and return the filtered possible values
	*
	* This stategy is similar to the "group" filter since it is about finding a group of cells sharing the same possible values in the same row.
	* The difference here is that it handles "parasitic" values that could be in the possible values of the group.
	* 
	* For example, if, in a row, :
	* - first cell only has 1 and 2 as possible values,
	* - second cell only has 1, 2, 3 as possible values,
	* - all the other cells contain various possible values, but none has neither 1 nor 2 
	* 
	* then only 1st and 2nd cell can possibly hold the value 1 and 2, meaning 1st and 2nd cell forms a group and the possible value 3 of the 2nd cell
	* is considered as a parasitic value.
	* 
	* The same logic applies for the row and the square.
	*
	* @param sudoku The sudoku to update
	* @param row Row of the cell to apply the filter on
	* @param col Column of the cell to apply the filter on
	* @returns Returns the filtered set of possible values
	*/
	QSet<int> DLL_EXPORT hiddenGroupValueFilter(const Sudoku *sudoku, int row, int col);

	/*!
	* Use the "noChoice" filter on a cell of the sudoku and return the filtered possible values
	*
	* The stategy of this filter is to consider that if in a row, a possible value X is only possible in one cell, then we can clear the 
	* X from the possible value of the cells sharing a column or a square with the cell.
	* 
	* The same logic applies for the row and the square
	*
	* @param sudoku The sudoku to update
	* @param row Row of the cell to apply the filter on
	* @param col Column of the cell to apply the filter on
	* @returns Returns the filtered set of possible values
	*/
	QSet<int> DLL_EXPORT noChoiceFilter(const Sudoku *sudoku, int row, int col);

	/*!
	* Return all the subsets that it is possible to create with given set of values.
	*
	* This recursive function create the combinations it is possible to make from given set of values.
	*
	* @param inList The set of value shared by all subsets
	* @param toAdd The set to create the subset from
	* @param outList The list of subsets created
	*/
	void DLL_EXPORT createCombinations(QSet<int> inList, QSet<int> toAdd, QList<QSet<int> > &outList);
}
#endif