#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <lightshader.h>
#include <camera.h>



void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x_in, double y_in);
void scoll_callback(GLFWwindow* window, double x_offset, double y_offset) ;
void processInput(GLFWwindow *window);


float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 pointLightPositions[] = {
    glm::vec3( 1.5f,  1.f,  1.f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -2.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

PointLight pointLight1 = { 
    pointLightPositions[0],   // position

    glm::vec3(0.05f, 0.05f, 0.05f), 
    glm::vec3(0.8f, 0.8f, 0.8f), 
    glm::vec3(1.0f, 1.0f, 1.0f), 

    1.0f,
    0.09f,
    0.032f
};
PointLight pointLight2 = { 
    pointLightPositions[1],   // position


    glm::vec3(0.05f, 0.05f, 0.05f), 
    glm::vec3(0.8f, 0.8f, 0.8f), 
    glm::vec3(1.0f, 1.0f, 1.0f), 

    1.0f,
    0.09f,
    0.032f
};
PointLight pointLight3 = { 
    pointLightPositions[2],   // position


    glm::vec3(0.05f, 0.05f, 0.05f), 
    glm::vec3(0.8f, 0.8f, 0.8f), 
    glm::vec3(1.0f, 1.0f, 1.0f), 

    1.0f,
    0.09f,
    0.032f
};
PointLight pointLight4 = { 
    pointLightPositions[3],   // position


    glm::vec3(0.05f, 0.05f, 0.05f), 
    glm::vec3(0.8f, 0.8f, 0.8f), 
    glm::vec3(1.0f, 1.0f, 1.0f), 

    1.0f,
    0.09f,
    0.032f
};

DirLight dirlight1 = { 
    glm::vec3(-0.2f, -1.0f, -0.3f),   // position

    glm::vec3(0.05f, 0.05f, 0.05f),   
    glm::vec3(0.8f, 0.8f, 0.8f),   
    glm::vec3(0.2f, 0.2f, 0.2f),   
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);     
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);       

    glBindVertexArray(0);//解绑

    //为灯也创建一个VAO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);



    std::filesystem::path cwd = std::filesystem::current_path();

    std::filesystem::path vs_path = cwd / "src" / "obj_shader.vs";
    std::filesystem::path obj_fs_path = cwd / "src" / "obj_shader.fs";
    std::filesystem::path light_fs_path = cwd /"src" / "light_shader.fs";
    std::filesystem::path light_vs_path = cwd / "src" / "light_shader.vs";

    Shader myShader(vs_path.string().c_str(), obj_fs_path.string().c_str());
    Shader lightShader(light_vs_path.string().c_str(), light_fs_path.string().c_str());

    unsigned int texture_diff;
    glGenTextures(1, &texture_diff);
    glBindTexture(GL_TEXTURE_2D, texture_diff);

    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  

    int width, height, nrchannels;
    std::filesystem::path diff_map_path = cwd / "src" / "container2.png";
    unsigned char* data = stbi_load(diff_map_path.string().c_str(), &width, &height, &nrchannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

    } else {
        std::cout << "Failed to load texture" << std::endl; 
    }
    stbi_image_free(data);

    unsigned int texture_spec;
    glGenTextures(1, &texture_spec);
    glBindTexture(GL_TEXTURE_2D, texture_spec);

    std::filesystem::path spec_map_path = cwd / "src" / "container2_specular.png";
    data = stbi_load(spec_map_path.string().c_str(), &width, &height, &nrchannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl; 
    }




    glEnable(GL_DEPTH_TEST);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    myShader.use();
    myShader.setVec3("lightColor", glm::vec3(1.0f));
    
    

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

         //渲染指令;
        glClearColor(0.f, 0.f, 0.f, 1.0f); //0.2f, 0.3f, 0.3f, 1.0f
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清空屏幕颜色缓冲和深度缓冲


        delta_time = glfwGetTime() - last_frame;
        last_frame = glfwGetTime();

        glm::mat4 model(1.f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)screenWidth/screenHeight, 0.1f, 100.f);
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));
        
        myShader.use();
        myShader.setMat4("model", model);
        myShader.setMat4("view", view);
        myShader.setMat4("projection", projection);
        myShader.setMat4("normalMatrix", normalMatrix);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_diff);  
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_spec);         

        myShader.setInt("material.diffuse", 0);
        myShader.setInt("material.specular", 1);
        myShader.setFloat("material.shininess", 32.f);

 
        myShader.setDirLights("dirlight",dirlight1);   
        myShader.setPointLights("pointLights", std::vector<PointLight>({pointLight1, pointLight2, pointLight3, pointLight4}));  

        myShader.setVec3("viewerPos", camera.position);



       



        myShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36); 

      
        for (int i = 0; i < sizeof(pointLightPositions)/ sizeof(pointLightPositions[0]); i++) {
            model = glm::mat4(1.f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            
            


            lightShader.use();
            lightShader.setMat4("model", model);
            lightShader.setMat4("view", view);
            lightShader.setMat4("projection", projection);    
    
            lightShader.use();
            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
        


       

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





