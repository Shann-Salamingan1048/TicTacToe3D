module;

#include <glad/glad.h>
export module VBO;
export class VBO
{
public:
	GLuint ID;
	VBO(GLfloat* vertices, GLsizeiptr size);
	VBO(const GLfloat* vertices, GLsizeiptr size);
	VBO() = default;
	void Bind();
	void Unbind();
	void Delete();

};