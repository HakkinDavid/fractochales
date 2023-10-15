#include <iostream>
#include "Lightning/Lightning.h"
#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

void framebuffer_size_callback (GLFWwindow* window, int width, int height);
void processInput (GLFWwindow *window);
void display ();

int main()
{
    Lightning rasho;
    rasho.randomize();
    rasho.traverse(0, 0);
    rasho.show();

    
    /*
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Rayos Fractales", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "No se pudo iniciar la ventana" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "No se inició GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    while(!glfwWindowShouldClose(window))
    {
        // entrada
        processInput(window);
        
        // renderizado
        // ...
        display();

        // checar eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    */
    return 0;
}

void framebuffer_size_callback (GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput (GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void display () {
    glClearColor(0.1f, 0.0f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}