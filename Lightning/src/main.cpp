#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <shader.h>
#include <camera.h>



void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x_in, double y_in);
void scoll_callback(GLFWwindow* window, double x_offset, double y_offset) ;
void processInput(GLFWwindow *window);


float vertices[] = {
    -0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f, 
    -0.5f,  0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 

    -0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f, 
     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f,  

    -0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  

     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  

    -0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f, -0.5f, 

    -0.5f,  0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f, -0.5f
};

int screenHeight = 600;
int screenWidth = 800;

Camera camera(glm::vec3(0.f,0.f, 3.f));
GLboolean firstMouse = true;
double last_x = screenHeight/2;
double last_y = screenWidth/2;

float delta_time = 0.f;
float last_frame = 0.f;

int main () {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);   

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //GLAD 必须在你设置 OpenGL 上下文之后初始化
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout <<  "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //GLFW监听
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scoll_callback);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    std::filesystem::path vs_path = std::filesystem::current_path().parent_path() / "src/shader.vs";
    std::filesystem::path fs_path = std::filesystem::current_path().parent_path() / "src/shader.fs";
    Shader myShader(vs_path.string().c_str(), fs_path.string().c_str());

    glEnable(GL_DEPTH_TEST);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

         //渲染指令;
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清空屏幕颜色缓冲和深度缓冲


       delta_time = glfwGetTime() - last_frame;
       last_frame = glfwGetTime();

       glm::mat4 model(1.f);
       glm::mat4 view = camera.getViewMatrix();
       glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)screenWidth/screenHeight, 0.1f, 100.f);

        int modelLoc = glGetUniformLocation(myShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(myShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));       
        int projectionLoc = glGetUniformLocation(myShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


        myShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);





        glfwSwapBuffers(window);
        glfwPollEvents();       

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);    
    glfwTerminate();


    return 0;

}
void frame_buffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width, height);
}

void processInput(GLFWwindow *window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(FOWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(BACKWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(LEFT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(RIGHT, delta_time);
    }     
   
}

void scoll_callback(GLFWwindow *window, double x_offset, double y_offset){
    camera.processMouseScoll(static_cast<float>(y_offset));
}
void mouse_callback(GLFWwindow* window, double x_in, double y_in){
    float x_pos = static_cast<float> (x_in);
    float y_pos = static_cast<float> (y_in);
    float x_offset = x_pos - last_x;
    float y_offset = last_y - y_pos;
    last_x = x_pos;
    last_y = y_pos;

    camera.processMouseMovement(x_offset, y_offset, true);




}





