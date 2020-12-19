#include <iostream>

#include <gtest/gtest.h>
#include "Sudoku.h"
#include "solver.h"

TEST(load_csv_testcase, load_csv_ok)
{
	Sudoku sudoku;
	QString path(TESTS_PATH);
	int err = sudoku.loadFromCsv(path+"/data/hard.csv");
    EXPECT_EQ(err, 0);
	EXPECT_EQ(sudoku.isValid(), true);
}

TEST(load_csv_testcase, load_csv_invalid)
{
	Sudoku sudoku;
	QString path(TESTS_PATH);
	int err = 0;
	
	err = sudoku.loadFromCsv(path + "/data/hard_ko_nb_value.csv");
	EXPECT_EQ(err, 2);
	EXPECT_EQ(sudoku.isValid(), true); // valid à true car on reset après un load fail 

	err = sudoku.loadFromCsv(path + "/data/hard_ko_row.csv");
	EXPECT_EQ(err, 0);
	EXPECT_EQ(sudoku.isValid(), false);

	err = sudoku.loadFromCsv(path + "/data/hard_ko_col.csv");
	EXPECT_EQ(err, 0);
	EXPECT_EQ(sudoku.isValid(), false);

	err = sudoku.loadFromCsv(path + "/data/hard_ko_square.csv");
	EXPECT_EQ(err, 0);
	EXPECT_EQ(sudoku.isValid(), false);
}

TEST(filter_testcase, direct_filter)
{
	Sudoku sudoku;
	QString path(TESTS_PATH);
	int cellRow = 0;
	int cellCol = 1;

	int err = sudoku.loadFromCsv(path + "/data/easy.csv");
	EXPECT_EQ(err, 0);
	EXPECT_EQ(sudoku.isValid(), true);

	QSet<int> all = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	EXPECT_EQ(all, sudoku.getCellPossibleValues(cellRow, cellCol));
	solver::updatePossibleValues(&sudoku, solver::Direct);

	QSet<int> direct = { 3, 9 };
	EXPECT_EQ(direct, sudoku.getCellPossibleValues(cellRow, cellCol));
}

TEST(filter_testcase, indirect_filter)
{
	Sudoku sudoku;
	QString path(TESTS_PATH);
	int cellRow = 5;
	int cellCol = 6;

	int err = sudoku.loadFromCsv(path + "/data/easy.csv");
	EXPECT_EQ(err, 0);
	EXPECT_EQ(sudoku.isValid(), true);

	QSet<int> all = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	EXPECT_EQ(all, sudoku.getCellPossibleValues(cellRow, cellCol));

	solver::updatePossibleValues(&sudoku, solver::Direct);

	QSet<int> direct = { 2, 5, 6, 8, 9 };
	EXPECT_EQ(direct, sudoku.getCellPossibleValues(cellRow, cellCol));

	solver::updatePossibleValues(&sudoku, solver::Indirect);

	QSet<int> indirect = { 8, 9 };
	EXPECT_EQ(indirect, sudoku.getCellPossibleValues(cellRow, cellCol));
}

TEST(filter_testcase, group_filter)
{
	// After the Direct filter, cells (8, 0) and (8, 1) both holds {2, 5} as possible values and form a group
	Sudoku sudoku;
	QString path(TESTS_PATH);
	int cellRow = 7;
	int cellCol = 0;

	int err = sudoku.loadFromCsv(path + "/data/easy.csv");
	EXPECT_EQ(err, 0);
	EXPECT_EQ(sudoku.isValid(), true);

	QSet<int> all = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	EXPECT_EQ(all, sudoku.getCellPossibleValues(cellRow, cellCol));

	solver::updatePossibleValues(&sudoku, solver::Direct);

	QSet<int> direct = { 5, 6 };
	EXPECT_EQ(direct, sudoku.getCellPossibleValues(cellRow, cellCol));

	solver::updatePossibleValues(&sudoku, solver::Group);

	QSet<int> group = { 6 };
	EXPECT_EQ(group, sudoku.getCellPossibleValues(cellRow, cellCol));
}

TEST(filter_testcase, create_combination_testcase)
{
	QList<QSet<int> > output;
	solver::createCombinations(QSet<int>(), QSet<int>({ 1, 2, 3, 4 }), output);
	EXPECT_EQ(output.size(), 15);
}

TEST(filter_testcase, hidden_group_filter)
{
	// After the Direct filter, cells (0, 2) and (0, 3) are the only cells int the row that can hold {1, 6} 
	// as possible values and form a group
	Sudoku sudoku;
	QString path(TESTS_PATH);
	int cellRow1 = 0;
	int cellCol1 = 2;
	int cellRow2 = 0;
	int cellCol2 = 3;

	int err = sudoku.loadFromCsv(path + "/data/easy.csv");
	EXPECT_EQ(err, 0);
	EXPECT_EQ(sudoku.isValid(), true);

	QSet<int> all = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	EXPECT_EQ(all, sudoku.getCellPossibleValues(cellRow1, cellCol1));
	EXPECT_EQ(all, sudoku.getCellPossibleValues(cellRow2, cellCol2));

	solver::updatePossibleValues(&sudoku, solver::Direct);

	QSet<int> direct1 = { 1, 6, 9 };
	EXPECT_EQ(direct1, sudoku.getCellPossibleValues(cellRow1, cellCol1));
	QSet<int> direct2 = { 1, 3, 6, 9 };
	EXPECT_EQ(direct2, sudoku.getCellPossibleValues(cellRow2, cellCol2));

	solver::updatePossibleValues(&sudoku, solver::HiddenGroup);

	QSet<int> hiddenGroup = { 1, 6 };
	EXPECT_EQ(hiddenGroup, sudoku.getCellPossibleValues(cellRow1, cellCol1));
	EXPECT_EQ(hiddenGroup, sudoku.getCellPossibleValues(cellRow2, cellCol2));
}

TEST(filter_testcase, nochoice_filter)
{
	// After the Direct filter, cell (0, 2) is the only one that can holds 1 in its column
	Sudoku sudoku;
	QString path(TESTS_PATH);
	int cellRow = 0;
	int cellCol = 2;

	int err = sudoku.loadFromCsv(path + "/data/easy.csv");
	EXPECT_EQ(err, 0);
	EXPECT_EQ(sudoku.isValid(), true);

	QSet<int> all = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	EXPECT_EQ(all, sudoku.getCellPossibleValues(cellRow, cellCol));

	solver::updatePossibleValues(&sudoku, solver::Direct);

	QSet<int> direct = { 1, 6, 9 };
	EXPECT_EQ(direct, sudoku.getCellPossibleValues(cellRow, cellCol));

	solver::updatePossibleValues(&sudoku, solver::NoChoice);

	QSet<int> indirect = { 1 };
	EXPECT_EQ(indirect, sudoku.getCellPossibleValues(cellRow, cellCol));
}

