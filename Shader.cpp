/*
*	��ȡ�����룬ʹ�� shader �ļ�
*
*	δ���Ľ���
*		�ļ��������ͼ�飬��Ҫ�����֣�
*		��� shader �ļ�����׺��飻
*		��ȡ�ı�/�������ļ� ��ר����һ��������/������
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

	// ���� shader �ļ�
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

	// �ر� shader �ļ�
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
	// ���� vs
	if (_loadShader(vsFilePath, &m_vShaderFile, &m_vFileSize) != 0) return;
	const char* tmp = m_vShaderFile;

	// ����
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &tmp, NULL);
	glCompileShader(vertexShader);

	GLint success ;
	GLchar infoLog[1024];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		//���������Ϣ��
		glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
		return;
	}

	//����
	if (_loadShader(fsFilePath, &m_fShaderFile, &m_fFileSize) != 0) return;
	tmp = m_fShaderFile;

	//fs obj
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &tmp, NULL);
	glCompileShader(fragmentShader);

	// ��� fs ��������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//���������Ϣ��
		glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
		return;
	}

	//���� program objec ���ҽ�������
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);

	glLinkProgram(m_shaderProgram);
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		//���������Ϣ��
		glGetProgramInfoLog(m_shaderProgram, 1024, NULL, infoLog);
		return;
	}
	//ɾ�� vs,fs
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

//
Shader::~Shader()
{
	_unLoadShader();

	m_shaderProgram = 0;//ɾ�� shaderProgram obj
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
