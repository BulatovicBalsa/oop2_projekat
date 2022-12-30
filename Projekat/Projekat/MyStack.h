#pragma once
#include <list>
using namespace std;


template <typename T> class MyStack {
private:
	list<T> head;
public:
	MyStack<T>() : head() {};
	void push(T elem) {
		head.push_front(elem);
	}
	T pop() {
		T nesto = *(head.begin());
		head.pop_front();
		return nesto;
	}
	T top() {
		return *(head.begin());
	}
	inline bool isEmpty() { return head.empty(); }
};
