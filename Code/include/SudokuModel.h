#ifndef SUDOKUMODEL_H
#define SUDOKUMODEL_H

#include <QObject>
#include <QMainWindow>
#include <QAbstractItemModel>
#include <qmenubar.h>
#include <QAction>
#include <iostream>
#include <qvector.h>

#include "ui_MainWindow.h"

class SudokuModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	SudokuModel(QWidget *parent = nullptr);
	~SudokuModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const override { return 9; }
	int columnCount(const QModelIndex &parent = QModelIndex()) const override { return 9; }
	QVariant data(const QModelIndex &index, int role) const override;

	void loadFromCsv(QString csvPath);

	QString toString();

private slots:

private:
	QVector<QVector<int> > _values;
};

#endif // SUDOKUMODEL_H
