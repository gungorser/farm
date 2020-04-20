/*
 * async.h
 *
 *  Created on: Apr 19, 2020
 *      Author: user
 */

#ifndef ASYNC_H_
#define ASYNC_H_

#include "ordered_list.h"
#include "queue.h"

namespace utils{
class async{
public:
	using handler_t = void (*)();
	struct timer_t {
		unsigned long m_time;
		handler_t mp_handler;
		operator unsigned long() const { return m_time;	}
		bool operator==(const timer_t& rhs)const{
			return m_time==rhs.m_time && mp_handler == rhs.mp_handler;
		}
	};

	async(ordered_list<timer_t>& timers, queue<handler_t> &handlers)
		: m_timers(timers)
		, m_handlers(handlers)
	{}

	void async_wait(unsigned long ms, handler_t callback) {
		m_timers.insert({millis()+ms, callback});
	}

	void consume_timer() {
		auto currtime=millis();
		ordered_list<timer_t>::iterator it;
		do{
			it=find_if(m_timers.begin(), m_timers.end(), [currtime](const timer_t& timer){
				return timer <= currtime && timer > currtime-windowperiod;
			});
			if(it != m_timers.end()){
				m_handlers.push((*it).mp_handler);
				m_timers.erase(it);
			}
		}while(it != m_timers.end());
	}

	void consume_handler(){
		while(m_handlers.empty()){
			m_handlers.front()();
			m_handlers.pop();
		}
	}
	ordered_list<timer_t>& get_timer(){return m_timers;}
	virtual ~async()=default;

	virtual unsigned long millis() const = 0;

private:
	static const unsigned long int windowperiod=10;
	ordered_list<timer_t> &m_timers;
	queue<handler_t> &m_handlers;
};
}


#endif /* ASYNC_H_ */
