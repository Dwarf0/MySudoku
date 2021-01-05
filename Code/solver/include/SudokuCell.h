#ifndef SUDOKUCELL_H
#define SUDOKUCELL_H

#include <QObject>
#include <iostream>
#include <qset.h>

#include "globals.h"

/*!
* This class handles the properties of a cell of a sudoku.
*
* This class represents one cell of a sudoku. 
* Its value is within 0 and 9 (included), 0 indicating an empty cell.
* _isInitialValue indicates if its value is an initial value of the puzzle or if it has been set by the user.
* If the cell is empty (i.e. _value is 0), _possibleValues refers to the values it could contain while respecting 
* the other SudokuCells of the Sudoku containing it.
* 
*/

class SudokuCell
{
public:
	/*!
	* SudokuCell's constructor
	* @param row Row of the cell within the Sudoku
	* @param col Column of the cell within the Sudoku
	*/
	SudokuCell(int row = -1, int col = -1) :
		_row(row), _col(col), _value(0), _isInitialValue(false), _isValid(true), _possibleValues({ 1, 2, 3, 4 ,5 ,6 , 7, 8, 9 })
	{}

	SudokuCell(const SudokuCell *s);
	/*!
	* SudokuCell's deconstructor
	*/
	~SudokuCell() {}

	/*!
	* Set the row of the Cell
	* @param row The new row value of the SudokuCell
	*/
	void setRow(int row) { _row = row; }
	/*!
	* Set the column of the Cell
	* @param col The new column value of the SudokuCell
	*/
	void setColumn(int col) { _col = col; }
	/*!
	* Set the values that can be contained in this SudokuCell
	* @param possibleValues The updated possible values of the SudokuCell
	*/
	void setPossibleValues(QSet<int> possibleValues) { _possibleValues = possibleValues; }
	/*!
	* Set the value of the SudokuCell. It must be between 0 and 9 included, 0 indicating an empty cell.
	* If value is 0, the possible values of the SudokuCell are also reset.
	* 
	* @param value The new value of the cell
	*/
	void setValue(int value);
	/*!
	* Set the initial value of a cell. It also set _isInitialValue to True and _possibleValues to given value.
	* @param value The value of the cell
	*/
	void setInitialValue(int value);
	/*!
	* Set if the currently hold value is valid or not according to the Sudoku containing the SudokuCell.
	* @param valid True if current value is to be considered valid
	*/
	void setValid(bool valid) { _isValid = valid; }
	/*!
	* Reset all of the SudokuCell attributes to the constructor's default ones, except for _row and _col.
	*/
	void reset();

	/*!
	* Get the SudokuCell value. It is an integer value within 0 and 9 included, 0 indicating an empty cell.
	* @returns The current value of the SudokuCell
	*/
	int getValue() const { return _value; }
	/*!
	* Get if the SudokuCell holds an initial value.
	* @returns True if the SudokuCell contains an initial value, False if it was user defined
	*/
	int isInitialValue() const { return _isInitialValue; }
	/*!
	* Get if the SudokuCell holds a valid value according to last validity check of the Sudoku containing the cell.
	* @returns True if the value of the cell is valid
	*/
	int isValid() const { return _isValid; }
	/*!
	* Get the possible values according to the Sudoku containing this SudokuCell.
	* @returns The set of possible values of the SudokuCell
	*/
	QSet<int> getPossibleValues() const { return _possibleValues;  }

private:

	bool _isInitialValue; /*!< @brief True if the value is an initial/unmodifiable one, False otherwise */
	bool _isValid; /*!< @brief if the SudokuCell holds a valid value according to last validity check */
	int _value; /*!< @brief contains the value of the cell (0 if value is unset) */
	QSet<int> _possibleValues;  /*!< @brief contains the values that could be affected to the SudokuCell 
									 according to what is currently known about the Sudoku containing the Cell*/

	int _row; /*!< @brief Cell row position inside the Sudoku */
	int _col; /*!< @brief Cell column position inside the Sudoku */
};

#endif // SUDOKUCELL_H
