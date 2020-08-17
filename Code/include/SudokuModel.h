#ifndef SUDOKUMODEL_H
#define SUDOKUMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <iostream>

#include "globals.h"


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

	void setCellValue(int value, int row, int col);
	void updateCellValidAt(int row, int col);
	bool isValid();
	bool isFilled();

	QString toString() const;
	void print() const;

	enum AdditionalRoles {
		IsValidRole = Qt::UserRole+1,
		IsInitialValueRole
	};

protected:
	struct SudokuCell {
		bool isInitialValue = false;
		bool isValid = false;
		int value = 0;
	};
	typedef struct SudokuCell SudokuCell;

public slots:
	void setAutocheckMode(bool autocheck) { _autocheckMode = autocheck; }

private:
	void resetModelValues();

	SudokuCell _values[SUDOKU_SIZE][SUDOKU_SIZE];
	bool _autocheckMode;
};

#endif // SUDOKUMODEL_H
