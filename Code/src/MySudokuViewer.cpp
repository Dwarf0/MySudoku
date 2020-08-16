#include "MySudokuViewer.h"

#include <QFileDialog>

MySudokuViewer::MySudokuViewer(QWidget *parent) {
	_mainWindowUi.setupUi(this);

	_sudokuModel = new SudokuModel(this);

	_sudokuView = _mainWindowUi.tableView;
	_sudokuView->setModel(_sudokuModel);
	_sudokuView->horizontalHeader()->hide();
	_sudokuView->verticalHeader()->hide();
	_sudokuView->horizontalHeader()->setDefaultSectionSize(60);
	_sudokuView->verticalHeader()->setDefaultSectionSize(60);
	
	connect(_mainWindowUi.actionLoadFromCsv, &QAction::triggered, this, &MySudokuViewer::loadFromCsv);
}

MySudokuViewer::~MySudokuViewer() {
}

void MySudokuViewer::loadFromCsv()
{ 
	QFileDialog diag(this, tr("Select csv file"), QString(), tr("CSV (*.csv)"));
	QString csvPath = diag.getOpenFileName();
	if (csvPath != QString::null) {
		_sudokuModel->loadFromCsv(csvPath);
	}
}
