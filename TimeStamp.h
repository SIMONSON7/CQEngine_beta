/*
*		高精度计时器：
*
*		2016/3/29	 qiu_hao
*
*/
#ifndef __TIME_STAMP_H__
#define __TIME_STAMP_H__

#include <Windows.h>

class   Timestamp
{
public:
	Timestamp()
	{
		QueryPerformanceFrequency(&m_frequency);	//获得机器内部计时器的时钟频率
		QueryPerformanceCounter(&m_start);			//获取当前计数
	}

	//每次循环开始调用：计数
	void   record()
	{
		QueryPerformanceCounter(&m_start);	//计数：记录 “开始计数”
	}

	//获取失去的时间（秒）
	double   getElapsedSecond() const
	{
		LARGE_INTEGER   cur;
		QueryPerformanceCounter(&cur);		//再次计数：记录 “结束计数”
		return  double(cur.QuadPart - m_start.QuadPart) / double(m_frequency.QuadPart);
	}

	//获取失去的时间（毫秒）
	double	getElapsedMillisecond() const
	{
		return getElapsedSecond()*1000.f;
	}

protected:
	LARGE_INTEGER   m_start;
	LARGE_INTEGER   m_frequency;

};

#endif/* __TIME_STAMP_H__ */