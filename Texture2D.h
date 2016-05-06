/*
*
*		2016/4/6	 qiu_hao
*
*/

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <stdio.h>
#include "freeImage/FreeImage.h"

class Texture2D
{
public:
	Texture2D() 
		:m_textureId(0), m_dib(nullptr), 
		m_textureParamMagFilter(GL_LINEAR), m_textureParamMinFilter(GL_LINEAR),
		m_width(0), m_height(0)
	{}
	~Texture2D()
	{
		if (m_dib)
		{
			FreeImage_Unload(m_dib);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &m_textureId);
	}
public:
	bool Init(const char* _fileName)
	{
		return __loadTexture(_fileName);
	}

	void UseTexture()
	{
		glBindTexture(GL_TEXTURE_2D, m_textureId);
	}

	void UnuseTexture()
	{
		glBindTexture(GL_TEXTURE_2D,0);
	}

	void SetTextureMagFilter(const int _magFilter)
	{
		m_textureParamMagFilter = _magFilter;
	}

	void SetTextureMinFilter(const int _minFilter)
	{
		m_textureParamMinFilter = _minFilter;
	}

private:
	// .png only now 
	bool __loadTexture(const char* _fileName)
	{
		if (!_fileName) return false;

		// load file bits
		FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(_fileName, 0);
		m_dib = FreeImage_Load(fifmt, _fileName, 0);

		// 
		unsigned    fmt = GL_RGB;
		if (FreeImage_GetColorType(m_dib) == FIC_RGBALPHA)
		{
			fmt = GL_RGBA;
		}
		else
		{
			m_dib = FreeImage_ConvertTo24Bits(m_dib);
		}
		m_width = FreeImage_GetWidth(m_dib);
		m_height = FreeImage_GetHeight(m_dib);
		BYTE    *pixels = (BYTE*)FreeImage_GetBits(m_dib);

		if (fmt == GL_RGBA)
		{
			for (int i = 0; i < m_width * m_height * 4; i += 4)
			{
				// std::swap
				BYTE r = pixels[i];
				pixels[i] = pixels[i + 2];
				pixels[i + 2] = r;
			}
		}
	 
		// Textuer obj
		glGenTextures(1, &m_textureId);;
		glBindTexture(GL_TEXTURE_2D, m_textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_textureParamMagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_textureParamMinFilter);

		// load into the GL
		glTexImage2D(
			GL_TEXTURE_2D,      
			0,                 
			fmt,                
			m_width,              
			m_height,             
			0,                  
			fmt,                
			GL_UNSIGNED_BYTE,   
			pixels
			);

		return true;
	}

private:
	size_t		m_textureId;
	FIBITMAP*	m_dib; // no use
	int			m_textureParamMagFilter;
	int			m_textureParamMinFilter;

public:
	size_t		m_width;
	size_t		m_height;
};


#endif








