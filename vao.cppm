module;
#include <glad/glad.h>

export module VAO;
import VBO;
import EBO;
export class VAO
{
public:
	GLuint ID;
	VAO();


	void LinkAttrib(VBO& vbo, GLuint layout);
	void LinkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void LinkEBO(EBO& ebo);
	void Bind();
	void Unbind();
	void Delete();
};