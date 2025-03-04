/*
 * Written by Lukas Rennhofer @2025
 * Engine inspired by Dominique Roduit
 * Spe* @2025 - MIT Licese
 */

#pragma once
#include "icg_helper.h"

class Framebuffer {

    private:
        int width_;
        int height_;
        GLuint framebuffer_object_id_;
        GLuint depth_render_buffer_id_;
        GLuint color_texture_id_;

        GLenum buffer_id_;

        //static GLenum buffer_count_ = 0;

    public:
        // warning: overrides viewport!!
        void Bind() {
            glViewport(0, 0, width_, height_);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object_id_);
            const GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
            glDrawBuffers(1 /*length of buffers[]*/, buffers);
            glReadBuffer(GL_COLOR_ATTACHMENT0);
        }

        void Unbind() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        int Init(int image_width, int image_height, GLint internalFormat = GL_RGB, GLenum format = GL_RGB, GLenum type = GL_UNSIGNED_BYTE, bool use_interpolation = false) {
            this->width_ = image_width;
            this->height_ = image_height;

            {
                glGenTextures(1, &color_texture_id_);
                glBindTexture(GL_TEXTURE_2D, color_texture_id_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                if(use_interpolation){
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                } else {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                }

                // khronos.org/opengles/sdk/docs/man3/docbook4/xhtml/glTexImage2D.xml
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width_, height_, 0,
                             format, GL_UNSIGNED_BYTE, NULL);
            }

            {
                glGenRenderbuffers(1, &depth_render_buffer_id_);
                glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buffer_id_);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width_, height_);
                glBindRenderbuffer(GL_RENDERBUFFER, 0);
            }

            {
                glGenFramebuffers(1, &framebuffer_object_id_);
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object_id_);
                glFramebufferTexture2D(GL_FRAMEBUFFER,
                                       GL_COLOR_ATTACHMENT0 /*location = 0*/,
                                       GL_TEXTURE_2D, color_texture_id_,
                                       0 /*level*/);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                          GL_RENDERBUFFER, depth_render_buffer_id_);

                if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
                    GL_FRAMEBUFFER_COMPLETE) {
                    cerr << "Framebuffer is at crisis :(" << endl;
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }

            return color_texture_id_;
        }

        void Cleanup() {
            glDeleteTextures(1, &color_texture_id_);
            glDeleteRenderbuffers(1, &depth_render_buffer_id_);
            glBindFramebuffer(GL_FRAMEBUFFER, 0 /*UNBIND*/);
            glDeleteFramebuffers(1, &framebuffer_object_id_);
        }
};
