#include <QApplication>
#include "MySudokuViewer.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	MySudokuViewer sudokuViewer;
	sudokuViewer.show();
    
	return app.exec();
}
