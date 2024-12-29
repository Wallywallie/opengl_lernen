#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> //// 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

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

};




#endif