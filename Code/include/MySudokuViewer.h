#ifndef MYSUDOKUVIEWER_H
#define MYSUDOKUVIEWER_H

#include <QObject>
#include <QMainWindow>
#include <qmenubar.h>
#include <QAction>
#include <iostream>

#include "ui_MainWindow.h"

#include "SudokuModel.h"

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
	SudokuModel _sudokuModel;
};

#endif // MYSUDOKUVIEWER_H
