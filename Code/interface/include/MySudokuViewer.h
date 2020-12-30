#ifndef MYSUDOKUVIEWER_H
#define MYSUDOKUVIEWER_H

#include <QObject>
#include <QMainWindow>
#include <QSignalMapper>
#include <iostream>

#include "ui_InitDialog.h"
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
	void initManually();

private:
	Ui::MainWindow _mainWindowUi;
	Ui::InitDialog _initDialogUi;

	SudokuModel *_sudokuModel;
	SudokuView *_sudokuView;
	SudokuDelegate *_sudokuDelegate;

	QSignalMapper *_signalMapper;
};

#endif // MYSUDOKUVIEWER_H
