#include "lightshader.h"

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath)  {

    //1. 从文件路径读取顶点/片段着色器 

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 保证ifstream可以抛出异常
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        //读取文件缓冲到数据流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        //关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();

        //转换数据到string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure& e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n"  <<e.what()<< std::endl;
        std::cout << "文件："  << vertexPath << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    //2. 编译着色器

    unsigned int vertex, fragment;
    int success;
    char infolog[512];


    //顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex,1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
    };

    //片段着色器
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL,infolog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
    }

    //着色器程序
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << std::endl;
    
    }

    //删除着色器
    glDeleteShader(vertex);
    glDeleteShader(fragment);
};


void Shader::use(){
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}


void Shader::setMat4(const std::string &name, const glm::mat4 &value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setPointLights(const std::string &name, const std::vector<PointLight> &pointlights ) const{
    for (int i = 0; i < pointlights.size(); ++i) {
        std::string index = std::to_string(i);

        glUniform3fv(glGetUniformLocation(ID, (name + "[" + index + "].position").c_str()), 1, glm::value_ptr(pointlights[i].position));
        glUniform3fv(glGetUniformLocation(ID, (name + "[" + index + "].ambient").c_str()), 1, glm::value_ptr(pointlights[i].ambient));
        glUniform3fv(glGetUniformLocation(ID, (name + "[" + index + "].diffuse").c_str()), 1, glm::value_ptr(pointlights[i].diffuse));
        glUniform3fv(glGetUniformLocation(ID, (name + "[" + index + "].specular").c_str()), 1, glm::value_ptr(pointlights[i].specular));

        glUniform1f(glGetUniformLocation(ID, (name + "[" + index + "].constant").c_str()), pointlights[i].constant);
        glUniform1f(glGetUniformLocation(ID, (name + "[" + index + "].linear").c_str()), pointlights[i].linear);
        glUniform1f(glGetUniformLocation(ID, (name + "[" + index + "].quadratic").c_str()), pointlights[i].quadratic);        
    }


    
}

void Shader::setDirLights(const std::string &name, const DirLight &dirlight ) const{


glUniform3fv(glGetUniformLocation(ID, (name + ".direction").c_str()), 1, glm::value_ptr(dirlight.direction));
glUniform3fv(glGetUniformLocation(ID, (name + ".ambient").c_str()), 1, glm::value_ptr(dirlight.ambient));
glUniform3fv(glGetUniformLocation(ID, (name + ".diffuse").c_str()), 1, glm::value_ptr(dirlight.diffuse));
glUniform3fv(glGetUniformLocation(ID, (name + ".specular").c_str()), 1, glm::value_ptr(dirlight.specular));
    
}