/*
*	��ȡ�����룬���ӣ�ʹ�� shader �ļ�
*
*	2016/4/9	 qiu_hao
*
*/
#ifndef __SHADER_H__
#define __SHADER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <EGL/egl.h>
#include <gles2/gl2.h>

#define MAX_SOURCE_SIZE 20480 // shader �ļ������ֽ���

class Shader
{
public:
	// ��ȡ�����룬���� shader �ļ�
	Shader(const char* vsFilePath, const char* fsFilePath);

	// ʹ�õ�ǰ shader obj
	void UseShader();

	//
	void UnuseShader();

	~Shader();
private:
	// ��ȡ���� shader �ļ��� char* mShaderFile
	int _loadShader(const char* filePath, char** shader, unsigned int* fileSize);

	// �ͷ� char** mShaderFile 
	void _unLoadShader();

public:
	inline unsigned getShaderProgram()
	{
		return this->m_shaderProgram;
	}

private:
	unsigned int m_shaderProgram;		// ���Ӻ��shader obj

	char* m_vShaderFile;					// VS �ű�
	char* m_fShaderFile;					// FS �ű�
	unsigned int m_vFileSize;			// VS �ű��ֽ���
	unsigned int m_fFileSize;			// FS �ű��ֽ���
};




#endif
