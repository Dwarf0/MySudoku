#ifndef MYSUDOKUVIEWER_H
#define MYSUDOKUVIEWER_H

#include <QObject>
#include <QMainWindow>
#include <iostream>

#include "ui_MainWindow.h"

#include "SudokuModel.h"
#include "SudokuView.h"
#include "SudokuDelegate.h"

class MySudokuViewer : public QMainWindow
{
	Q_OBJECT
public:
	MySudokuViewer(QWidget *parent = nullptr);
	~MySudokuViewer();

private slots:
	void loadFromCsv();

private:
	Ui::MainWindow _mainWindowUi;
	SudokuModel *_sudokuModel;
	SudokuView *_sudokuView;
	SudokuDelegate *_sudokuDelegate;
};

#endif // MYSUDOKUVIEWER_H
