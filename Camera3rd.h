/*
*	3rd	Camera
*		
*	2016/3/29	 qiu_hao
*
*/
#ifndef __CAMERA3RD_H__
#define __CAMERA3RD_H__
#include "Camera.h"

class Camera3rd : public Camera
{
public:
	Camera3rd()
		:
		Camera()
	{
		m_mWorld.identify();
	}

	Camera3rd(float _radiuss)
		:
		m_fYaw(0), m_fRadius(_radiuss),
		Camera()
	{
		m_mWorld.identify();
	}

	/* ! Not recommended */
	Camera3rd(float _radius ,float3 _eyePos, float3 _targetPos, float3 _upPos, float3 _rightPos)
		:
		m_fRadius(_radius),
		Camera(_eyePos,_targetPos,_upPos,_rightPos)
	{
		m_mWorld.identify();
	}

public:
	void Init(float _radius, float3 _eyePos, float3 _targetPos, float3 _upPos)
	{
		SetRadius(_radius);
		SetEye(_eyePos);
		SetTarget(_targetPos);
		CalDir();
		SetUp(_upPos);
	}

	// convient 
	matrix4 GetMVP(matrix4& _worldMatix,const float _winWidht,const float _winHeight)
	{
		m_mWorld = _worldMatix;
		return m_mWorld*GetViewProj(_winWidht, _winHeight);
	}

public:
	
	inline void SetRadius( const float _radius )
	{
		this->m_fRadius = _radius;
	}
	inline float GetRadius() const
	{
		return this->m_fRadius;
	}
public:
	void Update()
	{
		float3 upDir = normalize(m_fUp);
		m_fEye = m_fTarget - m_fDir*m_fRadius;
		m_fRight = normalize(cross(m_fDir, upDir));
		m_mView = lookAt(m_fEye, m_fTarget, m_fUp);
	}
	/*
	 *	- world's coordinate --> screen's coordinate
	 */
	float2  WordToScreen(const float3& _world)
	{
		float4  worlds(_world.x, _world.y, _world.z, 1);
		float4  screens;
		__project(worlds, screens);
		return  float2(screens.x, screens.y);
	}
	/*
	*	- screen's coordinate --> world's coordinate
	*/
	float3  ScreenToWorld(const float2& _screen)
	{
		float4  screens(_screen.x, _screen.y, 0, 1);
		float4  world;
		__unProject(screens, world);
		return  float3(world.x, world.y, world.z); 
	}

	float3  ScreenToWorld(float x, float y)
	{
		float4  screens(x, y, 0, 1);
		float4  world;
		__unProject(screens, world);
		return  float3(world.x, world.y, world.z);
	}

	// create an ray 
	Ray CreateRayFromScreen(int x, int y)
	{
		float4  minWorld;
		float4  maxWorld;
		float4  screen(float(x), float(y), 0, 1);
		float4  screen1(float(x), float(y), 1, 1);
		__unProject(screen, minWorld);	//z = 0
		__unProject(screen1, maxWorld);	//z = 1

		Ray     ray;
		ray.setOrigin(float3(minWorld.x, minWorld.y, minWorld.z));

		float3  dir(maxWorld.x - minWorld.x, maxWorld.y - minWorld.y, maxWorld.z - minWorld.z);
		ray.setDirection(normalize(dir));
		return  ray;
	}

	void RotateCamera(float _angle)
	{
		m_fDir = rotateY<float>(m_fDir, _angle);
	}

private:
	bool    __project(const float4& world, float4& screen)
	{
		screen = (m_mProj*m_mView*m_mWorld)*world;
		if (screen.w == 0.0f)
		{
			return false;
		}
		screen.x /= screen.w;
		screen.y /= screen.w;
		screen.z /= screen.w;

		// map to range 0 - 1
		screen.x = screen.x * 0.5f + 0.5f;
		screen.y = screen.y * 0.5f + 0.5f;
		screen.z = screen.z * 0.5f + 0.5f;

		// map to viewport
		screen.x = screen.x * m_viewSize.x;
		screen.y = m_viewSize.y - (screen.y * m_viewSize.y);
		return  true;
	}

	//
	bool    __unProject(const float4& screen, float4& world)
	{
		float4 v;
		v.x = screen.x;
		v.y = screen.y;
		v.z = screen.z;
		v.w = 1.0;

		// map from viewport to 0 - 1
		v.x = (v.x) / m_viewSize.x;
		v.y = (m_viewSize.y - v.y) / m_viewSize.y;
		//v.y = (v.y - _viewPort.Y) / _viewPort.Height;

		// map to range -1 to 1
		v.x = v.x * 2.0f - 1.0f;
		v.y = v.y * 2.0f - 1.0f;
		v.z = v.z * 2.0f - 1.0f;

		matrix4  inverse = (m_mProj * m_mView * m_mWorld).inverse();

		v = v * inverse;
		if (v.w == 0.0f)
		{
			return false;
		}
		world = v / v.w;
		return true;
	}

private:
	matrix4	m_mWorld;	// world transform / model transform
	float	m_fRadius;	// distance
	float	m_fYaw;		// angle


	
};

#endif /* __CAMERA3RD_H__ */
