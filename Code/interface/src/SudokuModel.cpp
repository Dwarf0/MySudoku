#include "SudokuModel.h"

#include <QFile>
#include <QTextStream>
#include <QBrush>

SudokuModel::SudokuModel(QWidget *parent) {
	_autocheckMode = false;
	_displayHelp = false;
}

SudokuModel::~SudokuModel() {
}

QList<int> SudokuModel::getPossibleValues(int r, int c)
{
	QList<int> values;
	foreach(int v, _sudoku->getPossibleValues(r, c)) {
		values.append(v);
	}
	qSort(values);
	return values;
}

void SudokuModel::displayHelp(bool displayHelp) 
{ 
	_displayHelp = displayHelp; 
	// if(_displayHelp) updateCellsPossibleValues(); 
}

void SudokuModel::applyFilter(int filterType)
{
	if (_sudoku->applyFilter(filterType)) {
		emit dataChanged(createIndex(0, 0), createIndex(8, 8));
	}
}

void SudokuModel::disableCellsSignals()
{
	//for (int i = 0; i < SUDOKU_SIZE; ++i) {
	//	for (int j = 0; j < SUDOKU_SIZE; ++j) {
	//		_values[i][j]->disableEmittingSignals();
	//	}
	//}
}

void SudokuModel::enableCellsSignals()
{
	//for (int i = 0; i < SUDOKU_SIZE; ++i) {
	//	for (int j = 0; j < SUDOKU_SIZE; ++j) {
	//		_values[i][j]->enableEmittingSignals();
	//	}
	//}
}

void SudokuModel::resetModelValues()
{
	disableCellsSignals();
	_sudoku->reset();
	enableCellsSignals();
}

void SudokuModel::updateCellsPossibleValues()
{
	_sudoku->updatePossibleValues();
	//for (int i = 0; i < SUDOKU_SIZE; ++i) {
	//	for (int j = 0; j < SUDOKU_SIZE; ++j) {
	//		_sudoku->updatePossibleValues(); [i][j]->updatePossibleValues();
	//	}
	//}
}

QVariant SudokuModel::data(const QModelIndex &index, int role) const
{ 
	int r = index.row();
	int c = index.column();
	int value = _sudoku->getValue(r, c);
	bool valid = _sudoku->isValid(r, c);
	bool isInitialValue = _sudoku->isInitialValue(r, c);
	QList<int> possibleValues = _sudoku->getPossibleValues(r, c);

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
		_sudoku->setValue(r, c, isEmptyVal ? 0 : intVal);
		return true;
	}
	return false;
}

Qt::ItemFlags SudokuModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags f = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	if (!_sudoku->isInitialValue(index.row(), index.column())) {
		f |= Qt::ItemIsEditable;
	}
	return f;
}

int SudokuModel::loadFromCsv(QString csvPath)
{
	return _sudoku->loadFromCSV();
}

bool SudokuModel::isValid()
{
	return _sudoku->isValid();
}

bool SudokuModel::isFilled()
{
	return _sudoku->isFilled();
}

QString SudokuModel::toString() const
{
	QString str;
	for (int i = 0; i < SUDOKU_SIZE; i++) {
		QStringList l;
		for (int j = 0; j < SUDOKU_SIZE; j++) {
			l.append(QString::number(_sudoku->getValue(i, j)));
		}
		str.append(l.join(";"));
		str.append("\n");
	}
	return str.left(str.size() - 1);
}

void SudokuModel::print() const
{
	QString topSep = "|-----------------|"; // ┌┬┐ ╔╦╗
	QString sep = "|-+-+-+-+-+-+-+-+-|";	// ├┼┤ ╠╬╣ 
	QString botSep = "|-----------------|"; // └┴┘ ╚╩╝

	QString str = toString(); // | ║
	str.replace(';', '|');
	str.replace('\n', "|\n" + sep + "\n|");
	str = '|' + str + '|';

	std::cout << topSep.toStdString() << std::endl;
	std::cout << str.toStdString() << std::endl;
	std::cout << botSep.toStdString() << std::endl;
}
