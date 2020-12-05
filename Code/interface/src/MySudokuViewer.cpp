#include "MySudokuViewer.h"
#include "solver.h"

#include <QFileDialog>
#include <QSignalMapper>
#include <QtGlobal>

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
	connect(_mainWindowUi.actionDisplayHelp, &QAction::triggered, _sudokuModel, &SudokuModel::displayHelp);

	QSignalMapper* signalMapper = new QSignalMapper(this);
	connect(_mainWindowUi.directFilterButton, &QPushButton::clicked, signalMapper, static_cast<void (QSignalMapper::*)(void)> (&QSignalMapper::map));
	connect(_mainWindowUi.indirectFilterButton, &QPushButton::clicked, signalMapper, static_cast<void (QSignalMapper::*)(void)> (&QSignalMapper::map));
	connect(_mainWindowUi.groupFilterButton, &QPushButton::clicked, signalMapper, static_cast<void (QSignalMapper::*)(void)> (&QSignalMapper::map));
	connect(_mainWindowUi.hiddenFilterButton, &QPushButton::clicked, signalMapper, static_cast<void (QSignalMapper::*)(void)> (&QSignalMapper::map));
	connect(_mainWindowUi.noChoiceFilterButton, &QPushButton::clicked, signalMapper, static_cast<void (QSignalMapper::*)(void)> (&QSignalMapper::map));

	signalMapper->setMapping(_mainWindowUi.directFilterButton, solver::Direct);
	signalMapper->setMapping(_mainWindowUi.indirectFilterButton, solver::Indirect);
	signalMapper->setMapping(_mainWindowUi.groupFilterButton, solver::Group);
	signalMapper->setMapping(_mainWindowUi.hiddenFilterButton, solver::HiddenGroup);
	signalMapper->setMapping(_mainWindowUi.noChoiceFilterButton, solver::NoChoice);

	connect(signalMapper, QOverload<int>::of(&QSignalMapper::mapped), _sudokuModel, &SudokuModel::applyFilter);
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
