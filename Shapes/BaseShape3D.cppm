module;
#include <cstdint>
export module BaseShape3D;
export class BaseShape3D
{
public:
    explicit BaseShape3D(uint16_t width, uint16_t height, uint16_t depth);
    explicit BaseShape3D();
    virtual ~BaseShape3D() = 0;
public:
    virtual void Draw() = 0;

public:
    struct Size
    {
        uint16_t height{0};
        uint16_t width{0};
        uint16_t depth{0};
    };
    Size size;

};
inline BaseShape3D::~BaseShape3D() = default;