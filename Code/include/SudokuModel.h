#ifndef SUDOKUMODEL_H
#define SUDOKUMODEL_H

#include <QObject>
#include <QMainWindow>
#include <QAbstractItemModel>
#include <qmenubar.h>
#include <QAction>
#include <iostream>
#include <qvector.h>

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
	// bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	int loadFromCsv(QString csvPath);

	bool cellIsValid(int row, int col);
	bool isValid();

	QString toString();
	void print();

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
