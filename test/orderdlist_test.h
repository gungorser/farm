/*
 * list_test.h
 *
 *  Created on: Apr 19, 2020
 *      Author: user
 */
#include "../ordered_list.h"

#ifndef TEST_LIST_TEST_H_
#define TEST_LIST_TEST_H_

using namespace utils;

TEST(list, insert_empty) {
	ordered_list <int> l;
	EXPECT_EQ(l.end(), l.begin());
}

TEST(list, insert_one) {
	ordered_list <int> l;
	EXPECT_EQ(*l.insert(5), 5);
	EXPECT_EQ(*l.begin(), 5);
}

TEST(list, insert_left) {
	ordered_list <int> l;
	l.insert(5);
	EXPECT_EQ(*l.insert(3), 3);
	EXPECT_EQ(*l.begin(), 3);
}

TEST(list, insert_equal) {
	ordered_list <int> l;
	l.insert(5);
	EXPECT_EQ(*l.insert(5), 5);
	EXPECT_EQ(*l.begin(), 5);
}

TEST(list, insert_right) {
	ordered_list <int> l;
	l.insert(5);
	EXPECT_EQ(*l.insert(7), 7);
	EXPECT_EQ(*l.begin(), 5);
}

TEST(list, insert_middle) {
	ordered_list <int> l;
	l.insert(5);
	l.insert(9);
	EXPECT_EQ(*l.insert(7), 7);
	EXPECT_EQ(*l.begin(), 5);
}

TEST(list, insert_middle_equal) {
	ordered_list <int> l;
	l.insert(5);
	l.insert(9);
	l.insert(7);
	EXPECT_EQ(*l.insert(7), 7);
	EXPECT_EQ(*l.begin(), 5);
}

TEST(list, erase_empty) {
	ordered_list <int> l;
	l.erase(l.end());
}

TEST(list, erase_one) {
	ordered_list <int> l;
	auto it=l.insert(5);
	EXPECT_EQ(l.erase(it), l.end());
}

TEST(list, erase_first) {
	ordered_list <int> l;
	auto it=l.insert(5);
	auto it2=l.insert(7);
	EXPECT_EQ(l.erase(it), it2);
}

TEST(list, erase_last) {
	ordered_list <int> l;
	l.insert(5);
	auto it2=l.insert(7);
	EXPECT_EQ(l.erase(it2), l.end());
}

TEST(list, erase_middle) {
	ordered_list <int> l;
	l.insert(3);
	auto it=l.insert(5);
	auto it2=l.insert(7);
	EXPECT_EQ(l.erase(it), it2);
}

TEST(list, erase_equal) {
	ordered_list <int> l;
	auto it=l.insert(5);
	l.insert(5);
	EXPECT_EQ(l.erase(it), l.end());
}

#endif /* TEST_LIST_TEST_H_ */
