module;
#include <glad/glad.h>
#include <cstdint> // Required for uintptr_t casting
module VAO;
import VBO;
import EBO;
VAO::VAO()
{
	glCreateVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO& vbo, GLuint layout)
{
	// For a basic 3-float position setup, we calculate the stride here
	const GLsizei stride = 3 * sizeof(GLfloat);

	// 1. Enable the attribute index directly on this VAO
	glEnableVertexArrayAttrib(ID, layout);

	// 2. Define the format (3 floats, no normalization, 0 offset)
	glVertexArrayAttribFormat(ID, layout, 3, GL_FLOAT, GL_FALSE, 0);

	// 3. Link the attribute layout to a binding point (we just match the layout number)
	glVertexArrayAttribBinding(ID, layout, layout);

	// 4. Plug the VBO directly into that binding point slot
	glVertexArrayVertexBuffer(ID, layout, vbo.ID, 0, stride);
}

void VAO::LinkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	// 1. Enable the attribute index directly on this VAO
	glEnableVertexArrayAttrib(ID, layout);

	// 2. Convert the legacy void* pointer into a modern relative integer offset
	auto relativeOffset = static_cast<GLuint>(reinterpret_cast<uintptr_t>(offset));

	// 3. Define the format dynamically based on your passed parameters
	glVertexArrayAttribFormat(ID, layout, numComponents, type, GL_FALSE, relativeOffset);

	// 4. Link the attribute layout to the binding point
	glVertexArrayAttribBinding(ID, layout, layout);

	// 5. Plug the VBO directly into the slot
	glVertexArrayVertexBuffer(ID, layout, vbo.ID, 0, stride);
}

void VAO::Bind()
{
	// Binding is still required right before calling glDrawArrays!
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}
// Inside vao.cpp
void VAO::LinkEBO(EBO& ebo)
{
	// Directly plug the EBO into this specific VAO's state!
	glVertexArrayElementBuffer(ID, ebo.ID);
}
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}