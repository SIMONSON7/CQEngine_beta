/*
*	读取，编译，使用 shader 文件
*
*	未来改进：
*		文件错误类型检查，不要用数字；
*		添加 shader 文件名后缀检查；
*		读取文本/二进制文件 等专门做一个工具类/函数；
*	2016/4/9	 qiu_hao
*
*/
#include "Shader.h"

//
int  Shader::_loadShader(const char* filePath, char** shader, unsigned int* fileSize)
{
	FILE* fp = NULL;
	if (!(fp = fopen(filePath, "rb")))
		return -1;
	while (!feof(fp))
		if (fgetc(fp) == '\n') ++(*fileSize);
	++(*fileSize);                                                                    
	fclose(fp);

	// 加载 shader 文件
	if (!(fp = fopen(filePath, "rb")))
		return -1;
	*shader = (char*)malloc(sizeof(char)*MAX_SOURCE_SIZE);

	char c_tmp = -1;
	unsigned int index = 0;
	for (int i = 0; i < MAX_SOURCE_SIZE - 1; ++i)
	{
		c_tmp = fgetc(fp);
		if (c_tmp == EOF) break;
		(*shader)[index++] = c_tmp;
	}
	(*shader)[index] = '\0';

	// 关闭 shader 文件
	fclose(fp);
	return 0;
}

//
void Shader::_unLoadShader()
{
	//vs
	if (!m_vShaderFile) return;
	free(m_vShaderFile);
	m_vShaderFile = NULL;
	//fs
	if (!m_fShaderFile) return;
	free(m_fShaderFile);
	m_fShaderFile = NULL;

}

//
Shader::Shader(const char* vsFilePath, const char* fsFilePath)
	:m_vShaderFile(NULL), m_fShaderFile(NULL), m_vFileSize(0), m_fFileSize(0)
{
	// 加载 vs
	if (_loadShader(vsFilePath, &m_vShaderFile, &m_vFileSize) != 0) return;
	const char* tmp = m_vShaderFile;

	// 编译
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &tmp, NULL);
	glCompileShader(vertexShader);

	GLint success ;
	GLchar infoLog[1024];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		//输出错误信息；
		glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
		return;
	}

	//加载
	if (_loadShader(fsFilePath, &m_fShaderFile, &m_fFileSize) != 0) return;
	tmp = m_fShaderFile;

	//fs obj
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &tmp, NULL);
	glCompileShader(fragmentShader);

	// 检查 fs 编译结果：
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//输出错误信息；
		glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
		return;
	}

	//生成 program objec 并且进行链接
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);

	glLinkProgram(m_shaderProgram);
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		//输出错误信息；
		glGetProgramInfoLog(m_shaderProgram, 1024, NULL, infoLog);
		return;
	}
	//删除 vs,fs
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

//
Shader::~Shader()
{
	_unLoadShader();

	m_shaderProgram = 0;//删除 shaderProgram obj
}


//
void Shader::UseShader()
{
	glUseProgram(this->m_shaderProgram);
}

//
void Shader::UnuseShader()
{
	glUseProgram(0);
}
