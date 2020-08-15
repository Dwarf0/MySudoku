#ifndef MYSUDOKUVIEWER_H
#define MYSUDOKUVIEWER_H

#include <QObject>
#include <QMainWindow>
#include <qmenubar.h>
#include <QAction>
#include <iostream>
#include "ui_MainWindow.h"

class MySudokuViewer : public QMainWindow
{
	Q_OBJECT
public:
	MySudokuViewer(QWidget *parent = nullptr) {
		_mainWindowUi.setupUi(this);

		connect(_mainWindowUi.actionLoadFromCsv, &QAction::triggered, this, &MySudokuViewer::loadFromCsv);
	}

	~MySudokuViewer() {
	}

private slots:
	void loadFromCsv() { std::cout << "hello" << std::endl; };

private:
	Ui::MainWindow _mainWindowUi;
};

#endif // MYSUDOKUVIEWER_H
