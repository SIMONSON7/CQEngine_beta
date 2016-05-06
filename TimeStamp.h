/*
*		�߾��ȼ�ʱ����
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
		QueryPerformanceFrequency(&m_frequency);	//��û����ڲ���ʱ����ʱ��Ƶ��
		QueryPerformanceCounter(&m_start);			//��ȡ��ǰ����
	}

	//ÿ��ѭ����ʼ���ã�����
	void   record()
	{
		QueryPerformanceCounter(&m_start);	//��������¼ ����ʼ������
	}

	//��ȡʧȥ��ʱ�䣨�룩
	double   getElapsedSecond() const
	{
		LARGE_INTEGER   cur;
		QueryPerformanceCounter(&cur);		//�ٴμ�������¼ ������������
		return  double(cur.QuadPart - m_start.QuadPart) / double(m_frequency.QuadPart);
	}

	//��ȡʧȥ��ʱ�䣨���룩
	double	getElapsedMillisecond() const
	{
		return getElapsedSecond()*1000.f;
	}

protected:
	LARGE_INTEGER   m_start;
	LARGE_INTEGER   m_frequency;

};

#endif/* __TIME_STAMP_H__ */