#ifndef FILTER_H
#define FILTER_H

#include "Sudoku.h"

struct Filter
{
	Filter():
		_name(""), _briefDescription(""), _filter(nullptr)
	{}
	Filter(QString name, QString desc, QSet<int>(*filter)(const Sudoku *sudoku, int row, int col))
		: _name(name), _briefDescription(desc), _filter(filter)
	{}
	QString name() const { return _name; }
	QString description() const { return _briefDescription; }
	QSet<int> filter (const Sudoku *sudoku, int row, int col) const
	{
		return _filter(sudoku, row, col);
	}
	
private:
	QString _name;
	QString _briefDescription;
	QSet<int>(*_filter)(const Sudoku *sudoku, int row, int col);
};

#endif // FILTER_H