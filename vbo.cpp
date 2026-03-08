#include "vbo.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	// 1. Create AND initialize the buffer state immediately
	glCreateBuffers(1, &ID);

	// 2. Safely upload the data directly to the initialized ID
	glNamedBufferData(ID, size, vertices, GL_STATIC_DRAW);
}

VBO::VBO(const GLfloat* vertices, GLsizeiptr size)
{
	glCreateBuffers(1, &ID);
	glNamedBufferData(ID, size, vertices, GL_STATIC_DRAW);
}

void VBO::Bind()
{
	// You likely won't need to call this anymore since your VAO handles
	// the VBO linkage directly, but it is safe to keep just in case!
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}