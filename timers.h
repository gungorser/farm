#if defined(BUILD_TYPE) && BUILD_TYPE  == RELEASE
#include <Arduino.h>
#include <Time.h>
#endif

#include "queue.h"

#ifndef TIMERS_H_
#define TIMERS_H_

#define TIMER_FREQUENCY 5;
using handler_t = void (*)();

struct Time{
	uint8_t hour;
	uint8_t minute;
};

struct Timer {
	unsigned long m_time;
	handler_t mp_handler;
};

Node<Timer> *timers = nullptr;
queue<handler_t> handlers;

void print_timers(){
	Serial.println("print_timers");
	auto p_timer = timers;
	while (p_timer){
		Serial.println(p_timer->m_data.m_time);
		p_timer = p_timer->m_next;
	}
}

void async_wait(unsigned long ms, handler_t callback) {
	auto p_timer = new Node<Timer> { { millis()+ms, callback }, nullptr };
	auto p_curr = timers;
	if (!p_curr) {
		timers = p_timer;
		return;
	}

	Node<Timer> *p_prev = nullptr;
	while (p_curr && p_curr->m_data.m_time < p_timer->m_data.m_time) {
		p_prev = p_curr;
		p_curr = p_curr->m_next;
	}
	if(p_prev){
		p_prev->m_next = p_timer;
	}else{
		timers=p_timer;
	}
	p_timer->m_next = p_curr;
}

void async_wait_until(Time time, handler_t callback) {
	auto currtime = now() % SECS_PER_DAY;
	Serial.print("currtime");Serial.println(currtime);
	auto until = makeTime({
		0,
		time.minute,
		time.hour,
		day(),
		month(),
		year()
	}) % SECS_PER_DAY;
	auto diff = ((until-currtime+SECS_PER_DAY) % SECS_PER_DAY) * 1000;
	async_wait(diff, callback);
}

void consume_timer(unsigned long currtime) {
	Serial.println(millis());
	while (timers && timers->m_data.m_time < currtime) {
		handlers.push(timers->m_data.mp_handler);
		auto p_next = timers->m_next;
		delete timers;
		timers = p_next;
	}
}

void consume_handler() {
	while (!handlers.empty()) {
		handlers.front()();
		handlers.pop();
	}
}

#if defined(BUILD_TYPE) && BUILD_TYPE  == RELEASE
void initialize_time(Time initial_time){
	// TIMER 1 for interrupt frequency 1000 Hz:
	cli(); // stop interrupts
	TCCR1A = 0; // set entire TCCR1A register to 0
	TCCR1B = 0; // same for TCCR1B
	TCNT1  = 0; // initialize counter value to 0
	// set compare match register for 1000 Hz increments
	OCR1A = 15999; // = 16000000 / (1 * 1000) - 1 (must be <65536)
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS12, CS11 and CS10 bits for 1 prescaler
	TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);
	sei(); // allow interrupts

	setTime(initial_time.hour, initial_time.minute, 0, 1, 1, 2020);
	Serial.print("Sistem Aktif. Saat: "); Serial.println(now()%SECS_PER_DAY);
}

ISR(TIMER1_COMPA_vect)        // interrupt service routine
{
	consume_timer(millis());
}
#endif
#endif /* TIMERS_H_ */
