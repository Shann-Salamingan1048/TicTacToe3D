#include <glad/glad.h>


class Texture
{
public:
	GLuint ID;
	GLenum type;
	Texture(const char* image, GLenum texType, GLenum slot, GLenum pixelType, bool verticalEnable);

	void setActiveTexture(GLenum slot);
	void setFilterMipMap(GLint filer1, GLint filter2);
	void setFilterTexture(GLint filer1, GLint filter2);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};
