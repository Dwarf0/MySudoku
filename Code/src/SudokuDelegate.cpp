#include "SudokuDelegate.h"

#include <QPainter>
#include <QLineEdit>

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

QWidget * SudokuDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return new QLineEdit(parent);
}

void SudokuDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
	QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(editor);
	if (lineEdit) {
		QString val = index.data(SudokuModel::CellValueRole).toString();
		lineEdit->setText(val);
	}
}

void SudokuDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(editor);
	if (lineEdit) {
		QString val = lineEdit->text();
		model->setData(index, val);
	}
}
