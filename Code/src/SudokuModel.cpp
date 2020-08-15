#include "SudokuModel.h"

#include <QFile>
#include <QTextStream>

SudokuModel::SudokuModel(QWidget *parent) {
	for (int i = 0; i < 9; i++) {
		QVector<int> row;
		for (int j = 0; j < 9; j++) {
			row.append(0);
		}
		_values.append(row);
	}
}

SudokuModel::~SudokuModel() {
}

QVariant SudokuModel::data(const QModelIndex &index, int role) const
{ 
	if( 0 <= index.row() || index.row() < 9 || 0 <= index.column() || index.column() < 9)
		return _values[index.row()][index.column()];
	return QVariant();
}


void SudokuModel::loadFromCsv(QString csvPath)
{
	QFile csv(csvPath);
	if (!csv.exists()) {
		std::cerr << "ERROR - csv file not found at " << csvPath.toStdString().c_str() << std::endl;
		return;
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
			std::cerr << "ERROR - wrong number of values at row " << rowIndex << " of csv file " << csvPath.toStdString().c_str() << std::endl;
			return;
		}
		for (int colIndex = 0; colIndex < splitted.size(); ++colIndex) {
			bool ok=true;
			int val = splitted[colIndex].toInt(&ok);
			if (!ok) {
				std::cerr << "ERROR - csv value " << splitted[colIndex].toStdString().c_str() 
							<< " at row " << rowIndex
							<< " of csv file " << csvPath.toStdString().c_str()
							<< " could not be converted to int"
							<< std::endl;
				return;
			}
			_values[rowIndex][colIndex] = splitted[colIndex].toInt();
		}
		rowIndex++;
	}

	csv.close();
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
	return str;
}
