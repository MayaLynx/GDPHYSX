#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "main.h"
#include "Physics/PhysicsParticle.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/PhysicsCube.h"
#include "Physics/ForceGenerator/ForceGenerator.h"
#include "Physics/ForceGenerator/GravityForceGenerator.h"
#include "Physics/ForceGenerator/DragForceGenerator.h"
#include "Physics/Springs/AnchoredSpring.h"
#include "Physics/ParticleContact.h"
#include "Physics/ParticleLink.h"
#include "Physics/Rods/Rod.h"
#include "Physics/Rods/Chain.h"
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
    window = glfwCreateWindow(windowWidth, windowHeight, "Koyu Engine", NULL, NULL);
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

    // Create objects/particles
    PhysicsParticle* p1 = new PhysicsParticle(shaderProg);
    p1->setColor(glm::vec3(0.4f, 0.f, 0.4f));
    p1->setRadius(50.f);
    p1->position = glm::vec3(0.f, 100.f, 0.f);
    p1->mass = 50.f;
    pWorld.addParticle(p1);

    p1->addForceAtPoint(glm::vec3(
            -1, 0, 0
        ) * glm::vec3(100000),
        glm::vec3(0, 0, 0) 
    );

    PhysicsCube* p2 = new PhysicsCube(shaderProg, glm::vec3(60, 30, 30));
    p2->setColor(glm::vec3(0.7f, 0.f, 0.f));
    p2->position = glm::vec3(-100, 100, 0);
    p2->mass = 50.f;
    pWorld.addParticle(p2);

    // Create chain that's linked to p1 particle
    Chain* chain = new Chain();
    chain->particles[0] = p1;
    chain->length = 200;
    chain->anchorPoint = glm::vec3(0.f, 100.f, 0.f);

    pWorld.links.push_back(chain);

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
        pWorld.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}