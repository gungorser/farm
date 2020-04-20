/*
 * multiset.h
 *
 *  Created on: Apr 19, 2020
 *      Author: user
 */

#ifndef ORDERED_LIST_H_
#define ORDERED_LIST_H_

namespace utils
{
template<typename T>
class ordered_list {
	struct Node {
		T m_data;
		Node *mp_prev=nullptr;
		Node *mp_next=nullptr;

	};
	Node *mp_root=nullptr;

public:
	struct iterator{
		Node* mp_node=nullptr;
		iterator(){}
		iterator(Node* node):mp_node(node){}
		T& operator*(){return mp_node->m_data;}
		bool operator==(const iterator& rhs) const { return mp_node==rhs.mp_node;}
		bool operator!=(const iterator& rhs) const { return mp_node!=rhs.mp_node;}
		iterator& operator++(){
			if(mp_node->mp_next) mp_node=mp_node->mp_next;
			else mp_node = nullptr;
			return *this;
		}
	};

	virtual iterator insert(const T &data){
		if(!mp_root) return insert (nullptr, nullptr, data);
		return insert(mp_root->mp_prev, mp_root, data);
	}

	iterator erase(iterator iter){
		auto& p_node=iter.mp_node;
		if(!p_node) return nullptr;
		if(p_node->mp_next) p_node->mp_next->mp_prev=p_node->mp_prev;
		if(p_node->mp_prev) p_node->mp_prev->mp_next=p_node->mp_next;
		else mp_root=p_node->mp_next;
		auto retval = p_node->mp_next;
		delete p_node;
		return retval;
	}

	iterator begin(){return iterator{mp_root};}
	iterator end(){return iterator{nullptr};}
	virtual ~ordered_list()=default;

private:
	iterator insert(Node* p_prev, Node* p_curr, const T &data) {
		if(!p_curr && !p_prev){
			mp_root = new Node {data};			// empty
			return mp_root;
		}
		if (!p_curr){
			auto p_data = new Node {data, p_prev};
			p_prev->mp_next=p_data;
			return p_data;
		}
		if(data <= p_curr->m_data){
			auto p_data = new Node {data, p_prev, p_curr};
			p_curr->mp_prev=p_data;
			if(p_prev) p_prev->mp_next=p_data;
			else mp_root=p_data;
			return p_data;
		}
		return insert(p_curr, p_curr->mp_next, data);
	}
};

template<class InputIterator, class UnaryPredicate>
InputIterator find_if(InputIterator begin, InputIterator end, UnaryPredicate cond){
	if(begin==end) return nullptr;
	if(cond(*begin)) return begin;
	return find_if(++begin, end, cond);
}
}

#endif /* ORDERED_LIST_H_ */
