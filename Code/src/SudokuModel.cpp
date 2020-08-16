﻿#include "SudokuModel.h"

#include <QFile>
#include <QTextStream>

SudokuModel::SudokuModel(QWidget *parent) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			_values[i][j] = 0;
		}
	}
}

SudokuModel::~SudokuModel() {
}

QVariant SudokuModel::data(const QModelIndex &index, int role) const
{ 
	int r = index.row();
	int c = index.column();

	if (role == Qt::DisplayRole) {
		if (0 <= index.row() || index.row() < 9 || 0 <= index.column() || index.column() < 9)
			return _values[index.row()][index.column()];
	} else if (role == Qt::TextAlignmentRole) {
		return Qt::AlignHCenter | Qt::AlignVCenter;
	} else if (role == Qt::BackgroundRole) {
		return (r / 3 + c / 3) % 2 ? QBrush(Qt::lightGray) : QBrush(Qt::white);
	}
	return QVariant();
}


int SudokuModel::loadFromCsv(QString csvPath)
{
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
		if (splitted.size() != 9) {
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
			_values[rowIndex][colIndex] = splitted[colIndex].toInt();
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
	for (int i = 0; i < 9; i++) {
		QStringList l;
		for (int j = 0; j < 9; j++) {
			l.append(QString::number(_values[i][j]));
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
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			if (!valueIsValid(i, j))
				return false;
		}
	}
	return true;
}

bool SudokuModel::valueIsValid(int row, int col)
{
	int curValue = _values[row][col];
	if (curValue == 0) {
		return true;
	}
	for (int i = 0; i < 9; ++i) {
		bool onRow = _values[row][i] == curValue && i != col;
		bool onCol = _values[i][col] == curValue && i != row;
		if (onRow || onCol) {
			return false;
		}
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			int r = i + 3 * int(row / 3);
			int c = j + 3 * int(col / 3);
			if (_values[r][c] == curValue && (r != row && c != col)) {
				return false;
			}
		}
	}
	return true;
}
