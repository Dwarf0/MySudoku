#ifndef SOLVER_H
#define SOLVER_H

#include "Sudoku.h"
#include "Filter.h"
#include "core.h"

/*!
 * Solver is to be used as an interface for easy access to the filters and solve functions
*/
class DLL_EXPORT Solver
{
public:
	Solver();

	/*!
	* Enum containing the different filters available.
	*
	* Enum containing the different filters available. The enum values are set so that it can be directly used as a flag.
	*/
	enum FilterTypes {
		Direct = 1,
		Indirect = 2,
		Group = 4,
		HiddenGroup = 8,
		NoChoice = 16,
	};

	/*!
	* Solve the given Sudoku.
	*
	* Solve the Sudoku using all the filters available. If it is not enough to solve the sudoku, it affects a possible value
	* to an empty cell then run the filters again.
	*
	* @param sudoku The sudoku to solve
	*/
	void solve(Sudoku *sudoku);

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
	bool updatePossibleValues(Sudoku *sudoku, int filterType = 0);

	/*!
	* Apply the given filters to one cell of the Sudoku.
	*
	* Update the possible values of the cell of the sudoku using given the filters.
	* 'Applying a filter' means removing possible values from the requested cell using differents strategies (or filters).
	* filterType is to be used as a flag, each bit corresponding to a filter (cf FilterTypes).
	*
	* @param sudoku The sudoku to update
	* @param filterType The filters to be used
	* @param row Row of the cell to apply the filter on
	* @param col Column of the cell to apply the filter on
	* @returns Returns the updated set of possible values
	*/
	QSet<int> applyFiltersOnCell(Sudoku *sudoku, int filterType, int row, int col);

	/*!
	* Return a hash of adressable filters.
	*
	* @returns Returns  a hash of adressable filters
	*/
	const QHash<FilterTypes, Filter> getFilters() { return _filters; }

private:
	QHash<FilterTypes, Filter> _filters;
};

#endif