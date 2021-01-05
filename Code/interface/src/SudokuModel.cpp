#include <QFile>
#include <QTextStream>
#include <QBrush>

#include "SudokuModel.h"

SudokuModel::SudokuModel(QWidget *parent) {
	_autocheckMode = false;
	_displayHelp = false;
	_sudokus.clear();
	_sudokus.push_back(QSharedPointer<Sudoku>(new Sudoku()));
	_curSudokuIdx = 0;
}

SudokuModel::~SudokuModel() {
}

void SudokuModel::setAutocheckMode(bool autocheck) 
{ 
	_autocheckMode = autocheck; 
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount()- 1));
}

void SudokuModel::displayHelp(bool displayHelp) 
{ 
	_displayHelp = displayHelp; 
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
}

void SudokuModel::resetModelValues()
{
	_sudokus.clear();
	_sudokus.push_back(QSharedPointer<Sudoku>(new Sudoku()));
	_curSudokuIdx = 0;

	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
}

void SudokuModel::addNewSudoku()
{
	if (_curSudokuIdx < _sudokus.size() - 1)
		_sudokus.erase(_sudokus.begin() + _curSudokuIdx, _sudokus.end());
	_sudokus.push_back(QSharedPointer<Sudoku>(new Sudoku(_sudokus[_curSudokuIdx].data())));
	_curSudokuIdx = _sudokus.size() - 1;
}

QVariant SudokuModel::data(const QModelIndex &index, int role) const
{
	int r = index.row();
	int c = index.column();
	int value = _sudokus[_curSudokuIdx]->getCellValue(r, c);
	bool valid = _sudokus[_curSudokuIdx]->isCellValid(r, c);
	bool isInitialValue = _sudokus[_curSudokuIdx]->isCellInitialValue(r, c);
	QSet<int> possibleValues = _sudokus[_curSudokuIdx]->getCellPossibleValues(r, c);

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
		QList<int> intList;
		foreach(int v, possibleValues) {
			intList.append(v);
		}
		qSort<QList<int> >(intList);
		//qsort(intList);
		QStringList strList;
		foreach(int v, intList) {
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
		addNewSudoku();
		_sudokus[_curSudokuIdx]->setCellValue(r, c, isEmptyVal ? 0 : intVal);
		return true;
	}
	return false;
}

Qt::ItemFlags SudokuModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags f = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	if (!_sudokus[_curSudokuIdx]->isCellInitialValue(index.row(), index.column())) {
		f |= Qt::ItemIsEditable;
	}
	return f;
}

int SudokuModel::loadFromCsv(QString csvPath)
{
	_sudokus.clear();

	QSharedPointer<Sudoku> s(new Sudoku());
	int err = s->loadFromCsv(csvPath);
	if (!err) {
		_sudokus.push_back(s);
		_curSudokuIdx = _sudokus.size() - 1;
	}
	
	return err;
}

void SudokuModel::initFromSudoku(const Sudoku * sudoku)
{
	QSharedPointer<Sudoku> s(new Sudoku());
	_sudokus.clear();

	s->enableInitMode();
	*(s.data()) = *sudoku;
	s->disableInitMode();

	_sudokus.push_back(s);
	_curSudokuIdx = _sudokus.size() - 1;
}

bool SudokuModel::isValid() const
{
	return _sudokus[_curSudokuIdx]->isValid();
}

bool SudokuModel::isFilled() const
{
	return _sudokus[_curSudokuIdx]->isFilled();
}

void SudokuModel::autofill()
{
	addNewSudoku();
	_sudokus[_curSudokuIdx]->autofill();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
}

void SudokuModel::applyFilter(int filterType)
{
	addNewSudoku();
	if (solver.updatePossibleValues(_sudokus[_curSudokuIdx].data(), filterType)) {
		emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
	}
}

void SudokuModel::solve()
{
	addNewSudoku();
	solver.solve(_sudokus[_curSudokuIdx].data());
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
}
