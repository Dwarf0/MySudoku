#ifndef SUDOKUMODEL_H
#define SUDOKUMODEL_H

#include <QObject>
#include <QMainWindow>
#include <QAbstractItemModel>
#include <qmenubar.h>
#include <QAction>
#include <iostream>
#include <qvector.h>

#define SUDOKU_SIZE 9

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

	bool valueIsValid(int row, int col);
	bool isValid();

	QString toString();
	void print();

private slots:

private:
	int _values[9][9];
};

#endif // SUDOKUMODEL_H
