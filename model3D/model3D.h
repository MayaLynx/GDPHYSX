#pragma once
#pragma once
#include <glad/glad.h>
#include "../tiny_obj_loader.h"
#include "../stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <vector>

class model3D
{
private:
	glm::vec3 position;
	glm::vec3 scale;
	float axis_x, axis_y, axis_z;
	float theta;
	int img_width, img_height, colorChannels;
	std::string meshPath;
	std::string texturePath;
	glm::vec3 color;
	GLuint VAO, VBO, EBO;
	GLuint shader;
	std::vector<GLuint> mesh_indices;
	std::vector<GLfloat> fullVertexData;
	GLuint texture;
	unsigned char* tex_bytes;
	glm::mat4 transformation_matrix;

public:
	model3D(std::string meshPath, glm::vec3 position, GLuint shader);
	model3D(std::string meshPath, std::string texturePath, glm::vec3 position, GLuint shader, bool hasAlpha);
	void setColor(glm::vec3 newColor);
	void updatePosition(glm::vec3 newPos);
	void translate();
	void setScale(glm::vec3 newScale);
	void Scale();
	void rotate();
	void transform();
	void draw();
};