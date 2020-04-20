/*
 * queue.h
 *
 *  Created on: Apr 18, 2020
 *      Author: user
 */

#ifndef QUEUE_H_
#define QUEUE_H_

namespace utils{
template<typename T>
class queue{
	struct Node{
		T m_data;
		Node *m_next;
	};

	Node* m_back=nullptr;
	Node* m_front=nullptr;
	int m_size=0;

public:
	void push(const T& t){
		auto node = new Node{t, nullptr};
		if (!m_back)
			m_back = m_front = node;
		else{
			m_back->m_next = node;
			m_back = node;
		}
		m_size++;
	}

	void pop(){
		if(m_front){
			auto p_temp=m_front;
			m_front = m_front->m_next;
			delete p_temp;
			m_size--;
		}
		if(!m_size)
			m_back=nullptr;
	}

	bool empty() const{ return !m_back;}
	T& front(){ return m_front->m_data;}
	int size()const { return m_size;}
};
}

#endif /* QUEUE_H_ */
