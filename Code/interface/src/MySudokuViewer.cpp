#include "MySudokuViewer.h"
#include "solver.h"

#include <QFileDialog>
#include <QSignalMapper>
#include <QtGlobal>
#include <QWindow>
#include <QDialogButtonBox>

MySudokuViewer::MySudokuViewer(QWidget *parent) {
	_mainWindowUi.setupUi(this);

	_sudokuView = _mainWindowUi.tableView;
	_sudokuModel = new SudokuModel(_sudokuView);
	_sudokuDelegate = new SudokuDelegate(_sudokuView);
	_sudokuView->setModel(_sudokuModel);
	_sudokuView->setItemDelegate(_sudokuDelegate);
	
	connect(_mainWindowUi.actionLoadFromCsv, &QAction::triggered, this, &MySudokuViewer::loadFromCsv);
	connect(_mainWindowUi.actionInitManually, &QAction::triggered, this, &MySudokuViewer::initManually);
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

void MySudokuViewer::initManually()
{
	QDialog *initWindow = new QDialog(this);
	_initDialogUi.setupUi(initWindow);

	SudokuModel *model = new SudokuModel(_initDialogUi.tableView);
	SudokuDelegate *delegate = new SudokuDelegate(_initDialogUi.tableView);
	_initDialogUi.tableView->setModel(model);
	_initDialogUi.tableView->setItemDelegate(delegate);
	model->enableInitMode();
	if (initWindow->exec() == QDialog::Accepted) {
		const Sudoku * s = model->getSudoku();
		_sudokuModel->initFromSudoku(s);
	}
	model->disableInitMode();
	delete initWindow;
}