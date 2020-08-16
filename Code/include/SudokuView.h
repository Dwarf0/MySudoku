#ifndef SUDOKUVIEW_H
#define SUDOKUVIEW_H

#include <QObject>
#include <QMainWindow>
#include <QTableView>
#include <qmenubar.h>
#include <QAction>
#include <iostream>
#include <qvector.h>

#include "SudokuModel.h"

class SudokuView : public QTableView
{
	Q_OBJECT
public:
	SudokuView(QWidget *parent = nullptr);
	~SudokuView();

private slots:

private:
	SudokuModel *_model;
};

#endif // SUDOKUVIEW_H
