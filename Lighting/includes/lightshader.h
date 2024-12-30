#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> //// 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

struct PointLight{
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
struct DirLight {
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};


class Shader{
public:

    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    //使用程序
    void use();
    
    //uniform工具函数
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value)const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;

    void setMat4(const std::string &name, const glm::mat4 &value) const;

    void setPointLights(const std::string &name, const std::vector<PointLight> &pointlights ) const;

    void setDirLights(const std::string &name, const DirLight &dirlight  ) const;

};




#endif