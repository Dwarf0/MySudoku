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

void Sudoku::setValue(int r, int c, int val)
{ 
	_values[r][c]->setValue(val); 
	updateValidity(); 
}

void Sudoku::setPossibleValues(int r, int c, QSet<int> possibleValues)
{
	_values[r][c]->setPossibleValues(possibleValues);
}

int Sudoku::loadFromCsv(QString csvPath)
{
	// Check file existence
	QFile csv(csvPath);
	if (!csv.exists()) {
		std::cout << "Sudoku loading error: csv file not found at " << csvPath.toStdString() << std::endl;
		return 1;
	}

	csv.open(QFile::ReadOnly);
	QTextStream textStream(&csv);
	QString line;
	int rowIndex = 0;
	int values[SUDOKU_SIZE][SUDOKU_SIZE];
	
	while (textStream.readLineInto(&line)) {
		if (line.isEmpty()) {
			continue;
		}

		// Check number of columns
		QStringList splitted = line.split(";");
		if (splitted.size() != SUDOKU_SIZE) {
			std::cout 
				<< "Sudoku loading error: " 
				<< "wrong number of values at row " << rowIndex 
				<< " of csv file " << csvPath.toStdString() 
				<< std::endl;
			csv.close();
			reset();
			return 2;
		}

		bool ok = true;
		for (int colIndex = 0; colIndex < splitted.size(); ++colIndex) {
			int val = splitted[colIndex].toInt(&ok);
			// Check values
			if (!ok || val < 0 || val > 9) {
				std::cout 
					<< "Sudoku loading error: " 
					<< "csv value " << splitted[colIndex].toStdString()
					<< " at row " << rowIndex
					<< " of csv file " << csvPath.toStdString()
					<< " could not be converted to int"
					<< std::endl;
				csv.close();
				return 3;
			}
			values[rowIndex][colIndex] = val;
		}
		rowIndex++;
	}
	csv.close();

	// here, csv is valid and its values are loaded in the variable "values"
	reset();

	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			setInitialValue(i, j, values[i][j]);
		}
	}

	updateValidity();

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

void Sudoku::updateValidity()
{
	for (int i_cell = 0; i_cell < SUDOKU_SIZE * SUDOKU_SIZE; ++i_cell) {
		int row = i_cell / SUDOKU_SIZE;
		int col = i_cell % SUDOKU_SIZE;
		int value = getValue(row, col);

		bool valid = true;
		if (value == 0) {
			continue;
		}

		for (int i = 0; i < SUDOKU_SIZE; ++i) {
			bool alreadyOnRow = getValue(row, i) == value && i != col;
			bool alreadyOnCol = getValue(i, col) == value && i != row;

			int squareRow = (row / 3) * 3 + i % 3;
			int squareCol = (col / 3) * 3 + i / 3;
			bool alreadyOnSquare = (getValue(squareRow, squareCol) == value) && !(squareRow == row && squareCol == col);
			if (alreadyOnRow || alreadyOnCol || alreadyOnSquare) {
				valid = false; 
				break;
			}
		}
		setValid(row, col, valid);
	}
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

bool Sudoku::isValid()
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (!_values[i][j]->isValid()) {
				return false;
			}
		}
	}
	return true;
}
