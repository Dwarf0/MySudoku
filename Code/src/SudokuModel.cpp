#include "SudokuModel.h"

#include <QFile>
#include <QTextStream>
#include <QBrush>

SudokuModel::SudokuModel(QWidget *parent) {
	resetModelValues();
	_autocheckMode = false;
	_displayHelp = false;
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
			_values[i][j].possibleValues = { 1, 2, 3, 4 ,5 ,6 , 7, 8, 9 };
		}
	}
}

QVariant SudokuModel::data(const QModelIndex &index, int role) const
{ 
	int r = index.row();
	int c = index.column();
	SudokuCell cell = _values[index.row()][index.column()];
	int value = cell.value;
	bool valid = cell.isValid;
	bool isInitialValue = cell.isInitialValue;

	if (role == Qt::DisplayRole) {
		if (value > 0) {
			return value;
		} else if (_displayHelp) {
			QList<int> values;
			foreach(int v, cell.possibleValues) {
				values.append(v);
			}
			qSort(values);

			QStringList strList;
			foreach(int v, values) {
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
		setCellValue(intVal, r, c);

		// TODO : gérer une liste qui contient les cells vides plutôt que tout parcourir ?
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				updateCellPossibleValuesAt(i, j);
			}
		}
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

	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			updateCellPossibleValuesAt(i, j);
		}
	}

	return 0;
}

void SudokuModel::updateCellValidAt(int row, int col)
{
	bool valid = true;
	int value = _values[row][col].value;
	if (value != 0) {
		for (int i = 0; i < SUDOKU_SIZE; ++i) {
			bool alreadyOnRow = _values[row][i].value == value && i != col;
			bool alreadyOnCol = _values[i][col].value == value && i != row;

			int squareRow = (row / 3) * 3 + i % 3;
			int squareCol = (col / 3) * 3 + i / 3;
			bool alreadyOnSquare = _values[squareRow][squareCol].value == value && squareRow != row && squareCol != col;
			if (alreadyOnRow || alreadyOnCol || alreadyOnSquare) {
				valid = false;
				break;
			}
		}
	}

	_values[row][col].isValid = valid;
}

void SudokuModel::updateCellPossibleValuesAt(int row, int col)
{
	if (_values[row][col].value > 0) {
		_values[row][col].possibleValues.clear();
	}
	else {
		_values[row][col].possibleValues = { 1, 2, 3, 4 ,5 ,6 , 7, 8, 9 };
		for (int i = 0; i < SUDOKU_SIZE; ++i) {
			_values[row][col].possibleValues.remove(_values[row][i].value);
			_values[row][col].possibleValues.remove(_values[i][col].value);

			int squareRow = (row / 3) * 3 + i % 3;
			int squareCol = (col / 3) * 3 + i / 3;
			_values[row][col].possibleValues.remove(_values[squareRow][squareCol].value);
		}
	}
}

void SudokuModel::setCellValue(int value, int row, int col)
{
	_values[row][col].value = value;
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		updateCellValidAt(i, col);
		updateCellValidAt(row, i);

		int squareRow = (row / 3) * 3 + i % 3;
		int squareCol = (col / 3) * 3 + i / 3;
		updateCellValidAt(squareRow, squareCol);;
	}
}


bool SudokuModel::isValid()
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (!_values[i][j].isValid)
				return false;
		}
	}
	return true;
}

bool SudokuModel::isFilled()
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (_values[i][j].value == 0) {
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
