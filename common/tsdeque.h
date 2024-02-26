#pragma once

#include <mutex>
#include <deque>

namespace Network{

	//Thread safe Deque
	template <typename T>
	class TsDeque {
	public:
		TsDeque() = default;
		TsDeque(const TsDeque<T>&) = delete;
		virtual ~TsDeque() = default;

		const T& front() {
			std::scoped_lock lock(m_dequeMutex);
			return m_deque.front();
		}

		const T& back() {
			std::scoped_lock lock(m_dequeMutex);
			return m_deque.back();
		}

		void push_back(const T& item) {
			std::scoped_lock lock(m_dequeMutex);
			m_deque.push_back(item);

		}

		void push_front(const T& item) {
			std::scoped_lock lock(m_dequeMutex);
			m_deque.push_front(item);
		}

		bool empty() {
			std::scoped_lock lock(m_dequeMutex);
			return m_deque.empty();
		}

		size_t size() {
			std::scoped_lock lock(m_dequeMutex);
			return m_deque.size();
		}

		void clear() {
			std::scoped_lock lock(m_dequeMutex);
			m_deque.clear();
		}

		T pop_front() {
			std::scoped_lock lock(m_dequeMutex);
			auto item = std::move(m_deque.front());
			m_deque.pop_front();
			return item;
		}

		T pop_back() {
			std::scoped_lock lock(m_dequeMutex);
			auto item = std::move(m_deque.back());
			m_deque.pop_back();
			return item;
		}


	protected:
		std::mutex m_dequeMutex;
		std::deque<T> m_deque;
	};

}