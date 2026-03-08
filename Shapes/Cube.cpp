//
// Created by shann on 3/1/26.
//

#include "Cube.h"
#include <print>

#include "../Utilities/globalFunctions.h"
#include <glm/gtc/matrix_transform.hpp> // Required for glm::scale and glm::translate
Cube::~Cube()
{
    std::println("Cube Destructor!");
    m_vao.Delete();
    m_vbo.Delete();
}

Cube::Cube() : BaseShape3D()
{
    m_vao.Bind();
    m_vao.LinkAttrib(m_vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *) 0);
    m_vao.Unbind();
    m_vbo.Unbind();
}
Cube::Cube(uint16_t width, uint16_t height, uint16_t depth) : BaseShape3D(width, height, depth)
{
    m_vao.Bind();
    m_vao.LinkAttrib(m_vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *) 0);
    m_vao.Unbind();
    m_vbo.Unbind();
}

void Cube::Draw()
{
    m_vao.Bind();
    glDrawArrays(GL_TRIANGLES,0,36);
}
