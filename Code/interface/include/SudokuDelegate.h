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
	QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // SUDOKUDELEGATE_H
