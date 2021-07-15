#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "render/Shader.h"
#include "render/Camera.h"
#include "render/Model.h"
#include "render/light/DirLight.h"
#include "render/light/SpotLight.h"
#include "render/light/Attenuation.h"

int width = 800;
int height = 600;

float last_xpos = (float)width / 2.0f;
float last_ypos = (float)height / 2.0f;

float fov = 45.0f;

float last_frame = 0.0f;
float delta_time = 0.0f;

glm::vec3 cameraInitPos(0.0f, 0.0f, 3.0f);

render::Camera camera({
    render::Camera::Type::fly,
    cameraInitPos,
    glm::vec3(0.0f, 0.0f, -1.0f),
    -75.0f,
    0.0f,
    delta_time
});

bool right_mouse_btn_pressed = false;

void framebuffer_size_callback(GLFWwindow* window, int new_width, int new_height) {
    width = new_width;
    height = new_height;
    glViewport(0, 0, width, height);
}

void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!right_mouse_btn_pressed) {
        last_xpos = xpos;
        last_ypos = ypos;
        
        return;
    }
        
    float x_offset = last_xpos - xpos;
    float y_offset = ypos - last_ypos;
    
    last_xpos = xpos;
    last_ypos = ypos;
    
    const float sensetive = 0.1f * fov / 45.0f;
    
    x_offset *= sensetive;
    y_offset *= sensetive;
    
    camera.AddYaw(x_offset);
    camera.AddPitch(y_offset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        right_mouse_btn_pressed = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        right_mouse_btn_pressed = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void handle_key(GLFWwindow* window, int key, void (*callback)()) {
    static double last_call_time = glfwGetTime();
    
    double time = glfwGetTime();
    double diff = time - last_call_time;
    
    if (glfwGetKey(window, key) == GLFW_PRESS && diff > 0.2) {
        callback();
        last_call_time = time;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= yoffset;
    
    if (fov < 1.0f) fov = 1.0f;
    else if (fov > 45.0f) fov = 45.0f;
}

void process_camera_move(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.MoveBackwardRight();
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.MoveBackwardLeft();
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.MoveForwardRight();
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.MoveForwardLeft();
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.MoveForward();
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.MoveBackward();
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.MoveLeft();
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.MoveRight();
    }
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    process_camera_move(window);
}

int main(int argc, const char * argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(window);
    
    if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))) {
        std::cout << "Failed to initilize glad" << std::endl;
        glfwTerminate();
        return -2;
    }
    
    glfwGetFramebufferSize(window, &width, &height);
    
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    render::Shader cubeProgram("./shaders/model-vs.glsl", "./shaders/model-fs.glsl");
    render::Model backpackModel("./assets/backpack/backpack.obj");
    render::light::DirLight dirLight(
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.7f, 0.7f, 0.7f),
        glm::vec3(0.9f, 0.9f, 0.9f)
    );
    std::vector<render::light::SpotLight> spotLights = {
        render::light::SpotLight(
            glm::vec3(0.0342912f, 0.12814f, -1.87929f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.6f, 0.6f, 0.6f),
            5.5f,
            8.0f,
            render::light::Attenuation::seven
        )
    };
    
    while (!glfwWindowShouldClose(window)) {
        process_input(window);
        
        float time = glfwGetTime();
        
        delta_time = time - last_frame;
        last_frame = time;
        
        camera.SetDeltaTime(delta_time);

        glClearColor(0.14f, 0.14f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
        
        cubeProgram.use();
        
        cubeProgram.setValue("view", camera.GetView());
        cubeProgram.setValue("projection", projection);
        cubeProgram.setValue("model", model);
        
        camera.Set(cubeProgram);
        dirLight.Set(cubeProgram);
        
        for (int i = 0; i < spotLights.size(); i++) {
            spotLights[i].Set(cubeProgram, i);
        }
        
        backpackModel.Draw(cubeProgram);
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
    return 0;
}
