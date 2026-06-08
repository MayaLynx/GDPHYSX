#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "main.h"
#include "model3D/model3D.h"
#include "Physics/PhysicsParticle.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/ForceGenerator/ForceGenerator.h"
#include "Physics/ForceGenerator/GravityForceGenerator.h"
#include "Physics/ForceGenerator/DragForceGenerator.h"
#include "Physics/Springs/AnchoredSpring.h"
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <chrono>

using namespace std::chrono_literals;
using namespace Koyu;

int main(void)
{
    // Time in between frames
    constexpr std::chrono::nanoseconds timestep(16ms);

    GLFWwindow* window;

    // Create new physics world object
    PhysicsWorld pWorld = PhysicsWorld();

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // Window size variables
    float windowWidth = 800;
    float windowHeight = 800;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowWidth, windowHeight, "GDPHYSX Scratch", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(
        0,
        0,
        windowWidth,
        windowHeight
    );

    // Shader initialization
    std::fstream vertSrc("Shaders/shader.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/shader.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);
    glLinkProgram(shaderProg);

    // Orthographic view matrix
    glm::mat4 projection = glm::ortho(
        -400.0f,
        400.0f,
        -400.0f,
        400.0f,
        -400.0f,
        400.0f);

    // Create objects
    /*model3D* sphere = new model3D("3D/sphere.obj", glm::vec3(0.f, 0.f, 0.f), shaderProg);
    sphere->setScale(30.f, 30.f, 30.f);
    sphere->setColor(glm::vec3(0.4f, 0.f, 0.4f));*/

    glm::vec3 velocity = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 force = glm::vec3(6000.f, 0.f, 0.f);    

    PhysicsParticle* p1 = new PhysicsParticle(shaderProg);
    p1->setColor(glm::vec3(0.4f, 0.f, 0.4f));
    p1->position = glm::vec3(0.f, 200.f, 0.f);
    p1->mass = 50.f;
    p1->addForce(force);
    pWorld.addParticle(p1);

    /*PhysicsParticle* p2 = new PhysicsParticle(shaderProg);
    p2->setColor(glm::vec3(0.4f, 0.f, 0.f));
    p2->position = glm::vec3(-350.f, 200.f, 0.f);
    p2->velocity = velocity;
    p2->acceleration = glm::vec3(0.f, 0.f, 0.f);
    p2->damping = 0.6f;
    p2->addForce(force);
    pWorld.addParticle(p2);

    PhysicsParticle* p3 = new PhysicsParticle(shaderProg);
    p3->setColor(glm::vec3(0.f, 0.4f, 0.f));
    p3->position = glm::vec3(-350.f, 200.f, 0.f);
    p3->velocity = velocity;
    p3->acceleration = glm::vec3(0.f, 0.f, 0.f);
    p3->damping = 0.9f;
    p3->addForce(force);
    pWorld.addParticle(p3);*/

    glm::vec3 springPos = glm::vec3(0.f, 200.f, 0.f);
    AnchoredSpring aSpring = AnchoredSpring(springPos, 5.f, 0.5f);
    pWorld.forceRegistry.add(p1, &aSpring);

    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        curr_time = clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
        prev_time = curr_time; 

        curr_ns += dur;
        if (curr_ns >= timestep)
        {
            constexpr float timestep_sec = timestep.count() / (float)(1E09); // Convert ns -> secs

            curr_ns -= timestep;

            //std::cout << "Physics update" << std::endl;
            pWorld.update(timestep_sec);

        }
        //std::cout << "Normal update" << std::endl;
        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Position of camera in the world
        glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 3.0f);

        glm::mat4 cameraPositionMatrix =
            glm::translate(glm::mat4(1.0f),
                cameraPos * -1.0f);

        glm::vec3 worldUp = glm::vec3(0, 1.0f, 0);
        glm::vec3 center = glm::vec3(0.f, 0.f, 0.f);

        // Forward, Right, Up
        glm::vec3 F = glm::vec3(center - cameraPos);
        F = glm::normalize(F);
        glm::vec3 R = glm::normalize(glm::cross(F, worldUp));
        glm::vec3 U = glm::normalize(glm::cross(R, F));

        glm::mat4 cameraRotMatrix = glm::mat4(1.0f);

        // Camera matrix
        cameraRotMatrix[0][0] = R.x;
        cameraRotMatrix[1][0] = R.y;
        cameraRotMatrix[2][0] = R.z;

        cameraRotMatrix[0][1] = U.x;
        cameraRotMatrix[1][1] = U.y;
        cameraRotMatrix[2][1] = U.z;

        cameraRotMatrix[0][2] = -F.x;
        cameraRotMatrix[1][2] = -F.y;
        cameraRotMatrix[2][2] = -F.z;

        // View matrix
        glm::mat4 viewMatrix = cameraRotMatrix * cameraPositionMatrix;

        // Projection matrix assignment
        unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projLoc,
            1,
            GL_FALSE,
            glm::value_ptr(projection));

        // View matrix assignment
        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc,
            1,
           GL_FALSE,
            glm::value_ptr(viewMatrix));

        // Draw object
        p1->draw();
        //p2->draw();
        //p3->draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}