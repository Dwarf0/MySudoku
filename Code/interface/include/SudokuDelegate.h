#ifndef SUDOKUDELEGATE_H
#define SUDOKUDELEGATE_H

#include <QObject>
#include <QLineEdit>
#include <QFocusEvent>
#include <QEvent>
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

private:

	class SingleValueLineEdit : public QLineEdit
	{
	public:
		SingleValueLineEdit(QWidget *parent = nullptr) : QLineEdit(parent) {  }
		~SingleValueLineEdit() {}
	private:
		void keyPressEvent(QKeyEvent *event) override
		{
			QLineEdit::keyPressEvent(event);
			focusNextChild();
		}
	};
};



#endif // SUDOKUDELEGATE_H
