#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <QtGlobal>

template<class T>
class RingBuffer
{
public:
	explicit RingBuffer(int bufferSize = 0x8000)
		: m_begin(0), m_end(0), m_head(0), m_tail(0)
	{
		resize(bufferSize);
	}

	~RingBuffer()
	{
		delete [] m_begin;
	}

	void resize(int size)
	{
		if (size <= bufferSize())
			return;

		T *ptr = new T[size];
		int n = read(ptr, this->size());

		if (m_begin)
			delete [] m_begin;

		m_begin = ptr;
		m_end = m_begin + size;
		m_head = m_begin;
		m_tail = m_head + n;
	}

	int read(T *data, int n, const T *start = 0) const
	{
		if (n <= 0 || !data)
			return 0;

		const T *ptr = start ? start : m_head;
		Q_ASSERT_X(ptr >= m_begin && ptr < m_end, "RingBuffer<T>::read", "start is out of range");

		n = qMin(n, count(ptr));

		int r = 0;
		while(r < n)
		{
			int i = qMin<int>(n - r, m_end - ptr);
			qMemCopy(data, ptr, i * sizeof(T));
			data += i;
			r += i;
			ptr = forward(ptr, i);
		}

		return r;
	}

	int read(T *data, int n, int start) const { return read(data, n, pointer(start)); }

	void push(const T *data, int n)
	{
		bool correctHead = size() + n >= bufferSize();
		while(n > 0)
		{
			int i = qMin<int>(n, m_end - m_tail);
			qMemCopy(m_tail, data, i * sizeof(T));
			data += i;
			n -= i;
			m_tail = forward(m_tail, i);
		}
		if (correctHead)
			m_head = forward(m_tail, 1);
	}

	void push(const T &data) { push(&data, 1); }

	void pop(int n = 1)
	{
		if (n >= size)
			m_head = m_tail;
		else
			m_head = forward(m_head, n);
	}

	void clear()
	{
		m_head = m_tail = m_begin;
	}

	T *pointer(int i) const
	{
		Q_ASSERT_X(i >= -size() && i < size(), "RingBuffer<T>::pointer", "out of range");
		return forward(i >= 0 ? m_head : m_tail, i);
	}
	T &operator[](int i) { return *pointer(i); }
	T operator[](int i) const { return *pointer(i); }

	int size() const { return count(m_head, m_tail); }
	int bufferSize() const { return m_end - m_begin; }
	T *data() const { return m_begin; }
	T *begin() const { return m_begin; }
	T *end() const { return m_end; }
	T *head() const { return m_head; }
	T *tail() const { return m_tail; }

	// helper function
	int count(const T *head, const T *tail) const
		{ return tail >= head ? tail - head : m_end - head + tail - m_begin; }
	int count(const T *head) const { return count(head, m_tail); }
	T *forward(const T * ptr, int n = 1) const
	{
		Q_ASSERT_X(ptr >= m_begin && ptr < m_end, "RingBuffer<T>::forward", "out of range");
		ptr += n;
		while(ptr >= m_end)
			ptr -= m_end - m_begin;
		while(ptr < m_begin)
			ptr += m_end - m_begin;
		return const_cast<T*>(ptr);
	}

private:
	T *m_begin;
	T *m_end;
	T *m_head;
	T *m_tail;
};

#endif // RINGBUFFER_H
