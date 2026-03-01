#pragma once
#include <glad/glad.h>
#include "vbo.h"

class VAO
{
public:
	GLuint ID;
	VAO();


	void LinkAttrib(VBO& vbo, GLuint layout);
	void LinkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

	void Bind();
	void Unbind();
	void Delete();
};