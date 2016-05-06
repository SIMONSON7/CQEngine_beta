/*
*	读取，编译，链接，使用 shader 文件
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

#define MAX_SOURCE_SIZE 20480 // shader 文件最大的字节数

class Shader
{
public:
	// 读取并编译，链接 shader 文件
	Shader(const char* vsFilePath, const char* fsFilePath);

	// 使用当前 shader obj
	void UseShader();

	//
	void UnuseShader();

	~Shader();
private:
	// 读取加载 shader 文件到 char* mShaderFile
	int _loadShader(const char* filePath, char** shader, unsigned int* fileSize);

	// 释放 char** mShaderFile 
	void _unLoadShader();

public:
	inline unsigned getShaderProgram()
	{
		return this->m_shaderProgram;
	}

private:
	unsigned int m_shaderProgram;		// 链接后的shader obj

	char* m_vShaderFile;					// VS 脚本
	char* m_fShaderFile;					// FS 脚本
	unsigned int m_vFileSize;			// VS 脚本字节数
	unsigned int m_fFileSize;			// FS 脚本字节数
};




#endif
