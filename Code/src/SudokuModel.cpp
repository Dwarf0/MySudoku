#include "SudokuModel.h"

#include <QFile>
#include <QTextStream>
#include <QBrush>

SudokuModel::SudokuModel(QWidget *parent) {
	resetModelValues();
}

SudokuModel::~SudokuModel() {
}

void SudokuModel::resetModelValues()
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			_values[i][j].value = 0;
			_values[i][j].isInitialValue = false;
			_values[i][j].isValid = false;
		}
	}
}

QVariant SudokuModel::data(const QModelIndex &index, int role) const
{ 
	int r = index.row();
	int c = index.column();

	if (role == Qt::DisplayRole) {
		if (0 <= index.row() || index.row() < SUDOKU_SIZE || 0 <= index.column() || index.column() < SUDOKU_SIZE) {
			return _values[index.row()][index.column()].value;
		}
	} else if (role == Qt::TextAlignmentRole) {
		return Qt::AlignCenter;
	} else if (role == Qt::BackgroundRole) {
		return (r / SQUARE_SIZE + c / SQUARE_SIZE) % 2 ? QBrush(Qt::lightGray) : QBrush(Qt::white);
	}
	return QVariant();
}

bool SudokuModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	int r = index.row();
	int c = index.column();
	bool ok;
	int intVal = value.toInt(&ok);
	if (ok && 1 <= intVal && intVal <= 9) {
		_values[r][c].value = intVal;
		_values[r][c].isValid = cellIsValid(r, c);
		return true;
	}

	return false;
}

Qt::ItemFlags SudokuModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags f = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	if (!_values[index.row()][index.column()].isInitialValue) {
		f |= Qt::ItemIsEditable;
	}
	return f;
}

int SudokuModel::loadFromCsv(QString csvPath)
{
	resetModelValues();

	QFile csv(csvPath);
	if (!csv.exists()) {
		std::cout << "ERROR - csv file not found at " << csvPath.toStdString() << std::endl;
		return 1;
	}
	csv.open(QFile::ReadOnly);
	QTextStream textStream(&csv);
	QString line;
	int rowIndex = 0;
	while (textStream.readLineInto(&line)) {
		if (line.isEmpty()) {
			continue;
		}
		
		QStringList splitted = line.split(";");
		if (splitted.size() != SUDOKU_SIZE) {
			std::cout << "ERROR - wrong number of values at row " << rowIndex << " of csv file " << csvPath.toStdString() << std::endl;
			csv.close();
			return 2;
		}
		bool ok = true;
		for (int colIndex = 0; colIndex < splitted.size(); ++colIndex) {
			int val = splitted[colIndex].toInt(&ok);
			if (!ok || val < 0 || val > 9) {
				std::cout << "ERROR - csv value " << splitted[colIndex].toStdString()
							<< " at row " << rowIndex
							<< " of csv file " << csvPath.toStdString()
							<< " could not be converted to int"
							<< std::endl;
				csv.close();
				return 3;
			}
			_values[rowIndex][colIndex].value = val;
			if (val) {
				_values[rowIndex][colIndex].isInitialValue = true;
			}
		}
		rowIndex++;
	}

	csv.close();

	if (!isValid()) {
		return 4;
	}
	return 0;
}

QString SudokuModel::toString() const
{
	QString str;
	for (int i = 0; i < SUDOKU_SIZE; i++) {
		QStringList l;
		for (int j = 0; j < SUDOKU_SIZE; j++) {
			l.append(QString::number(_values[i][j].value));
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

bool SudokuModel::isValid()
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (!cellIsValid(i, j))
				return false;
		}
	}
	return true;
}

bool SudokuModel::cellIsValid(int row, int col)
{
	_values[row][col].isValid = isValueValidAt(_values[row][col].value, row, col);
	return _values[row][col].isValid;
}


bool SudokuModel::isValueValidAt(int value, int row, int col)
{
	if (value == 0) {
		return true;
	}
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		bool alreadyOnRow = _values[row][i].value == value && i != col;
		bool alreadyOnCol = _values[i][col].value == value && i != row;
		if (alreadyOnRow || alreadyOnCol) {
			return false;
		}
	}

	for (int i = 0; i < SQUARE_SIZE; ++i) {
		for (int j = 0; j < SQUARE_SIZE; ++j) {
			int r = i + SQUARE_SIZE * int(row / SQUARE_SIZE);
			int c = j + SQUARE_SIZE * int(col / SQUARE_SIZE);
			if (_values[r][c].value == value && (r != row && c != col)) {
				return false;
			}
		}
	}
	return true;
}
