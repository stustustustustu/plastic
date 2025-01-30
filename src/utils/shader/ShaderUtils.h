#ifndef SHADERUTILS_H
#define SHADERUTILS_H

#include "../src/config.h"

class ShaderUtils {
    public:
        GLuint ID;

        ShaderUtils(const char* vertexPath, const char* fragmentPath);
        void Use() const;

        void SetBool(const std::string &name, bool value) const;
        void SetInt(const std::string &name, int value) const;
        void SetFloat(const std::string &name, float value) const;
        void SetMat4(const std::string &name, const glm::mat4 &matrix) const;
        void SetVec3(const std::string &name, const glm::vec3 &value) const;
        void SetVec4(const std::string &name, const glm::vec4 &value) const;
    };

#endif // SHADERUTILS_H
