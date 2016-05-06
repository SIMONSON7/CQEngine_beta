/*
*	Camera basic
*   
*
*	2016/3/29	 qiu_hao
*
*/
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "CQMath.h"
#include <gles2/gl2.h>

using namespace CQ;

class Camera
{
public:
	Camera()
	{
		m_mView.identify();
		m_mProj.identify();
	}

	/* ! Not recommended */
	Camera(float3 _eyePos, float3 _targetPos, float3 _upPos,float3 _rightPos)
	:	m_fEye(_eyePos),
		m_fTarget(_targetPos),
		m_fUp(_upPos),
		m_fRight(_rightPos)
	{}

public:
	/*
	*	- provide a convient (proj) camera in default
	*
	*/
	matrix4 GetViewProj(const float _winWidth,const float _winHeight)	
	{
		SetViewSize(_winWidth, _winHeight);

		//view matirx4
		this->CalDir();
		this->CalView();

		//proj matrix4
		SetPerspectiveProj(45.0f, (GLfloat)_winWidth / (GLfloat)_winHeight, 0.1f, 100000.0f);//default ,zNear = 0.1f, zFar = 100.0f 

		return m_mProj * m_mView;
	}

public:/* Get&Set */
	inline void  SetViewSize(const float2& _viewSize)
	{
		this->m_viewSize = _viewSize;
	}
	inline void  SetViewSize(float _x, float _y)
	{
		this->m_viewSize = float2(_x, _y);
	}
	inline void SetEye(const float3 _eyePos)
	{
		this->m_fEye = _eyePos;
	}
	inline void SetUp(const float3 _upPos)
	{
		this->m_fUp = _upPos;
	}
	inline void SetRight(const float3 _rightPos)
	{
		this->m_fRight = _rightPos;
	}
	inline void SetTarget(const float3 _targetPos)
	{
		this->m_fTarget = _targetPos;
	}
	void CalDir()
	{
		this->m_fDir = normalize(m_fTarget - m_fEye);
	}
	inline  void  SetProject(const matrix4& _proj)
	{
		this->m_mProj = _proj;
	}
	inline void CalView()
	{
		this->m_mView = lookAt(m_fEye, m_fTarget, m_fUp);
	}
	void SetOrhtoProj(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar)
	{
		this->m_mProj = ortho<float>(_left, _right, _bottom, _top, _zNear, _zFar);
	}
	void SetPerspectiveProj(float _fovy, float _aspect, float _zNear, float _zFar)
	{
		this->m_mProj = perspective<float>(_fovy, _aspect, _zNear, _zFar);
	}

public:
	inline float2 GetViewSize() const
	{
		return this->m_viewSize;
	}
	inline float3 GetEye() const
	{
		return this->m_fEye;
	}
	inline float3 GetUp() const
	{
		return this->m_fUp;
	}
	inline float3 GetRight() const
	{
		return this->m_fRight;
	}
	inline float3 GetTarget() const
	{
		return this->m_fTarget;
	}
	inline const matrix4& GetProject() const
	{
		return  this->m_mProj;
	}
	inline const matrix4&  GetView() const
	{
		return  this->m_mView;
	}
	float3 GetDir()
	{
		return this->m_fDir;
	}

protected:
	float2	m_viewSize;	// view size

	float3	m_fEye;		// camera pos
	float3	m_fUp;		// up
	float3	m_fRight;	// right
	float3	m_fTarget;	// target
	float3	m_fDir;		// dir

	matrix4	m_mView;		// view tranform
	matrix4	m_mProj;		// Proj tranform
};

#endif/* __CAMERA_H__ */