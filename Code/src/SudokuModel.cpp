#include "SudokuModel.h"

#include <QFile>
#include <QTextStream>

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
		if (0 <= index.row() || index.row() < SUDOKU_SIZE || 0 <= index.column() || index.column() < SUDOKU_SIZE)
			return _values[index.row()][index.column()].value;
	} else if (role == Qt::TextAlignmentRole) {
		return Qt::AlignHCenter | Qt::AlignVCenter;
	} else if (role == Qt::BackgroundRole) {
		return (r / SQUARE_SIZE + c / SQUARE_SIZE) % 2 ? QBrush(Qt::lightGray) : QBrush(Qt::white);
	}
	return QVariant();
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
			if (!ok) {
				std::cout << "ERROR - csv value " << splitted[colIndex].toStdString()
							<< " at row " << rowIndex
							<< " of csv file " << csvPath.toStdString()
							<< " could not be converted to int"
							<< std::endl;
				csv.close();
				return 3;
			}
			_values[rowIndex][colIndex].value = splitted[colIndex].toInt();
			_values[rowIndex][colIndex].isInitialValue = true;
		}
		rowIndex++;
	}

	csv.close();

	if (!isValid()) {
		return 4;
	}
	return 0;
}

QString SudokuModel::toString()
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

void SudokuModel::print()
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
	bool valid = true;
	int curValue = _values[row][col].value;
	if (curValue == 0) {
		valid = true;
	}
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		bool onRow = _values[row][i].value == curValue && i != col;
		bool onCol = _values[i][col].value == curValue && i != row;
		if (onRow || onCol) {
			valid = false;
		}
	}

	for (int i = 0; i < SQUARE_SIZE; ++i) {
		for (int j = 0; j < SQUARE_SIZE; ++j) {
			int r = i + SQUARE_SIZE * int(row / SQUARE_SIZE);
			int c = j + SQUARE_SIZE * int(col / SQUARE_SIZE);
			if (_values[r][c].value == curValue && (r != row && c != col)) {
				valid = false;
			}
		}
	}
	_values[row][col].isValid = valid;
	return valid;
}
