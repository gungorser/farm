/*
 * queue_test.h
 *
 *  Created on: Apr 19, 2020
 *      Author: user
 */
#include "../queue.h"
#ifndef TEST_QUEUE_TEST_H_
#define TEST_QUEUE_TEST_H_

using namespace utils;

TEST(queue, push_empty) {
	queue <int> q;
	q.push(1);
	EXPECT_EQ(q.front(), 1);
}

TEST(queue, push_many) {
	queue <int> q;
	int test[]={1,2,3};
	for (auto& i: test) {q.push(i);}
	EXPECT_FALSE(q.empty());
	EXPECT_EQ(q.size(), 3);
	for (auto& i: test){
		EXPECT_EQ(q.front(), i);
		q.pop();
	}
	EXPECT_TRUE(q.empty());
	EXPECT_EQ(q.size(), 0);
}

#endif /* TEST_QUEUE_TEST_H_ */
