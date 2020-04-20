/*
 * async_test.h
 *
 *  Created on: Apr 19, 2020
 *      Author: user
 */

#include <mutex>
#include <thread>
#include <unistd.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../async.h"

#ifndef TEST_ASYNC_TEST_H_
#define TEST_ASYNC_TEST_H_

template<typename T>
class ordered_list_mock : public utils::ordered_list<T>{
	using iterator_t = typename utils::ordered_list<T>::iterator;
public:
	MOCK_METHOD1_T(insert, iterator_t(const T &data));
	MOCK_METHOD1_T(erase, iterator_t(iterator_t));

	utils::ordered_list<T> m_real;
};

template<typename T>
class queue_mock : public utils::queue<T>{
public:
	MOCK_METHOD1_T(push, void(const T& t));
	MOCK_METHOD0_T(pop, void());
};

class async_mock : public utils::async{
public:
	async_mock(ordered_list<async::timer_t>& timers,
			queue<async::handler_t>& handlers)
		: async(timers, handlers)
	{}

	MOCK_CONST_METHOD0(millis, unsigned long ());

};

TEST(async, async_wait){
	using ::testing::Return;
	ordered_list_mock<async::timer_t> timers;
	queue<async::handler_t> handlers;
	async_mock async{timers, handlers};
	void (*handler)()=[](){};
	EXPECT_CALL(async, millis()).WillOnce(Return(10));
	EXPECT_CALL(timers, insert(async::timer_t{110, handler}));
	async.async_wait(100, handler);
}

TEST(async, consume_timer){
	using ::testing::Return;
	using ::testing::Invoke;
	using ::testing::DoAll;
	using ::testing::SaveArg;
	using ::testing::_;
	using orderd_list_real = utils::ordered_list<utils::async::timer_t>;
	ordered_list_mock<async::timer_t> timers;
	queue_mock<async::handler_t> handlers;
	async_mock async{timers, handlers};
	//current time=10
	void (*handler)()=[](){};
	async::timer_t timer {110, handler};
	EXPECT_CALL(async, millis()).WillOnce(Return(10));
	orderd_list_real::iterator (orderd_list_real::*p_insert)(const async::timer_t&) = &orderd_list_real::insert;
	EXPECT_CALL(timers, insert(timer)).WillOnce(
			Invoke(&timers.m_real, p_insert));
	async.async_wait(100, handler);

	//current time=109
	EXPECT_CALL(async, millis()).WillOnce(Return(109));
	async.consume_timer();

	//current time=110
	EXPECT_CALL(async, millis()).WillOnce(Return(110));
	EXPECT_CALL(handlers, push(handler));
	EXPECT_CALL(timers, erase(_)).WillOnce(Invoke([&](orderd_list_real::iterator iter)->orderd_list_real::iterator{
		EXPECT_EQ(*iter, timer);
		return iter;
	}));
	async.consume_timer();
}

class async_test: public ::testing::Test {
protected:
	async_test()
		: m_async(m_timers, m_handlers)
		, m_isrthread(&async_test::isr, this)
		{}

	virtual void TearDown() {
		m_execute=false;
		m_isrthread.join();
	}

	async_mock m_async;
	ordered_list<async::timer_t> m_timers;
	queue<async::handler_t> m_handlers;

private:
	void isr() {
		const std::lock_guard<std::mutex> lock(m_lock);
		while(m_execute){
			usleep(period);
			millis++;
		}
	}
	std::thread m_isrthread;
	std::mutex m_lock;
	bool m_execute=true;
	unsigned long millis=0;
	static const unsigned int period=1;
};


#endif /* TEST_ASYNC_TEST_H_ */
