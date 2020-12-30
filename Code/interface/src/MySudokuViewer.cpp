#include "MySudokuViewer.h"
#include "Solver.h"

#include <QFileDialog>
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
	
	_signalMapper = new QSignalMapper(this);
	const QHash<Solver::FilterTypes, Filter> filters = _sudokuModel->getFilters();

	QList<Solver::FilterTypes> types = filters.keys();
	qSort<QList<Solver::FilterTypes> >(types);
	foreach(Solver::FilterTypes type, types) {
		QPushButton *button = new QPushButton(this);
		button->setText(filters[type].name());
		button->setToolTip(filters[type].description());
		connect(button, &QPushButton::clicked, _signalMapper, static_cast<void (QSignalMapper::*)(void)> (&QSignalMapper::map));
		_signalMapper->setMapping(button, type);
		_mainWindowUi.filterGroupBox->layout()->addWidget(button);
	}

	connect(_mainWindowUi.autofillButton, &QPushButton::clicked, _sudokuModel, &SudokuModel::autofill);
	connect(_signalMapper, QOverload<int>::of(&QSignalMapper::mapped), _sudokuModel, &SudokuModel::applyFilter);
}

MySudokuViewer::~MySudokuViewer() {
	delete _sudokuModel;
	delete _sudokuDelegate;
	delete _signalMapper;
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