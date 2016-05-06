/*
*		dynamic array
*
*		qiu_hao    2016/4/30	6:20 pm.
*/
#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdio.h>
#include <stdlib.h>
#include "Stage.h"
using namespace std;

#ifndef __STAGE_H__
#define Allocate	malloc
#else
#define Allocate(TYPE_SIZE)							\
	s->mm->allocate(TYPE_SIZE,Destory)				
#define Deallocte	free
#define Reallocate	realloc			

#endif


#define DEFAULT_NUM 1<<2

#ifndef roundup32(x) 
#define roundup32(x) (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))
#endif

template<class Type>
class Array
{
public:
	static
	Array* Create()
	{
		Stage* s = Stage::AccessStage(); // tmp			
		void* buffer = Allocate(sizeof(Array));	
		return new(buffer)Array;
	}
	static
	Array* Create(size_t _capacity)
	{
		Stage* s = Stage::AccessStage(); // tmp
		void* buffer = Allocate(sizeof(Array));	
		return new(buffer)Array(_capacity);
	}
	static void
	Destory(void* arr)
	{
		static_cast<Array*>(arr)->~Array();
	}
public:
	void Push(Type _value)
	{
		Push(m_cur, _value);
	}
	void Push(size_t _index, Type _value)
	{
		if (_index >= m_capacity)
		{
			__resize(0);
		}
		m_container[_index++] = _value;
		m_cur = _index;
	}
	Type GetValue()
	{
		GetValue(m_cur - 1);
	}
	Type GetValue(size_t _index)
	{
		if (_index >= m_cur) // 超过存储元素的最大索引
		{
			int tmp = m_cur;
			if (!IsEmpty()) --tmp;	// 不为空，返回最后一个存储元素
			return m_container[tmp];// 为空，返回索引 0 的元素
		}
		return m_container[_index];
	}
	// 如果不是顺序存储的，请不要使用这个函数
	Type Pop()
	{
		if (IsEmpty()) return m_container[m_cur];
		return m_container[--m_cur];
	}
public:
	bool operator==(const Array& _array)
	{
		return _array.m_capacity == m_capacity && __equal(_array);
	}
	bool operator!=(const Array& _array)
	{
		return !(this == _array);
	}
	Array& operator=(const Array<Type>& _array)
	{
		if (_array != this)
		{
			if (m_capacity < _array.m_cur)
			{
				__resize(_array.m_capacity);
			}
			else
			{
				memcpy(m_container, _array.m_container, sizeof(Type)*_array.m_cur);
				m_cur = _array.m_cur;
			}
		}
		return *this;
	}
	Type operator[](size_t _index)
	{
		return GetValue(_index);
	}
	size_t GetLastIndex()
	{
		return this->m_cur;
	}
	size_t GetCapacity()
	{
		return this->m_capacity;
	}
	bool IsEmpty()
	{
		return (m_cur == 0);
	}
private:
	bool __equal(const Array& _array)
	{
		int i = 0;
		while (i++ < m_cur)
		{
			if (m_container[i] != _array[i]) return false;
		}
		return true;
	}
	void __resize(size_t _num)
	{
		if (!_num)	m_capacity = m_capacity << 1;
		else m_capacity = _num;
		m_container = (Type*)realloc(m_container, sizeof(Type)*m_capacity);
	}
private:
	Array() :m_cur(0), m_capacity(DEFAULT_NUM)
	{
		m_container = (Type*)malloc(sizeof(Type) * DEFAULT_NUM);
	}
	Array(size_t _capacity) :m_cur(0), m_capacity(_capacity)
	{
		m_container = (Type*)malloc(sizeof(Type) * _capacity);
	}
	~Array()
	{
		Deallocte(m_container); // just free container, Type objs will be freed by pool
	}
private:
	size_t m_cur;	// 最后一个存储元素的下一个位置
	size_t m_capacity;
	Type*  m_container;
};

#endif /* __ARRAY_H__ */
