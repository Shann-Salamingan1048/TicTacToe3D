//
// Created by shann on 3/1/26.
//

#include "Cube.h"


Cube::Cube(Shader* shader)
    :m_shader(shader)
{
    m_vao.Bind();
    m_vao.LinkAttrib(m_vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    m_vao.LinkAttrib(m_vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    m_vao.Unbind();
    m_vbo.Unbind();

}

void Cube::Draw()
{
    m_vao.Bind();
    glDrawArrays(GL_TRIANGLES,0,36);
}
