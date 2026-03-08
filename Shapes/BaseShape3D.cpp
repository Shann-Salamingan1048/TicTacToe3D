//
// Created by shann on 3/5/26.
//

#include "BaseShape3D.h"


BaseShape3D::BaseShape3D(uint16_t width, uint16_t height, uint16_t depth)
    : size{.height = height, .width = width, .depth = depth}
{

}

BaseShape3D::BaseShape3D()
    : size{.height = 800, .width = 800, .depth = 800}
{

}
