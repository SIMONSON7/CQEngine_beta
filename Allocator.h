/*
	Allocator:
		类似于 apr_pool 简化版，并非完整意义上的 gc ;
		小而且不很复杂，基本上可以满足需求；

	缺点：
		* 没有内存压缩
		* 和 java 等语言的 gc 一样几乎无法实现立即释放某个对象；
		* ...

	qiu_hao			2016/4/17
*/

/*

chainHeader ----->	+————————+ 0		prev
					|		 |------>	+————————+ 0 	prev
					|________| 4		|		 |-----> 
		m_begin --->|		 |			|________| 4
					|		 |			|		 |
					|		 |			|		 |
				  	|  free	 |			|		 |
					|		 |			|		 |
					|		 |			|		 |
					|________|			|		 |
		  m_end --->|		 |			|		 |
					|		 |			|  used  |			....
					|		 |			|		 |
					|		 |			|		 |
					|  used	 |			|		 |
					|		 |			|		 |
					|		 |			|		 |
					|		 |			|		 |
					|		 |			|		 |
					|		 |			|		 |
					+————————+ 2048		|		 |
										+————————+ cb/2048

*/

#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#ifdef __ALLOCATOR_H__
#include <stdio.h>
#include <stdlib.h>
#endif

#define MEM_BIT	11
#define MEM_BLOCKSIZE (1<<MEM_BIT)

#ifndef CQ_MALLOC
#define CQ_MALLOC malloc
#endif
#ifndef CQ_FREE
#define CQ_FREE   free
#endif 

typedef void(*DestructorType)(void* pThis);

namespace CQ
{
	class MemManager
	{
	public:
		enum { MemBlockSize = MEM_BLOCKSIZE };
		enum { HeaderSize = sizeof(void*) };
		enum { BlockSize = MemBlockSize - HeaderSize };

	private:
		// declaraction :
		struct _MemBlock
		{
			_MemBlock* pPrev;
			char buffer[BlockSize];
		};
		struct _DestroyNode
		{
			_DestroyNode* pPrev;
			DestructorType fnDestroy;
		};

		char* m_begin;
		char* m_end;
		_DestroyNode* m_destroyChain;

	private:
		_MemBlock*  _ChainHeader() const
		{
			return (_MemBlock*)(m_begin - HeaderSize);
		}

	public:
		MemManager() : m_destroyChain(NULL)
		{
			m_begin = m_end = (char*)HeaderSize;	// 4
		}

		// copy ctor
		MemManager(const MemManager& rhs);

		// copy assignment ctot
		const MemManager& operator=(const MemManager& rhs);

	public:
		~MemManager()
		{
			clear();
		}

	//api:
	public:
		void  clear()
		{
			// execute dstor
			while (m_destroyChain)
			{
				m_destroyChain->fnDestroy(m_destroyChain + 1);
				m_destroyChain = m_destroyChain->pPrev;
			}

			// free memory
			_MemBlock* pHeader = _ChainHeader();
			while (pHeader)
			{
				_MemBlock* pTemp = pHeader->pPrev;
				CQ_FREE(pHeader);
				pHeader = pTemp;
			}
			m_begin = m_end = (char*)HeaderSize;	// 4 
		}

		// allocate from bottom to top , int front of insert block
		void*  allocate(size_t cb)
		{
			if ((size_t)(m_end - m_begin) < cb)
			{
				if (cb >= BlockSize)
				{
					_MemBlock* pHeader = _ChainHeader();
					_MemBlock* pNew = (_MemBlock*)CQ_MALLOC(HeaderSize + cb);
					if (pHeader)
					{
						pNew->pPrev = pHeader->pPrev;
						pHeader->pPrev = pNew;
					}
					else
					{
						m_end = m_begin = pNew->buffer;
						pNew->pPrev = NULL;
					}
					return pNew->buffer; //
				}	
				else // (m_end - m_begin) < cb < BlockSize
				{
					_MemBlock* pNew = (_MemBlock*)CQ_MALLOC(sizeof(_MemBlock));
					pNew->pPrev = _ChainHeader();
					m_begin = pNew->buffer;
					m_end = m_begin + BlockSize;
				}
			}
			return m_end -= cb;
		}

		void*  allocate(size_t cb, DestructorType fn)
		{
			_DestroyNode* pNode = (_DestroyNode*)allocate(sizeof(_DestroyNode) + cb);
			pNode->fnDestroy = fn;
			pNode->pPrev = m_destroyChain;
			m_destroyChain = pNode;
			return pNode + 1; // cb
		}
	};
}	/* namespace CQ */

#endif /* __ALLOCATOR_H__ */