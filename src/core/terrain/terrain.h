/*
 * Written by Lukas Rennhofer @2025
 * Engine inspired by Dominique Roduit
 * Spe* @2025 - MIT Licese
 */

#pragma once
#include "icg_helper.h"
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class Terrain {

private:
    GLuint vertex_array_id_;                // vertex array object
    GLuint vertex_buffer_object_position_;  // memory buffer for positions
    GLuint vertex_buffer_object_index_;     // memory buffer for indices
    GLuint program_id_;                     // GLSL shader program ID
    GLuint texture_id_, texture_color_id_, texture_snow_color_id_;                     // texture ID
    GLuint texture_grass_id_, texture_sand_id_, texture_rock_id_, texture_snow_id_;
    GLuint num_indices_;                    // number of vertices to render

    GLuint projection_id_, view_id_, model_id_;
    GLuint light_pos_id_, light_matrix_id_, shadows_tex_id_;

    vec3 light_pos_ = vec3(0.0f);
    mat4 light_matrix_ = mat4(1.0f);
    vec4 clip_plane_ = vec4(0.0f);

    int grid_tesselation_;
    float grid_area_;

    void LoadTexture(string filename, const char* tex_name, GLuint* texture_id, int texture_n, bool mipmap = true, GLuint wrap_mode = GL_REPEAT) {
        int width;
        int height;
        int nb_component;
        // set stb_image to have the same coordinates as OpenGL
        stbi_set_flip_vertically_on_load(1);
        unsigned char* image = stbi_load(filename.c_str(), &width,
                                         &height, &nb_component, 0);

        if(image == nullptr) {
            throw(string("Failed to load texture"));
        }

        glGenTextures(1, texture_id);
        glBindTexture(GL_TEXTURE_2D, *texture_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);

        if (nb_component == 1) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0,
                         GL_RGB, GL_UNSIGNED_BYTE, image);
        } else if(nb_component == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                         GL_RGB, GL_UNSIGNED_BYTE, image);
        } else if(nb_component == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, image);
        }

        GLuint tex_id = glGetUniformLocation(program_id_, tex_name);
        glUniform1i(tex_id, texture_n);

        if (mipmap)
            glGenerateMipmap(GL_TEXTURE_2D);

        // cleanup
        glBindTexture(GL_TEXTURE_2D, texture_n);
        stbi_image_free(image);
    }

public:
    float diffuse_ = 0.175f, specular_ = 0.8f, alpha_ = 60.0f;
    float hsnow_ = 0.8f, fsnow_ = 2.2f;
    float fheight_ = 0.1f, fslope_ = 1.2f, fcolor_ = 0.8333f;
    float light_bias_min_ = 5, light_bias_max_ = 20;
    bool wireframe_mode_ = false;
    vec3 cam_pos_, fog_color_;
    float fog_start_ = 80.0f, fog_end_ = 100.0f, fog_density_ = 0.004f, fog_power_ = 6.0f;
    int fog_type_ = 1;
    vec2 hoffset_ = vec2(0.0f);
    vec3 diffuse_color_;

    void Init(GLuint heightmap_texture_id, GLuint shadows_texture_id, int grid_tesselation, float grid_area) {
        grid_tesselation_ = grid_tesselation;
        grid_area_ = grid_area;

        // compile the shaders.
        program_id_ = icg_helper::LoadShaders("terrain_vshader.glsl",
                                              "terrain_fshader.glsl");
        if(!program_id_) {
            exit(EXIT_FAILURE);
        }

        glUseProgram(program_id_);

        glGenVertexArrays(1, &vertex_array_id_);
        glBindVertexArray(vertex_array_id_);

        {
            int grid_tesselation_ = this->grid_tesselation_;
            int grid_area_ = this->grid_area_;

            std::vector<GLfloat> vertices;
            std::vector<GLuint> indices;

            float delta = (float)grid_area_ / (float)grid_tesselation_;
            for (int y = 0; y <= grid_tesselation_; ++y) {
                for (int x = 0; x <= grid_tesselation_; ++x) {
                    vertices.push_back(-grid_area_ / 2.0f + delta*x);
                    vertices.push_back(-grid_area_ / 2.0f + delta*y);
                }
            }

            auto to_index = [grid_tesselation_](int x, int y) {
                return x + y * (grid_tesselation_ + 1);
            };

            for (int y = 0; y < grid_tesselation_; ++y) {
                for (int x = 0; x < grid_tesselation_; ++x) {
                    if (y % 2 == 0) { 
                        if (x == 0) { 
                            indices.push_back(to_index(x, y));
                            indices.push_back(to_index(x, y+1));
                        }
                        indices.push_back(to_index(x+1, y));
                        indices.push_back(to_index(x+1, y+1));

                    } else { // backward
                        int x_ = grid_tesselation_ - x - 1;
                        if (x == 0) { 
                            indices.push_back(to_index(x_+1, y));
                            indices.push_back(to_index(x_+1, y+1));
                        }
                        indices.push_back(to_index(x_, y));
                        indices.push_back(to_index(x_, y+1));
                    }
                }
            }

            num_indices_ = indices.size();

            // position buffer
            glGenBuffers(1, &vertex_buffer_object_position_);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_position_);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                         &vertices[0], GL_STATIC_DRAW);

            // vertex indices
            glGenBuffers(1, &vertex_buffer_object_index_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_object_index_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                         &indices[0], GL_STATIC_DRAW);

            // position shader attribute
            GLuint loc_position = glGetAttribLocation(program_id_, "position");
            glEnableVertexAttribArray(loc_position);
            glVertexAttribPointer(loc_position, 2, GL_FLOAT, DONT_NORMALIZE,
                                  ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }

        this->texture_id_ = heightmap_texture_id;
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        GLuint tex_id = glGetUniformLocation(program_id_, "heightmap");
        glUniform1i(tex_id, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        this->shadows_tex_id_ = shadows_texture_id;
        glBindTexture(GL_TEXTURE_2D, shadows_texture_id);
        GLuint shadows_uid = glGetUniformLocation(program_id_, "shadows");
        glUniform1i(shadows_uid, 7);
        glBindTexture(GL_TEXTURE_2D, 7);

        LoadTexture("terrain_texture.tga", "tex_color", &texture_color_id_, 1, false, GL_CLAMP_TO_EDGE);
        LoadTexture("terrain_snow_texture.tga", "tex_snow_color", &texture_snow_color_id_, 2, false, GL_CLAMP_TO_EDGE);
        LoadTexture("Grass1.png",	"grass_tex",	&texture_grass_id_, 3);
        LoadTexture("Rock9.png",	"sand_tex",		&texture_sand_id_,	4);
        LoadTexture("Rock5.png",	"rock_tex",		&texture_rock_id_,	5);
        LoadTexture("water.png",    "snow_tex",		&texture_snow_id_,	6);

        projection_id_ = glGetUniformLocation(program_id_, "projection");
        view_id_	   = glGetUniformLocation(program_id_, "view");
        model_id_	   = glGetUniformLocation(program_id_, "model");
        light_pos_id_  = glGetUniformLocation(program_id_, "light_pos");
        light_matrix_id_ = glGetUniformLocation(program_id_, "light_matrix");

        glBindVertexArray(0);
        glUseProgram(0);
    }

    void Cleanup() {
        glBindVertexArray(0);
        glUseProgram(0);
        glDeleteBuffers(1, &vertex_buffer_object_position_);
        glDeleteBuffers(1, &vertex_buffer_object_index_);
        glDeleteVertexArrays(1, &vertex_array_id_);
        glDeleteProgram(program_id_);
        glDeleteTextures(1, &texture_color_id_);

    }

    void Draw(const glm::mat4 &model = IDENTITY_MATRIX,
              const glm::mat4 &view = IDENTITY_MATRIX,
              const glm::mat4 &projection = IDENTITY_MATRIX) {
        glUseProgram(program_id_);
        glBindVertexArray(vertex_array_id_);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id_);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_color_id_);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture_snow_color_id_);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, texture_grass_id_);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, texture_sand_id_);

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, texture_rock_id_);

        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, texture_snow_id_);

        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, shadows_tex_id_);


        glUniformMatrix4fv(projection_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(projection));
        glUniformMatrix4fv(view_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(view));
        glUniformMatrix4fv(model_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(model));
        glUniform3fv(light_pos_id_, ONE, glm::value_ptr(light_pos_));
        glUniformMatrix4fv(light_matrix_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(light_matrix_));

        glUniform1f(glGetUniformLocation(program_id_, "tesselation"), this->grid_tesselation_);
        glUniform1f(glGetUniformLocation(program_id_, "area"), this->grid_area_);

        glUniform1f(glGetUniformLocation(program_id_, "diffuse"), this->diffuse_);
        glUniform1f(glGetUniformLocation(program_id_, "specular"), this->specular_);
        glUniform1f(glGetUniformLocation(program_id_, "alpha"), this->alpha_);
        glUniform1f(glGetUniformLocation(program_id_, "fheight"), this->fheight_);
        glUniform1f(glGetUniformLocation(program_id_, "hsnow"), this->hsnow_);
        glUniform1f(glGetUniformLocation(program_id_, "fsnow"), this->fsnow_);
        glUniform1f(glGetUniformLocation(program_id_, "fslope"), this->fslope_);
        glUniform1f(glGetUniformLocation(program_id_, "fcolor"), this->fcolor_);

        glUniform3fv(glGetUniformLocation(program_id_, "cam_pos"), ONE, glm::value_ptr(cam_pos_));
        glUniform3fv(glGetUniformLocation(program_id_, "fog_color"), ONE, glm::value_ptr(fog_color_));
        glUniform1f(glGetUniformLocation(program_id_, "fog_start"), this->fog_start_);
        glUniform1f(glGetUniformLocation(program_id_, "fog_end"), this->fog_end_);
        glUniform1i(glGetUniformLocation(program_id_, "fog_type"), this->fog_type_);
        glUniform1f(glGetUniformLocation(program_id_, "fog_density"), this->fog_density_);
        glUniform1f(glGetUniformLocation(program_id_, "fog_power"), this->fog_power_);

        glUniform2fv(glGetUniformLocation(program_id_, "hoffset"), ONE, glm::value_ptr(hoffset_));
        glUniform4fv(glGetUniformLocation(program_id_, "clip_plane"), ONE, glm::value_ptr(clip_plane_));

        glUniform3fv(glGetUniformLocation(program_id_, "diffuse_color"), ONE, glm::value_ptr(diffuse_color_));

        glUniform1f(glGetUniformLocation(program_id_, "light_bias_min"), this->light_bias_min_ * 1e-3);
        glUniform1f(glGetUniformLocation(program_id_, "light_bias_max"), this->light_bias_max_ * 1e-3);

        // draw
        if (wireframe_mode_)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glDrawElements(GL_TRIANGLE_STRIP, num_indices_, GL_UNSIGNED_INT, 0);

        if (wireframe_mode_)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glBindVertexArray(0);
        glUseProgram(0);
    }

    void SetLighting(vec3 light_pos, mat4 light_matrix) {
        this->light_pos_ = light_pos;
        this->light_matrix_ = light_matrix;
    }

    void SetClipPlane(vec4 clip_plane) {
        this->clip_plane_ = clip_plane;
    }
};
