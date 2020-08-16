#include <iostream>

#include <gtest/gtest.h>
#include "SudokuModel.h"

TEST(load_csv_testcase, load_csv_ok)
{
	SudokuModel model;
	QString path(TESTS_PATH);
	int err = model.loadFromCsv(path+"/data/hard.csv");
    EXPECT_EQ(err, 0);
}

TEST(load_csv_testcase, load_csv_invalid)
{
	SudokuModel model;
	QString path(TESTS_PATH);
	int err = model.loadFromCsv(path + "/data/hard_ko_nb_value.csv");
	EXPECT_EQ(err, 2);
	err = model.loadFromCsv(path + "/data/hard_ko_row.csv");
	EXPECT_EQ(err, 4);
	err = model.loadFromCsv(path + "/data/hard_ko_col.csv");
	EXPECT_EQ(err, 4);
	err = model.loadFromCsv(path + "/data/hard_ko_square.csv");
	EXPECT_EQ(err, 4);
}
