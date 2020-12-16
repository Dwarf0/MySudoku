#include <QFile>
#include <QTextStream>
#include <QBrush>

#include "SudokuModel.h"
#include "solver.h"

SudokuModel::SudokuModel(QWidget *parent) {
	_autocheckMode = false;
	_displayHelp = false;
	_sudoku = new Sudoku();
}

SudokuModel::~SudokuModel() {
}

void SudokuModel::displayHelp(bool displayHelp) 
{ 
	_displayHelp = displayHelp; 
}

void SudokuModel::applyFilter(int filterType)
{
	if (solver::updatePossibleValues(_sudoku, filterType)) {
		emit dataChanged(createIndex(0, 0), createIndex(8, 8));
	}
}

void SudokuModel::resetModelValues()
{
	_sudoku->reset();
}

void SudokuModel::updateCellsPossibleValues()
{
	solver::updatePossibleValues(_sudoku);
}

QVariant SudokuModel::data(const QModelIndex &index, int role) const
{ 
	int r = index.row();
	int c = index.column();
	int value = _sudoku->getCellValue(r, c);
	bool valid = _sudoku->isCellValid(r, c);
	bool isInitialValue = _sudoku->isCellInitialValue(r, c);
	QSet<int> possibleValues = _sudoku->getCellPossibleValues(r, c);

	if (role == Qt::DisplayRole) {
		if (value > 0) {
			return value;
		} else if (_displayHelp) {
			return data(index, SudokuModel::PossibleValuesRole);
		}
	} else if (role == Qt::TextColorRole) {
		if (value == 0 && _displayHelp) {
			return QColor(Qt::gray);
		}
	} else if (role == Qt::TextAlignmentRole) {
		return Qt::AlignCenter;
	} else if (role == Qt::BackgroundRole) {
		Qt::GlobalColor color = (r / SQUARE_SIZE + c / SQUARE_SIZE) % 2 ? Qt::lightGray : Qt::white;
		if (_autocheckMode && !isInitialValue && value > 0) {
			color = valid ? Qt::green : Qt::red;
		}
		return QBrush(color);
	} 
	// User Roles
	else if (role == SudokuModel::IsValidRole) {
		return valid;
	} else if (role == SudokuModel::IsInitialValueRole) {
		return isInitialValue;
	} else if (role == SudokuModel::CellValueRole) {
		return value;
	} else if (role == SudokuModel::PossibleValuesRole) {
		QStringList strList;
		foreach(int v, possibleValues) {
			strList.append(QString::number(v));
		}
		return strList.join(" ");
	}

	return QVariant();
}

bool SudokuModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	int r = index.row();
	int c = index.column();
	bool ok;
	int intVal = value.toInt(&ok);
	bool isIntVal = (ok && 1 <= intVal && intVal <= 9);
	bool isEmptyVal = value.toString().isEmpty();
	if (isIntVal || isEmptyVal) {
		_sudoku->setCellValue(r, c, isEmptyVal ? 0 : intVal);
		return true;
	}
	return false;
}

Qt::ItemFlags SudokuModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags f = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	if (!_sudoku->isCellInitialValue(index.row(), index.column())) {
		f |= Qt::ItemIsEditable;
	}
	return f;
}

int SudokuModel::loadFromCsv(QString csvPath)
{
	return _sudoku->loadFromCsv(csvPath);
}

void SudokuModel::initFromSudoku(const Sudoku * sudoku)
{
	*_sudoku = *sudoku;
}

bool SudokuModel::isValid()
{
	return _sudoku->isValid();
}

bool SudokuModel::isFilled()
{
	return _sudoku->isFilled();
}
