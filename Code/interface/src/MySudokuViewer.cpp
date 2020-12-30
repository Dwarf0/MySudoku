#include "MySudokuViewer.h"
#include "Solver.h"

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
	
	_mainWindowUi.filterGroupBox->setVisible(false);
	_mainWindowUi.helperGroupBox->setVisible(false);

	connect(_mainWindowUi.actionLoadFromCsv, &QAction::triggered, this, &MySudokuViewer::loadFromCsv);
	connect(_mainWindowUi.actionInitManually, &QAction::triggered, this, &MySudokuViewer::initManually);
	connect(_mainWindowUi.actionAutocheck, &QAction::triggered, _sudokuModel, &SudokuModel::setAutocheckMode);
	connect(_mainWindowUi.actionDisplayHelp, &QAction::triggered, _sudokuModel, &SudokuModel::displayHelp);

	connect(_mainWindowUi.solveButton, &QPushButton::clicked, _sudokuModel, &SudokuModel::solve);
	
	QSignalMapper* signalMapper = new QSignalMapper(this);

	connect(_mainWindowUi.directFilterButton, &QPushButton::clicked, signalMapper, static_cast<void (QSignalMapper::*)(void)> (&QSignalMapper::map));
	connect(_mainWindowUi.indirectFilterButton, &QPushButton::clicked, signalMapper, static_cast<void (QSignalMapper::*)(void)> (&QSignalMapper::map));
	connect(_mainWindowUi.groupFilterButton, &QPushButton::clicked, signalMapper, static_cast<void (QSignalMapper::*)(void)> (&QSignalMapper::map));
	connect(_mainWindowUi.hiddenFilterButton, &QPushButton::clicked, signalMapper, static_cast<void (QSignalMapper::*)(void)> (&QSignalMapper::map));
	connect(_mainWindowUi.noChoiceFilterButton, &QPushButton::clicked, signalMapper, static_cast<void (QSignalMapper::*)(void)> (&QSignalMapper::map));
	
	signalMapper->setMapping(_mainWindowUi.directFilterButton, Solver::Direct);
	signalMapper->setMapping(_mainWindowUi.indirectFilterButton, Solver::Indirect);
	signalMapper->setMapping(_mainWindowUi.groupFilterButton, Solver::Group);
	signalMapper->setMapping(_mainWindowUi.hiddenFilterButton, Solver::HiddenGroup);
	signalMapper->setMapping(_mainWindowUi.noChoiceFilterButton, Solver::NoChoice);

	connect(_mainWindowUi.autofillButton, &QPushButton::clicked, _sudokuModel, &SudokuModel::autofill);


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
	if (initWindow->exec() == QDialog::Accepted) {
		const Sudoku * s = model->getSudoku();
		_sudokuModel->initFromSudoku(s);
	}
	delete initWindow;
}