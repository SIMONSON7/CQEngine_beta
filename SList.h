/*
*	single list
*
*	qiu_hao			2016/4/20
*/
#include <stdio.h>
#include <stdlib.h>

#ifndef __SLIST_H__
#define __SLIST_H__


#define Allocate	calloc
#define Reallocate	realloc
#define Deallocate	free


namespace CQ
{
	// call back
#define __dstor_callback

	// list node
	template<class Type>
	struct ListNode
	{
		Type* data;
		ListNode* next;
	};
	typedef ListNode<class Type> MmData;

	// memory pool
	struct MemoryPool
	{
		size_t cnt, n, max;
		MmData** buf;
	};

	static
	MemoryPool* __mm_init()
	{
		return (MemoryPool*)Allocate(1, sizeof(MemoryPool));
	}

	static
	MmData* __mm_alloc(MemoryPool* mp)
	{
		if (!mp) return NULL;
		++mp->cnt;
		if (mp->n == 0)	return (MmData*)Allocate(1, sizeof(MmData));
		return mp->buf[--mp->n];
	}

	static
	void __mm_free(MemoryPool* mp, MmData* p)
	{
		if (!mp) return;
		--mp->cnt;
		if (mp->n == mp->max)
		{
			mp->max = mp->max ? mp->max << 1 : 16;
			mp->buf = (MmData**)realloc(mp->buf, sizeof(MmData*)*(mp->max));
		}
		mp->buf[mp->n++] = p;
	}

	static
	void __mm_destory(MemoryPool* mp)
	{
		size_t k;
		for (k = 0; k<mp->n; ++k)
		{
			__dstor_callback(mp->buf[k]);// no use
			Deallocate(mp->buf[k]);
		}
		Deallocate(mp->buf);
		Deallocate(mp); // ! mp is released by Allocator
	}

	/* Slist */
	typedef MmData SlistItertor;
	class Slist
	{
	public:
		Slist()
		{
			m_size = 0;
			mp = __mm_init();
			m_head = m_tail = __mm_alloc(mp);
			m_head->next = nullptr;
		}
		~Slist()
		{
			MmData* tmp;
			for (tmp = this->m_head; tmp != this->m_tail; tmp = tmp->next)
			{
				__mm_free(this->mp, tmp);
			}
			__mm_free(this->mp, tmp);
			__mm_destory(this->mp);
			free(this);
		}
	public:
		bool IsEmpty()
		{
			return (this->m_head->next == nullptr);
		}
		void* GetData(SlistItertor* iter)
		{
			if (!iter) return nullptr;
			return iter->data;
		}
		void Next(SlistItertor* iter)
		{
			if (!iter) return;
			iter = iter->next;
		}
		MmData* GetBegin()
		{
			return this->m_head;
		}
		MmData* GetEnd()
		{
			return this->m_tail;
		}

	public:
		void* Push()
		{
			MmData* q, *p = __mm_alloc(this->mp);
			q = this->m_tail;
			p->next = nullptr;
			this->m_tail->next = p;
			this->m_tail = p;
			++this->m_size;
			return &q->data; // the data of the new node
		}

		int Shirt(void* data) // back the data of the node whitch is going to be deleted
		{
			MmData* p;
			if (IsEmpty())	return -1;

			--this->m_size;
			p = this->m_head;
			this->m_head = this->m_head->next;
			if (data)
			{
				data = p->data;
			}
			__mm_free(this->mp, p);
			return 0;
		}

	private:
		MmData* m_head, *m_tail;
		MemoryPool* mp;
		size_t m_size;

	};

}

#endif







