/*
*
*		qiu_hao		2016/5/3		9:47.pm
*/
#ifndef __NODE_H__
#define __NODE_H__

#include "Stage.h"
#include "Enitity.h"
#include "2D.h"
#include "Array.h"
#include "Shader.h"
#include "Texture2D.h"
#include "CQMath.h"
using namespace std;

class Node:public IEntity
{
private:
	Node() 
		:m_parent(nullptr), m_childArray(nullptr),
		m_shader(nullptr), m_texture(nullptr),
		m_scale(1.0f),
		m_posAttri(0), m_uvAttri(0), m_colorAttri(0), m_mvpUni(0), m_textureUni(0)
	{
		
	}
	~Node()
	{
		if (m_shader)
		{
			m_shader->UnuseShader();
			delete m_shader;
		}
		if (m_texture)
		{
			m_texture->UnuseTexture();
			delete m_texture;
		}
	}

public:
	static Node*
	Create()
	{
		STAGE_CREATE(Node);
	}

	static void
	Destory(void* t)
	{
		STAGE_DESTORY(Node);
	}

	void Init(const CQ::float3 _centerPos,const char* _textureFile)
	{
		Init(_centerPos, _textureFile, 1.0f,nullptr,nullptr);
	}

	void Init(const CQ::float3 _centerPos, const char* _textureFile, const CQ::float4 _color)
	{
		Init(_centerPos, _textureFile, _color, nullptr, nullptr);
	}

	void Init(const CQ::float3 _centerPos, const char* _textureFile, const CQ::float4 _color,const char* _vertexPath, const char* _fragPath)
	{
		// shader
		if (_vertexPath && _fragPath)
		{
			m_shader = new Shader(_vertexPath, _fragPath);
		}

		// color 
		SetColor(_color);

		// texture
		m_texture = new Texture2D();
		SetTexture(_textureFile);

		// pos
		SetPos(_centerPos);

		// uv
		SetUV();
		
		//
		size_t	shaderProgram = 0;
		if (m_shader)
		{
			shaderProgram = m_shader->getShaderProgram();
		}
		else
		{
			Stage* stage = Stage::AccessStage();
			shaderProgram = stage->shader->getShaderProgram();
		}
		// ------------- 偷个懒，以后从 Shader 文件中再重新提取出接口-----------
		m_posAttri	 = glGetAttribLocation(shaderProgram, "vPosition");
		m_uvAttri	 = glGetAttribLocation(shaderProgram, "vUV");
		m_colorAttri = glGetAttribLocation(shaderProgram, "vColor");
		m_mvpUni	 = glGetUniformLocation(shaderProgram, "MVP");
		m_textureUni = glGetUniformLocation(shaderProgram, "texture");

		glEnableVertexAttribArray(m_posAttri);
		glEnableVertexAttribArray(m_uvAttri);
		glEnableVertexAttribArray(m_colorAttri);

		glUniform1i(m_textureUni, 0);
	}

	void SetPos(const CQ::float3 _centerPos)
	{
		m_pos = _centerPos;

		size_t half_width = (m_texture->m_width*m_scale) /2;
		size_t half_height =(m_texture->m_height*m_scale) /2;

		// lb
		CQ::float3 pos;
		pos.x = m_pos.x - half_width;
		pos.y = m_pos.y - half_height;
		pos.z = _centerPos.z;
		m_quad.v1.pos = pos;

		// rb
		pos.x = m_pos.x + half_width;
		pos.y = m_pos.y - half_height;
		pos.z = _centerPos.z;
		m_quad.v2.pos = pos;

		// rt
		pos.x = m_pos.x + half_width;
		pos.y = m_pos.y + half_height;
		pos.z = _centerPos.z;
		m_quad.v3.pos = pos;

		// lt
		pos.x = m_pos.x - half_width;
		pos.y = m_pos.y + half_height;
		pos.z = _centerPos.z;
		m_quad.v4.pos = pos;
	}

	bool SetTexture(const char* _textureFile)
	{
		return m_texture->Init(_textureFile);
	}

	void SetColor(CQ::float4 _color)
	{
		m_quad.v1.color = _color;
		m_quad.v2.color = _color;
		m_quad.v3.color = _color;
		m_quad.v4.color = _color;
	}

	void SetUV()
	{
		// v1
		m_quad.v1.u = 0.0f;
		m_quad.v1.v = 0.0f;

		// v2
		m_quad.v2.u = 1.0f;
		m_quad.v2.v = 0.0f;

		// v3
		m_quad.v3.u = 1.0f;
		m_quad.v3.v = 1.0f;

		// v4
		m_quad.v4.u = 0.0f;
		m_quad.v4.v = 1.0f;
	}

	void SetScale(float _scale)
	{
		m_scale = _scale;
		SetPos(m_pos);
	}

	void Draw()
	{
		if (m_shader)
		{
			m_shader->UseShader();
		}
		if (m_texture)
		{
			m_texture->UseTexture();
		}

		Stage* stage = Stage::AccessStage();
		CQ::matrix4 worldMatrix(1);
		__render(stage->defaultCamera->GetMVP(worldMatrix, stage->win.m_winWidth, stage->win.m_winHeight));

	}

	void Show()
	{
		if (!m_childArray)
		{
			m_childArray = Array<Node*>::Create();
		}
		if (!m_childArray->IsEmpty())
		{
			int i = 0;
			int len = m_childArray->GetLastIndex();
			Node* child;
			while (i < len)
			{
				child = (*m_childArray)[i];
				child->Show();
				i++;
			}
		}
		this->Draw();
	}

	void SetParent()
	{

	}

	void AddChild(Node* _nodeChild)
	{
		if (!m_childArray)
		{
			return;
		}
		if (_nodeChild)
		{
			m_childArray->Push(_nodeChild);
		}

	}
private:
	void __render(const CQ::matrix4 _mvp)
	{
		// mvp
		glUniformMatrix4fv(m_mvpUni, 1, false, _mvp.data());
		
		// texture
		m_texture->UseTexture();

		// render
		unsigned short vertexIndexs[] =
		{
			0, 1, 2, 0, 2, 3
		};

		glVertexAttribPointer(m_posAttri, 3, GL_FLOAT, false, sizeof(Vertex2D), &m_quad.v1.pos.x);
		glVertexAttribPointer(m_uvAttri, 2, GL_FLOAT, false, sizeof(Vertex2D), &m_quad.v1.u);
		glVertexAttribPointer(m_colorAttri, 4, GL_FLOAT, false, sizeof(Vertex2D), &m_quad.v1.color.x);
	
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, vertexIndexs);
	}



public:
	//
	Node*			m_parent;
	Array<Node*>*	m_childArray;
	// 
	Shader*			m_shader;
	Texture2D*		m_texture; // Texture2D* may be better than Texture
	// Camera 
	// bool IsBillboard

	//
	Quad			m_quad;
	float			m_scale;
	CQ::float3		m_pos;

	//
	size_t			m_posAttri;
	size_t			m_uvAttri;
	size_t			m_colorAttri;
	size_t			m_mvpUni;
	size_t			m_textureUni;
};

#endif	/* __NODE_H__ */



