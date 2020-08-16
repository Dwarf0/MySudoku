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

	bool isValueValidAt(int value, int row, int col);
	bool cellIsValid(int row, int col);
	bool isValid();

	QString toString() const;
	void print() const;

protected:
	struct SudokuCell {
		bool isInitialValue = false;
		bool isValid = false;
		int value = 0;
	};
	typedef struct SudokuCell SudokuCell;

private slots:

private:
	void resetModelValues();

	SudokuCell _values[SUDOKU_SIZE][SUDOKU_SIZE];
};

#endif // SUDOKUMODEL_H
