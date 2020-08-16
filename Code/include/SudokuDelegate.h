#ifndef SUDOKUDELEGATE_H
#define SUDOKUDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <iostream>

#include "SudokuModel.h"

class SudokuDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	SudokuDelegate(QWidget *parent = nullptr);
	~SudokuDelegate();
	
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
};

#endif // SUDOKUDELEGATE_H
