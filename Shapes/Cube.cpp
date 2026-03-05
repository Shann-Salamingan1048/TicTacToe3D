//
// Created by shann on 3/1/26.
//

#include "Cube.h"
#include <print>
Cube::~Cube()
{
    std::println("Cube Destructor!");
    m_vao.Delete();
    m_vbo.Delete();

}

Cube::Cube()
{
    m_vao.Bind();
    m_vao.LinkAttrib(m_vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    m_vao.Unbind();
    m_vbo.Unbind();

}

void Cube::AddCube(glm::vec3 spawnCube)
{
    Cube::CubePositions.emplace_back(spawnCube);
}

void Cube::Draw()
{
    m_vao.Bind();
    glDrawArrays(GL_TRIANGLES,0,36);
}
