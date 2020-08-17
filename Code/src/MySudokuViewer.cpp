#include "MySudokuViewer.h"

#include <QFileDialog>

MySudokuViewer::MySudokuViewer(QWidget *parent) {
	_mainWindowUi.setupUi(this);

	_sudokuModel = new SudokuModel(this);
	_sudokuDelegate = new SudokuDelegate(this);

	_sudokuView = _mainWindowUi.tableView;
	_sudokuView->setModel(_sudokuModel);
	_sudokuView->setItemDelegate(_sudokuDelegate);
	_sudokuView->horizontalHeader()->hide();
	_sudokuView->verticalHeader()->hide();
	_sudokuView->horizontalHeader()->setDefaultSectionSize(60);
	_sudokuView->verticalHeader()->setDefaultSectionSize(60);
	
	connect(_mainWindowUi.actionLoadFromCsv, &QAction::triggered, this, &MySudokuViewer::loadFromCsv);
	connect(_mainWindowUi.actionAutocheck, &QAction::triggered, _sudokuModel, &SudokuModel::setAutocheckMode);
}

MySudokuViewer::~MySudokuViewer() {
	delete _sudokuModel;
	delete _sudokuDelegate;
}

void MySudokuViewer::loadFromCsv()
{ 
	QFileDialog diag;
	QString csvPath = diag.getOpenFileName(this, tr("Select csv file"), QString(), tr("CSV (*.csv)"));
	if (csvPath != QString::null) {
		_sudokuModel->loadFromCsv(csvPath);
	}
}
