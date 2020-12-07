#ifndef SUDOKUMODEL_H
#define SUDOKUMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <iostream>
#include <qset.h>

#include "globals.h"
#include "Sudoku.h"

class SudokuModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	SudokuModel(QWidget *parent = nullptr);
	~SudokuModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const override { return SUDOKU_SIZE; }
	int columnCount(const QModelIndex &parent = QModelIndex()) const override { return SUDOKU_SIZE; }
	QVariant data(const QModelIndex &index, int role) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	Qt::ItemFlags flags(const QModelIndex &index) const;

	int loadFromCsv(QString csvPath);
	void updateCell(int value, int row, int col); 
	void updateCellsPossibleValues();
	QList<int> getPossibleValues(int r, int c);

	bool isValid();
	bool isFilled();

	QString toString() const;
	void print() const;

	enum AdditionalRoles {
		IsValidRole = Qt::UserRole+1,
		IsInitialValueRole,
		CellValueRole,
		PossibleValuesRole
	};

	void applyFilter(int filterType);

public slots:
	void setAutocheckMode(bool autocheck) { _autocheckMode = autocheck; }
	void displayHelp(bool displayHelp);

private:
	void resetModelValues();

	bool _autocheckMode;
	bool _displayHelp;

	Sudoku *_sudoku;
};

#endif // SUDOKUMODEL_H