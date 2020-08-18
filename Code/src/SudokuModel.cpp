#include "SudokuModel.h"

#include <QFile>
#include <QTextStream>
#include <QBrush>

SudokuModel::SudokuModel(QWidget *parent) {
	resetModelValues();
	_autocheckMode = false;
	_displayHelp = false;
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			_values[i][j].setRow(i);
			_values[i][j].setColumn(j);
			_values[i][j].setModel(this);
		}
	}
}

SudokuModel::~SudokuModel() {
}

void SudokuModel::resetModelValues()
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			_values[i][j].reset();
		}
	}
}

QVariant SudokuModel::data(const QModelIndex &index, int role) const
{ 
	int r = index.row();
	int c = index.column();
	SudokuCell cell = _values[index.row()][index.column()];
	int value = cell.getValue();
	bool valid = cell.isValid();
	bool isInitialValue = cell.isInitialValue();

	if (role == Qt::DisplayRole) {
		if (value > 0) {
			return value;
		} else if (_displayHelp) {
			QStringList strList;
			foreach(int v, cell.getPossibleValues()) {
				strList.append(QString::number(v));
			}
			return strList.join(" ");
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
	} else if (role == SudokuModel::IsValidRole) {
		return valid;
	} else if (role == SudokuModel::IsInitialValueRole) {
		return isInitialValue;
	} else if (role == SudokuModel::CellValueRole) {
		return value;
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
	std::cout << value.toString().toStdString() << std::endl;
	if (isIntVal || isEmptyVal) {
		updateCell(isEmptyVal ? 0 : intVal, r, c);
		return true;
	}
	return false;
}

Qt::ItemFlags SudokuModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags f = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	if (!_values[index.row()][index.column()].isInitialValue()) {
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
			_values[rowIndex][colIndex].setValue(val);
			if (val) {
				_values[rowIndex][colIndex].setInitialValue(true);
			}
		}
		rowIndex++;
	}

	csv.close();

	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			_values[i][j].updatePossibleValues();
		}
	}

	return 0;
}

void SudokuModel::updateCell(int value, int row, int col)
{
	_values[row][col].setValue(value);
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		_values[i][col].updateIsValid();
		_values[row][i].updateIsValid();

		int squareRow = (row / 3) * 3 + i % 3;
		int squareCol = (col / 3) * 3 + i / 3;
		_values[squareRow][squareCol].updateIsValid();

		_values[i][col].updatePossibleValues();
		_values[row][i].updatePossibleValues();
		_values[squareRow][squareCol].updatePossibleValues();
	}
}


bool SudokuModel::isValid()
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (!_values[i][j].isValid())
				return false;
		}
	}
	return true;
}

bool SudokuModel::isFilled()
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (_values[i][j].getValue() == 0) {
				return false;
			}
		}
	}
	return true;
}

QString SudokuModel::toString() const
{
	QString str;
	for (int i = 0; i < SUDOKU_SIZE; i++) {
		QStringList l;
		for (int j = 0; j < SUDOKU_SIZE; j++) {
			l.append(QString::number(_values[i][j].getValue()));
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
