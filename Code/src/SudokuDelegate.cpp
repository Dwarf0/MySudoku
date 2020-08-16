#include "SudokuDelegate.h"

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

	QVariant data = index.data(Qt::DisplayRole);
	opt.font.setBold(data.toBool());
	
	QStyledItemDelegate::paint(painter, opt, index);
}