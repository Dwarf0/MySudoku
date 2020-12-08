#include "SudokuView.h"

#include <QFile>
#include <QTextStream>
#include <QHeaderView>

SudokuView::SudokuView(QWidget *parent)
{
	horizontalHeader()->hide();
	verticalHeader()->hide();
	horizontalHeader()->setDefaultSectionSize(60);
	verticalHeader()->setDefaultSectionSize(60);
}

SudokuView::~SudokuView()
{
}
