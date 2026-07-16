#include "model3D.h"


model3D::model3D(std::string meshPath, glm::vec3 position, GLuint shader)
    : meshPath{ meshPath }
    , position{ position }
    , shader{ shader }
{
    scale = glm::vec3(1.f, 1.f, 1.f);
    axis_x = axis_y = 0.f;
    axis_z = 1.f;
    theta = 0.f;
    color = glm::vec3(1.f, 1.f, 1.f);

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        model3D::meshPath.c_str()
    );

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * attributes.vertices.size(),
        &attributes.vertices[0],
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    transformation_matrix = glm::mat4(1.0f);

    // std::cout << "Object created" << std::endl;
}

model3D::model3D(std::string meshPath, std::string texturePath, glm::vec3 position, GLuint shader, bool hasAlpha)
    : meshPath {meshPath}
    , texturePath {texturePath}
    , position {position}
    , shader {shader}
{
    scale = glm::vec3(1.f, 1.f, 1.f);
    axis_x = axis_y = 0.f;
    axis_z = 1.f;
    theta = 0.f;
    color = glm::vec3(1.f, 1.f, 1.f);

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes
        , &shapes
        , &material
        , &warning
        , &error
        , meshPath.c_str()
    );
    
    stbi_set_flip_vertically_on_load(true);

    // Texture loading setup
    tex_bytes =
        stbi_load(texturePath.c_str(),
            &img_width,
            &img_height,
            &colorChannels,
            0);

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    if (hasAlpha)
    {
        glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_RGBA,
            img_width,
            img_height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            tex_bytes);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_RGB,
            img_width,
            img_height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            tex_bytes);
    }
    

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        // Push the X, Y, Z of the vertex
        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3)]
        );
        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3) + 1]
        );
        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3) + 2]
        );

        // Push the normals
        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3)]
        );
        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 1]
        );
        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 2]
        );

        // Push Tex Coords
        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2)]
        );
        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2) + 1]
        );
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)0
    );

    GLintptr normPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)normPtr
    );

    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)uvPtr
    );

    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void model3D::setColor(glm::vec3 newColor)
{
    color = newColor;
}

void model3D::updatePosition(glm::vec3 newPos)
{
    position = newPos;
}

void model3D::translate()
{
    transformation_matrix = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(position)

    );

    /*unsigned int transformLoc = glGetUniformLocation(shader, "transform");
    glUniformMatrix4fv(transformLoc,
        1,
        GL_FALSE,
        glm::value_ptr(transformation_matrix));*/
}

void model3D::setScale(glm::vec3 newScale)
{
    scale = newScale;
}

void model3D::Scale()
{
    transformation_matrix = glm::scale(
        transformation_matrix,
        scale
    );
}

void model3D::rotate()
{
    transformation_matrix = glm::rotate(
        transformation_matrix,
        glm::radians(theta),
        glm::normalize(glm::vec3(axis_x, axis_y, axis_z))
    );
}

void model3D::transform()
{
    translate();
    Scale();
    rotate();

    unsigned int transformLoc = glGetUniformLocation(shader, "transform");
    glUniformMatrix4fv(transformLoc,
        1,
        GL_FALSE,
        glm::value_ptr(transformation_matrix));
}

void model3D::draw()
{
    if (!texturePath.empty())
    {
        glActiveTexture(GL_TEXTURE0);
        GLuint tex0Address = glGetUniformLocation(shader, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);
    }

    glUseProgram(shader);
    glBindVertexArray(VAO);

    transform();

    if (!texturePath.empty())
    {
        glDrawArrays(
                GL_TRIANGLES,
                0,
                fullVertexData.size() / 8
        );
    }
    else
    {
        unsigned int colorLoc = glGetUniformLocation(shader, "objectColor");
        glUniform3fv(colorLoc, 1, glm::value_ptr(color));

        glDrawElements(GL_TRIANGLES,
            mesh_indices.size(),
            GL_UNSIGNED_INT,
            0);
    }
    
}