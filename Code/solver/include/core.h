
#include <QSet>
#include "Sudoku.h"

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