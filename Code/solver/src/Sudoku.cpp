#include <QFile>
#include <QTextStream>

#include "Sudoku.h"

Sudoku::Sudoku()
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			_values[i][j] = new SudokuCell(i, j);
		}
	}
}

Sudoku::~Sudoku() {
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			delete _values[i][j];
		}
	}
}

void Sudoku::setPossibleValues(int r, int c, QSet<int> possibleValues)
{
	_values[r][c]->setPossibleValues(possibleValues);
}

int Sudoku::loadFromCsv(QString csvPath)
{

	reset();

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
			reset();
			return 2;
		}
		bool ok = true;
		for (int colIndex = 0; colIndex < splitted.size(); ++colIndex) {
			int val = splitted[colIndex].toInt(&ok);
			if (!ok || val < 0 || val > 9) {
				std::cout 
					<< "ERROR - csv value " << splitted[colIndex].toStdString()
					<< " at row " << rowIndex
					<< " of csv file " << csvPath.toStdString()
					<< " could not be converted to int"
					<< std::endl;
				csv.close();
				reset();
				return 3;
			}
			setInitialValue(rowIndex, colIndex, val);
		}
		rowIndex++;
	}
	csv.close();

	return 0;
}

void Sudoku::reset()
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			_values[i][j]->reset();
		}
	}
}

bool Sudoku::isValid() 
{
	bool valid = true;
	for (int i_cell = 0; i_cell < SUDOKU_SIZE * SUDOKU_SIZE; ++i_cell) {
		int row = i_cell / SUDOKU_SIZE;
		int col = i_cell % SUDOKU_SIZE;
		int value = getValue(row, col);
		for (int i = 0; i < SUDOKU_SIZE; ++i) {

			bool alreadyOnRow = getValue(row, i) == value && i != col;
			bool alreadyOnCol = getValue(i, col) == value && i != row;

			int squareRow = (row / 3) * 3 + i % 3;
			int squareCol = (col / 3) * 3 + i / 3;
			bool alreadyOnSquare = (getValue(squareRow, squareCol) == value) && !(squareRow == row && squareCol == col);
			if (alreadyOnRow || alreadyOnCol || alreadyOnSquare) {
				valid = false;
				setValid(row, col, false);
			}
			else {
				setValid(row, col, true);
			}
		}
	}
	return valid;
}

bool Sudoku::isFilled()
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (_values[i][j]->getValue() == 0) {
				return false;
			}
		}
	}
	return true;
}
