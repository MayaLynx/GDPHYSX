cmake_minimum_required(VERSION 3.10.0)
project(GDPHYSX VERSION 0.1.0 LANGUAGES C CXX)
cmake_policy(SET CMP0072 NEW)

include(FetchContent)

find_package(OpenGL REQUIRED)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        658f48e72aee3c6582e80b05ac0f8787a64fe6bb #v2.0.6
    GIT_PROGRESS   TRUE
    SOURCE_SUBDIR  cmake
)
FetchContent_MakeAvailable(glad)
glad_add_library(glad_gl_core_43 STATIC REPRODUCIBLE LOADER API gl:core=4.3)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

add_executable(GDPHYSX WIN32
    main.cpp
    main.h
    glad.c
    stb_image.h
    tiny_obj_loader.h
    model3D/model3D.cpp
    model3D/model3D.h
    Physics/PhysicsParticle.cpp
    Physics/PhysicsParticle.h
    Physics/PhysicsWorld.cpp
    Physics/PhysicsWorld.h
    Physics/ForceGenerator/ForceGenerator.cpp
    Physics/ForceGenerator/ForceGenerator.h
    Physics/ForceGenerator/ForceRegistry.cpp
    Physics/ForceGenerator/ForceRegistry.h
    Physics/ForceGenerator/GravityForceGenerator.cpp
    Physics/ForceGenerator/GravityForceGenerator.h
    Physics/ForceGenerator/DragForceGenerator.cpp
    Physics/ForceGenerator/DragForceGenerator.h
    Physics/Springs/AnchoredSpring.cpp
    Physics/Springs/AnchoredSpring.h
    Physics/ParticleContact.cpp
    Physics/ParticleContact.h
    Physics/ContactResolver.h
    Physics/ContactResolver.cpp
    Physics/ParticleLink.h
    Physics/ParticleLink.cpp
    Physics/Rods/Rod.h
    Physics/Rods/Rod.cpp)


target_include_directories(GDPHYSX PRIVATE Dependencies/include)

target_link_libraries(GDPHYSX glad_gl_core_43 glm::glm glfw OpenGL::GL)

enable_testing()

