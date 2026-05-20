#pragma once
#include <stack>
#include <queue>

template<typename T>
class lifo
{
	private:
		std::stack<T> s;
	
	public:
		lifo()
		{}
		
		// la struttura stack inserisce l'elemento sempre in cima
		void put(const T& x) {
			s.push(x);
		}
		
		// L'elemento ad essere preso è sempre quello in cima allo stack
		T get() {
			T x = s.top();
			s.pop();
			return x;
		}
		
		bool empty() {
			return s.empty();
		}
};

template<typename T>
class fifo
{
	private:
		std::queue<T> q;
	
	public:
		fifo()
		{}
		
		// La struttura queue inserisce l'elemento in coda 
		void put(const T& x) {
			q.push(x);
		}
		// Come lo stack queue prende l'elemento in cima
		T get() {
			T x = q.front();
			q.pop();
			return x;
		}
		
		bool empty() {
			return q.empty();
		}
};
