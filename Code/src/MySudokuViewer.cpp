#include "MySudokuViewer.h"

#include <QFileDialog>

MySudokuViewer::MySudokuViewer(QWidget *parent) {
	_mainWindowUi.setupUi(this);

	connect(_mainWindowUi.actionLoadFromCsv, &QAction::triggered, this, &MySudokuViewer::loadFromCsv);
}

MySudokuViewer::~MySudokuViewer() {
}

void MySudokuViewer::loadFromCsv()
{ 
	QFileDialog diag(this, tr("Select csv file"), QString(), tr("CSV (*.csv)"));
	QString csvPath = diag.getOpenFileName();
	if (csvPath != QString::null) {
		_sudokuModel.loadFromCsv(diag.getOpenFileName());
	}
}
