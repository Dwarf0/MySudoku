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

TEST(create_combination_testcase, with_4_elts)
{
	QList<QSet<int> > output;
	createCombinations(QSet<int>(), QSet<int>({ 1, 2, 3, 4 }), output);
	EXPECT_EQ(output.size(), 15);
}
