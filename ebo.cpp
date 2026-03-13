module;
#include <glad/glad.h>

module EBO;
EBO::EBO(GLuint* indices, GLsizeiptr size)
{
	// 1. Create AND initialize the buffer state immediately
	glCreateBuffers(1, &ID);

	// 2. Upload the data directly to the initialized ID
	glNamedBufferData(ID, size, indices, GL_STATIC_DRAW);
}

EBO::EBO(const GLuint* indices, GLsizeiptr size)
{
	glCreateBuffers(1, &ID);
	glNamedBufferData(ID, size, indices, GL_STATIC_DRAW);
}

void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}