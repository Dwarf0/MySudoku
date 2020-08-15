#include <iostream>

#include <gtest/gtest.h>
#include "SudokuModel.h"

TEST(load_csv_testcase, load_csv_ok)
{
	SudokuModel model;
	QString path(TESTS_PATH);
	int err = model.loadFromCsv(path+"/data/hard.csv");
    EXPECT_EQ(0, err);
}

TEST(load_csv_testcase, load_csv_ko)
{
	SudokuModel model;
	QString path(TESTS_PATH);
	int err = model.loadFromCsv(path + "/data/hard_ko.csv");
	EXPECT_NE(0, err);
}
