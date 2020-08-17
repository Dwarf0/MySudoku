#include "SudokuDelegate.h"

#include <QPainter>

SudokuDelegate::SudokuDelegate(QWidget *parent)
{
}

SudokuDelegate::~SudokuDelegate()
{
}

void SudokuDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItem opt = option;
	initStyleOption(&opt, index);

	bool isInitialValue = index.data(SudokuModel::IsInitialValueRole).toBool();

	opt.font.setBold(isInitialValue);
	
	QStyledItemDelegate::paint(painter, opt, index);
}
