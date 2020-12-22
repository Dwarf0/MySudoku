#include <QFile>
#include <QTextStream>

#include "Sudoku.h"

Sudoku::Sudoku()
{
	_initMode = false;
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			_values[i][j] = new SudokuCell(i, j);
#ifdef _DEBUG
			_solutionValues[i][j] = new SudokuCell(i, j);
#endif
		}
	}
}

Sudoku::~Sudoku() {
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			delete _values[i][j];
#ifdef _DEBUG
			delete _solutionValues[i][j];
#endif
		}
	}
}


Sudoku& Sudoku::operator=(const Sudoku& other)
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (other.isCellInitialValue(i, j)) {
				setCellInitialValue(i, j, other.getCellValue(i, j));
			} else {
				setCellPossibleValues(i, j, other.getCellPossibleValues(i, j));
				setCellValue(i, j, other.getCellValue(i, j));
			}
		}
	}
	return *this;
}

bool Sudoku::isFilled() const
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (getCellValue(i, j) == 0) {
				return false;
			}
		}
	}
	return true;
}

bool Sudoku::isValid() const
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (!isCellValid(i, j)) {
				return false;
			}
		}
	}
	return true;
}

bool Sudoku::isCellValid(int r, int c) const
{
	return _values[r][c]->isValid();
}

void Sudoku::setCellValue(int r, int c, int val)
{ 
	if (_initMode) {
		_values[r][c]->setInitialValue(val);
	}
	else {
		/// TODO : faire un chek de isInitialValue avant et mettre un warning si on tente de modifier une valeur initiale ?
		_values[r][c]->setValue(val);
	}
	updateValidity(); 
}

void Sudoku::setCellPossibleValues(int r, int c, QSet<int> possibleValues)
{
	if (!isCellInitialValue(r, c))
		_values[r][c]->setPossibleValues(possibleValues);
	else {
		QString mess = QString("Cell at (%1,%2) holds initial value - Unable to set possible value.")
						.arg(QString::number(r), QString::number(c));
		qWarning(mess.toStdString().c_str());
	}
#ifdef _DEBUG
	checkForFilterError();
#endif
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

	enableInitMode();
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			setCellValue(i, j, values[i][j]);
		}
	}
	disableInitMode();

	updateValidity();

#ifdef _DEBUG
	QString solution = csvPath.left(csvPath.lastIndexOf('.')) + "_solution.csv";
	QFile csvSolution(solution);
	csvSolution.open(QFile::ReadOnly);
	QTextStream solTextStream(&csvSolution);
	rowIndex = 0;
	while (solTextStream.readLineInto(&line)) {
		if (line.isEmpty()) {
			continue;
		}

		bool ok = true;
		QStringList splitted = line.split(";");
		for (int colIndex = 0; colIndex < splitted.size(); ++colIndex) {
			int val = splitted[colIndex].toInt(&ok);
			_solutionValues[rowIndex][colIndex]->setInitialValue(val);
		}
		rowIndex++;
	}
	csvSolution.close();
#endif
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

void Sudoku::clean()
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (!_values[i][j]->isInitialValue())
				setCellValue(i, j, 0);
		}
	}
}

void Sudoku::setCellInitialValue(int r, int c, int val)
{ 
	_values[r][c]->setInitialValue(val); 
}

void Sudoku::updateValidity()
{
	for (int i_cell = 0; i_cell < SUDOKU_SIZE * SUDOKU_SIZE; ++i_cell) {
		int row = i_cell / SUDOKU_SIZE;
		int col = i_cell % SUDOKU_SIZE;
		int value = getCellValue(row, col);

		bool valid = true;
		if (value == 0) {
			continue;
		}

		for (int i = 0; i < SUDOKU_SIZE; ++i) {
			bool alreadyOnRow = getCellValue(row, i) == value && i != col;
			bool alreadyOnCol = getCellValue(i, col) == value && i != row;

			int squareRow = (row / 3) * 3 + i % 3;
			int squareCol = (col / 3) * 3 + i / 3;
			bool alreadyOnSquare = (getCellValue(squareRow, squareCol) == value) && !(squareRow == row && squareCol == col);
			if (alreadyOnRow || alreadyOnCol || alreadyOnSquare) {
				valid = false; 
				break;
			}
		}
		setCellValid(row, col, valid);
	}
}

QString Sudoku::toString() const
{
	QString str;
	for (int i = 0; i < SUDOKU_SIZE; i++) {
		QStringList l;
		for (int j = 0; j < SUDOKU_SIZE; j++) {
			l.append(QString::number(getCellValue(i, j)));
		}
		str.append(l.join(";"));
		str.append("\n");
	}
	return str.left(str.size() - 1);
}

void Sudoku::print() const
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

#ifdef _DEBUG
void Sudoku::checkForFilterError()
{
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			bool solutionPresent = false;
			int sol = _solutionValues[i][j]->getValue();
			if (! sol) {
				// case where solution's value couldn't be loaded
				continue;
			}
			foreach(int val, getCellPossibleValues(i, j)) {
				solutionPresent |= sol == val;
			}
			if (!solutionPresent) {
				
				QString mess = QString("Correct value %1  disappeared from possible value of cell (%2;%3) !")
								.arg(QString::number(sol), QString::number(i), QString::number(j));
				qWarning(mess.toStdString().c_str());
			}
		}
	}
}
#endif

