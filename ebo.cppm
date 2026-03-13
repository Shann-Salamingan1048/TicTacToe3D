module;
#include <glad/glad.h>

export module EBO;
export class EBO
{
public:
	GLuint ID;
	EBO(GLuint* indices, GLsizeiptr size);
	EBO(const GLuint* indices, GLsizeiptr size);
	void Bind();
	void Unbind();
	void Delete();
};