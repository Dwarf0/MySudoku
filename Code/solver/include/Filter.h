#ifndef FILTER_H
#define FILTER_H

#include "Sudoku.h"

/*!
 * Filter's main purpose is to provide a filter function (defined at construction) along with some description
 * about the filter it applies.
*/
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
		if (_filter) {
			return _filter(sudoku, row, col);
		}
		qWarning("Warning - Trying to use an uninitialised filter function!");
		return QSet<int>();
	}
	
private:
	QString _name;
	QString _briefDescription;
	QSet<int>(*_filter)(const Sudoku *sudoku, int row, int col);
};

#endif // FILTER_H