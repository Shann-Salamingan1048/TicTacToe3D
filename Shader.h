#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <print>
#include <unordered_map>
class Shader
{
public:

	explicit Shader(const char* vertPath, const char* fragPath);
	void use() const noexcept;
	[[nodiscard]]GLuint getID() const noexcept { return m_ID;}; // [[nodiscard]] "Hey, the return value of this function is important. If the caller ignores it, generate a warning."
	// COMPILER WARNING: "Return value ignored: 'getHealth' declared with attribute 'nodiscard'" p.getHealth(); CORRECT:int h = p.getHealth();
	void deleteShader() const noexcept;

public:
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

public:
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;

	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;

	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;

	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
	mutable std::unordered_map<std::string, GLint> m_uniformCache;
	GLint getUniformLocation(const std::string& name) const noexcept;
private:
	GLuint m_ID;

private:
	static std::string readShaderFile(const char* filePath);
	static GLuint createShaderProgram(GLuint vertShader, GLuint fragShader);
	static GLuint compileShader(const char* source, GLenum type, const char* typeName);
};