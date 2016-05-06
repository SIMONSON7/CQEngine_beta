/*
*	1st	Camera 
*   
*	2016/3/29	 qiu_hao
*
*/
#ifndef __CAMERA1ST_H__
#define __CAMERA1ST_H__

#include "Camera.h"
#include <tchar.h>

class Camera1st : public Camera
{
public:
	Camera1st() :m_speed(10)
	{}
	Camera1st(float _speed, float3 _eyePos, float3 _targetPos, float3 _upPos, float3 _rightPos) 
		:m_speed(_speed),
		Camera(_eyePos,_targetPos,_upPos,_rightPos)
	{

	}

public:
	inline void SetSpeed(const float _speed)
	{
		m_speed = _speed;
	}
	inline float GetSpeed() const
	{
		return m_speed;
	}

	/*
		TMP
	*/
	void Update( const float _elapsed )
	{
		unsigned char key[256];
		GetKeyboardState(key);

		CalDir();
		float3 dir = GetDir();

		if ( key[VK_UP] & 0x80 )
		{
			m_fEye		-= dir*-m_speed * _elapsed;
			m_fTarget	-= dir*-m_speed*_elapsed;
		}
		if (key[VK_DOWN] & 0x80)
		{
			m_fEye += (dir*-m_speed) * _elapsed;
			m_fTarget += (dir*-m_speed) * _elapsed;
		}

		if (key[VK_LEFT] & 0x80)
		{
			m_fEye -= (m_fRight*m_speed) * _elapsed;
			m_fTarget -= (m_fRight*m_speed) * _elapsed;
		}

		if (key[VK_RIGHT] & 0x80)
		{
			m_fEye += (m_fRight*m_speed) * _elapsed;
			m_fTarget += (m_fRight*m_speed) * _elapsed;
		}
	}

private:
	float m_speed;

};

#endif/* __CAMERA1ST_H__ */