#ifndef SUDOKUCELL_H
#define SUDOKUCELL_H

#include <QObject>
#include <iostream>
#include <qset.h>

#include "globals.h"

class SudokuModel;

class SudokuCell : public QObject
{
	Q_OBJECT
public:
	SudokuCell(SudokuModel *model = nullptr);
	~SudokuCell();

	void setRow(int row) { _row = row; };
	void setColumn(int col) { _col = col; };
	void setModel(SudokuModel *model) { _model = model; };
	void setInitialValue(int isInitialValue) { _isInitialValue = isInitialValue; };
	void setValue(int value);
	void reset();

	int getValue() const { return _value; }
	int isValid() const { return _isValid; }
	int isInitialValue() const { return _isInitialValue; }
	QList<int> getPossibleValues() const;

	void updateIsValid();
	void updatePossibleValues();

signals:
	void valueChanged(int value);

public slots:
	void updateCell();

private:
	bool _isInitialValue;
	bool _isValid;
	int _value;
	QSet<int> _possibleValues;

	int _row;
	int _col;
	SudokuModel *_model;
};

#endif // SUDOKUCELL_H
