#ifndef SUDOKUMODEL_H
#define SUDOKUMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <iostream>
#include <qset.h>

#include "globals.h"
#include "Sudoku.h"
#include "Solver.h"

/*!
* This model is used as an interface between a Sudoku data and its (table) view.
*/
class SudokuModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	SudokuModel(QWidget *parent = nullptr);
	~SudokuModel();
	
	int rowCount(const QModelIndex &parent = QModelIndex()) const override { return SUDOKU_SIZE; }
	int columnCount(const QModelIndex &parent = QModelIndex()) const override { return SUDOKU_SIZE; }
	/*!
	* This method overrides some of the roles of the QAbstractTableModel:
	* - on Qt::DisplayRole, it displays the value (or the possible values if help is enabled) of the associated cell,
	* - on Qt::BackgroundRole, it displays green bg on valid cell, red bg on invalid cell, white on empty or initial cell.
	* 
	* The custom roles IsValidRole, IsInitialValueRole, CellValueRole, PossibleValuesRole are used to access parameters of the SudokuCell
	* associated with the current index.
	*
	* @param index QModelIndex of the cell
	* @param role Refers to the type of data to return
	* @returns a QVariant whose type depends on role
	*/
	QVariant data(const QModelIndex &index, int role) const override;
	/*!
	* This functions handles the flags of each cell.
	*
	* All items of the table are ItemIsSelectable and ItemIsEnabled and only non-initial cells are ItemIsEditable.
	* 
	* @param index QModelIndex of the cell
	* @returns all the flags associated with the cell
	*/
	Qt::ItemFlags flags(const QModelIndex &index) const;
	/*!
	* Return a constant pointer to the managed Sudoku.
	*
	* @returns a constant pointer to the managed Sudoku
	*/
	const Sudoku * getSudoku() { return _sudoku; }
	
	/*!
	* Sets the value of a SudokuCell.
	* 
	* @param index QModelIndex of the cell
	* @param value new value to be set on the cell
	* @param role Type of the data to set
	* @returns true if the new value has correctly been set, false otherwise
	*/
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	/*!
	* Enable the initialization mode.
	*
	* The initialization mode makes all call to setData also set the "initialValue" state of the cell.
	*/
	void enableInitMode() { _sudoku->enableInitMode(); }
	/*!
	* Disable the initialization mode.
	*
	* The initialization mode makes all call to setData also set the "initialValue" state of the cell.
	*/
	void disableInitMode() { _sudoku->disableInitMode(); }
	
	/*!
	* Initialize the Sudoku using the csv given at path.
	*
	* The csv must contains 9 lines, each line having 9 commas separated values that represent the initial values.
	* The 0 value means an empty cell.
	*
	* @param csvPath path to the csv file
	* @returns An error code
	*/
	int loadFromCsv(QString csvPath); 
	/*!
	* Initialize the Sudoku using the values of the cells of another Sudoku.
	*
	* @param Sudoku The Sudoku to copy the values from
	*/
	void initFromSudoku(const Sudoku *sudoku);

	/*!
	* Check if the cells' values of the current Sudoku respects sudoku rules
	* 
	* @returns true if sudoku rules are respected, false otherwise
	*/
	bool isValid();
	/*!
	* Check if all Sudoku's cells' values have been filled
	*
	* @returns true if all cells have a value, false otherwise
	*/
	bool isFilled();
	
	/*!
	* An enumeration used to have additionnal roles for the SudokuModel::data mathod
	*/
	enum AdditionalRoles {
		IsValidRole = Qt::UserRole+1,
		IsInitialValueRole,
		CellValueRole,
		PossibleValuesRole
	};

	/*!
	* Apply a filter to the Sudoku.
	* 
	* The filters available can be found in solver::FilterTypes.
	*
	* @param filterType Filter to apply on the managed Sudoku
	*/
	void applyFilter(int filterType);

	/*!
	* Completely solve the Sudoku.
	*/
	void solve();

	const QHash<Solver::FilterTypes, Filter> getFilters() { return solver.getFilters(); }

public slots:
	/*!
	* Set if the autocheck mode is to be used or not.
	*
	* The autocheck mode adds a background color to non-initial value to indicate if it respects the sudoku rules according to currently
	* known values of the Sudoku (i.e. it does not compare it to the Sudoku's solution).
	*
	* @param autocheck true if autocheck mode is to be used, false otherwise
	*/
	void setAutocheckMode(bool autocheck);
	/*!
	* Set if the help mode is to be used or not.
	*
	* The help mode displays on the currently empty cells which values could be placed. 
	* Different filters can be used to clear some of these values.
	*
	* @param displayHelp true if help mode is to be used, false otherwise
	*/
	void displayHelp(bool displayHelp);

	/*!
	* Automatically set the value of the empty cells with only one possible value
	*/
	void autofill();

private:
	/*!
	* Reset Sudoku's cells' values.
	*/
	void resetModelValues();

	bool _autocheckMode;
	bool _displayHelp;

	Sudoku *_sudoku;
	Solver solver;
};

#endif // SUDOKUMODEL_H
