#ifndef SUDOKU_H
#define SUDOKU_H

#include <QObject>
#include <iostream>
#include <qset.h>

#include "globals.h"

#include <SudokuCell.h>

/*!
* This class handles the properties of a sudoku puzzle.
*
* This class represents a sudoku puzzle. As such, it contains a SUDOKU_SIZE x SUDOKU_SIZE grid of SudokuCell.
* It lets the user get/set the values and possible values of each cell.
* 
* Initialization of the sudoku can be done using either the loadFromCsv or the enableInitMode/disableInitMode:
* - loadFromCsv needs a path to a csv file describing a sudoku puzzle.
* - when the initMode is set, every call to setCellValue will set the value and mark it as an initial value.
*/

class DLL_EXPORT Sudoku
{
public:
	/*!
	* Sudoku's constructor
	*/
	Sudoku();
	/*!
	* Sudoku's deconstructor
	*/
	~Sudoku();
	/*!
	* Sudoku's affectation operator copies over the value related properties of given Sudoku
	*
	* This function copies over the value, possibleValues and initialValue of each SudokuCell of given Sudoku instance.
	*
	* @param other Sudoku to get data from
	*/
	Sudoku& operator=(const Sudoku& other);

	/*!
	* Indicates if every cell's value has been set
	* @returns True if all SudokuCell contain a value
	*/
	bool isFilled() const;
	/*!
	* Indicates if every cell holds a value that respects sudoku rules
	* @returns True if all SudokuCell's value respects sudoku rules
	*/
	bool isValid() const;
	/*!
	* Indicates if the sudoku has been solved (i.e. filled and valid)
	* @returns True if the sudoku is both filled and valid
	*/
	bool isSolved() const { return isFilled() && isValid(); }

	/*!
	* Get the value of specified cell in the sudoku grid
	*
	* @param r row of the cell in the grid
	* @param c column of the cell in the grid
	* @returns the value of the cell
	*/
	int getCellValue(int r, int c) const { return _values[r][c]->getValue(); }
	/*!
	* Get the set of possible values of specified cell in the sudoku grid
	*
	* @param r row of the cell in the grid
	* @param c column of the cell in the grid
	* @returns the set of possible values of the cell
	*/
	QSet<int> getCellPossibleValues(int r, int c) const { return _values[r][c]->getPossibleValues(); }
	/*!
	* Indicates if the specified cell respects sudoku rules
	*
	* @param r row of the cell in the grid
	* @param c column of the cell in the grid
	* @returns True if it does not break any sudoku rules
	*/
	bool isCellValid(int r, int c) const;
	/*!
	* Indicates if the requested cell contains an initial value
	* 
	* This function indicates if the cell holds an initial value. An initial value is a value that is part of the puzzle 
	* (i.e. has not been user-defined) and is thus necessarily valid and unmodifiable.
	*
	* @param r row of the cell in the grid
	* @param c column of the cell in the grid
	* @returns True if the value is an initial value
	*/
	bool isCellInitialValue(int r, int c) const { return _values[r][c]->isInitialValue(); }

	/*!
	* Reset the sudoku grid
	*
	* Completely reset the sudoku (all cell's value set to 0 etc.).
	*/
	void reset();

	/*!
	* Clean the sudoku grid
	*
	* Reset the sudoku to its initial state (i.e. without user input, but keeping the initial puzzle values).
	*/
	void clean();
	/*!
	* Set the value of the SudokuCell. It must be between 0 and 9 included, 0 indicating an empty cell.
	* If value is 0, the possible values of the SudokuCell are also reset.
	*
	* @param r row of the cell in the grid
	* @param c column of the cell in the grid
	* @param val the value to set in the cell
	*/
	void setCellValue(int r, int c, int val);
	/*!
	* Set the possible values that can be contained in given cell.
	*
	* @param r row of the cell in the grid
	* @param c column of the cell in the grid
	* @param possibleValues the possible values to set in the cell
	*/
	void setCellPossibleValues(int r, int c, QSet<int> possibleValues);

	/*!
	* Makes the Sudoku enter "initialization mode"
	*
	* Makes the Sudoku enter "initialization mode". While in "initialization mode", calls to setCellValue also set the initialValue
	* state of the cell.
	*/
	void enableInitMode() { _initMode = true; }
	/*!
	* Makes the Sudoku leave "initialization mode"
	*
	* Makes the Sudoku leave "initialization mode". While in "initialization mode", calls to setCellValue also set the initialValue
	* state of the cell.
	*/
	void disableInitMode() { _initMode = false; }
	/*!
	* Initialize the Sudoku using the csv file at given path
	*
	* Initialize the Sudoku using the csv file at given path. The csv must only holds the initials value.
	*
	* @param filepath path to the csv file
	*/
	int loadFromCsv(QString filepath);

	/*!
	*  Automatically set the value of the empty cells with only one possible value
	*/
	void autofill();

	/*!
	* Returns a string version of the sudoku
	*
	* Returns a string version of the sudoku that could be outputted to a csv file.
	* The columns values are separated by a comma and rows are separated by a line return.
	* 
	* @returns A string version of the sudoku
	*/
	QString toString() const;
	/*!
	* Print the Sudoku to console.
	*/
	void print() const;

private:
	/*!
	* Set the value of the SudokuCell and makes it an initial value. 
	* 
	* Set the value of the SudokuCell and makes it an initial value. It must be between 0 and 9 included, 0 indicating an empty cell.
	*
	* @param r row of the cell in the grid
	* @param c column of the cell in the grid
	* @param val the value to set in the cell
	*/
	void setCellInitialValue(int r, int c, int val);
	/*!
	* Set if the value at given cell respect sudoku rules.
	*
	* @param r row of the cell in the grid
	* @param c column of the cell in the grid
	* @param valid True if the value respects the rules
	*/
	void setCellValid(int r, int c, bool valid) { _values[r][c]->setValid(valid); }
	/*!
	* Updates the valid state of each SudokuCell.
	*/
	void updateValidity();

	bool _initMode;  /*!< @brief True if Sudoku is in initialization mode, False otherwise */
	SudokuCell *_values[SUDOKU_SIZE][SUDOKU_SIZE];  /*!< @brief Grid of SudokuCell */

#ifdef _DEBUG
	void checkForFilterError();
	SudokuCell *_solutionValues[SUDOKU_SIZE][SUDOKU_SIZE];
#endif
};


#endif // SUDOKU_H
