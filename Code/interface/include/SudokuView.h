#ifndef SUDOKUVIEW_H
#define SUDOKUVIEW_H

#include <QObject>
#include <QTableView>
#include <iostream>

#include "SudokuModel.h"

class SudokuView : public QTableView
{
	Q_OBJECT
public:
	SudokuView(QWidget *parent = nullptr);
	~SudokuView();

private:
	SudokuModel *_model;
};

#endif // SUDOKUVIEW_H
