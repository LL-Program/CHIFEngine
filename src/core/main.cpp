/*
 * Written by Lukas Rennhofer @2025
 * Engine inspired by Dominique Roduit
 * Spe* @2025 - MIT Licese
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unistd.h>

#include "icg_helper.h"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "heightmap/heightmap.h"
#include "terrain/terrain.h"

#include "framebuffer.h"
#include "shadows.h"
#include "bezier.h"
#include "water/water.h"
#include "skybox/skybox.h"
#include "screenquad/screenquad.h"

#include "constants.h"

#define CAMERA_SPEED (0.05)
#define FRAMEBUFFER_RATIO (0.5)

using namespace glm;

Heightmap heightmap;
Terrain terrain;
Water water;
Framebuffer water_reflection;
ScreenQuad sq;
SkyBox skybox;
Shadows shadows;

int window_width = 1600;
int window_height = 1000;
int water_framebuffer_width = (int) window_width*FRAMEBUFFER_RATIO;
int water_framebuffer_height = (int) window_height*FRAMEBUFFER_RATIO;
const vec3 cam_up = vec3(0.0f, 0.0f, 1.0f);
const int grid_tesselation = 512, grid_area = 600;

enum camera_type {
    CAMERA_FREE, CAMERA_FPS, CAMERA_PATH, CAMERA_HOUR
};

mat4 projection_matrix;
mat4 framebuffer_projection_matrix;
mat4 view_matrix;
mat4 light_matrix;

// Camera
vec4 cam_vel;
vec4 cam_acc;
vec2 cam_dir;
vec3 cam_pos;
vec3 light_pos;

GLuint shadows_tex_id = 0;
GLuint heightmap_tex_id = 0;

vector<vec3> camera_controls = generate_piecewise_bezier(vector<vec3>{
    vec3(0.421, -1.211, 6.75),
    vec3(0.684, -1.750, 6.75),
    vec3(1.551, -1.728, 6.75),
    vec3(1.730, -1.202, 2.58),
    }, 2);

vector<vec2> orientation_controls = generate_piecewise_bezier(vector<vec2>{
    vec2(2.377, -1.753),
    vec2(3.400, -1.753),
    vec2(4.622, -1.753),
    vec2(5.937, -1.593),
    }, 2);

vector<vec3> diffuse_controls = generate_piecewise_bezier(vector<vec3>{
    vec3(0.1, 0.1, 0.2),
    vec3(0.1, 0.1, 0.2),
    vec3(0.6, 0.8, 0.3),
    vec3(1.0, 1.0, 1.0),
    vec3(1.0, 1.0, 0.4),
    vec3(1.0, 0.4, 0.2),
    vec3(0.1, 0.1, 0.2),
        }, 0);


void Init(GLFWwindow* window) {
    glClearColor(1.0, 1.0, 1.0 /*white*/, 1.0 /*solid*/);
    glEnable(GL_DEPTH_TEST);


    light_pos = vec3(300.0f, 0.0f, 60.0f);

    cam_pos = vec3(0.0f, 0.0f, 3.0f);
    cam_dir = vec2(12.5f, -1.8f);

    projection_matrix = perspective(45.0f, (float)window_width / (float)window_height, 0.1f, 1000.0f);
    framebuffer_projection_matrix = perspective(45.0f, (float)water_framebuffer_width/ (float)water_framebuffer_height, 0.1f, 1000.0f);

    mat4 light_projection = ortho(-80.0f, 80.0f, -300.0f, 300.0f, 0.1f, 800.0f); 
    mat4 light_view = lookAt(light_pos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    light_matrix = light_projection * light_view;

    shadows_tex_id = shadows.Init(grid_tesselation*4, grid_tesselation*4);

    heightmap_tex_id = heightmap.Init(grid_tesselation, grid_tesselation);
    terrain.Init(heightmap_tex_id, shadows_tex_id, grid_tesselation, grid_area);
    terrain.SetLighting(light_pos, light_matrix);

    GLuint reflection_texture_id = water_reflection.Init(water_framebuffer_width, water_framebuffer_height);

    sq.Init(window_width, window_height, reflection_texture_id);
    water.Init(heightmap_tex_id, reflection_texture_id, grid_tesselation, grid_area, FRAMEBUFFER_RATIO, &heightmap.dx_, &heightmap.dy_);

    skybox.Init();

}

void Update(float dt) {
    static bool first_run = true, lock_height = true, show_lighting = true;
    static float speed = 0.0;
    static float hoffset[2] = { heightmap.dx_, heightmap.dy_ }, sm_edges[2] = { 60.0f, 45.0f };
    static float camera_position[3]  = { 0.0, 0.0, 0.0 };
    static float camera_direction[2] = { 0.0, 0.0 };
    static float fog_color[3] = { 0.73, 0.8, 1.0 };
    static float hour = 18.5, timer = 0.0;
    static enum camera_type cam_type = CAMERA_FREE;

    camera_position[0] = cam_pos[0]; camera_position[1] = cam_pos[1]; camera_position[2] = cam_pos[2];
    camera_direction[0] = cam_dir[0]; camera_direction[1] = cam_dir[1];

    ImGui::SetNextWindowSize(ImVec2(0.0f, 0.0f));
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    if (first_run)
        ImGui::SetNextTreeNodeOpen(true);

    if (ImGui::CollapsingHeader("Camera")) {
        ImGui::DragFloat3("position", camera_position, 0.005);
        ImGui::DragFloat2("direction", camera_direction, 0.005);
        ImGui::SliderFloat("timer", &timer, 0.0, 1.0);
        ImGui::RadioButton("free", (int*)&cam_type, CAMERA_FREE); ImGui::SameLine();
        ImGui::RadioButton("FPS", (int*)&cam_type, CAMERA_FPS); ImGui::SameLine();
        ImGui::RadioButton("bezier path", (int*)&cam_type, CAMERA_PATH); ImGui::SameLine();
        ImGui::RadioButton("time", (int*)&cam_type, CAMERA_HOUR);
        ImGui::Checkbox("lock cam height", &lock_height); ImGui::SameLine();
        ImGui::Checkbox("wireframe", &terrain.wireframe_mode_);

        cam_pos[0] = camera_position[0]; cam_pos[1] = camera_position[1]; cam_pos[2] = camera_position[2];
        cam_dir[0] = camera_direction[0]; cam_dir[1] = camera_direction[1];
    }

    if (first_run)
        ImGui::SetNextTreeNodeOpen(true);

    if (ImGui::CollapsingHeader("Terrain Options")) {
        ImGui::DragFloat2("hoffset", hoffset, 0.005);
        ImGui::DragFloat("voffset", &heightmap.voffset_, 0.005);

        ImGui::DragFloat("hcomp", &heightmap.hcomp_, 0.005);
        ImGui::DragFloat("vcomp", &heightmap.vcomp_, 0.005);

        ImGui::DragInt("seed", &heightmap.seed_, 0.05);
        ImGui::DragFloat("speed", &speed, 0.01);
    }

    if (first_run)
        ImGui::SetNextTreeNodeOpen(true);

    if (ImGui::CollapsingHeader("Fog Options")) {
        ImGui::RadioButton("linear", &terrain.fog_type_, 0); ImGui::SameLine();
        ImGui::RadioButton("exponential", &terrain.fog_type_, 1);

        ImGui::SliderFloat("start", &terrain.fog_start_, 20, 100);
        ImGui::SliderFloat("end", &terrain.fog_end_, 20, 100);
        ImGui::SliderFloat3("color", fog_color, 0.0, 1.0);
        ImGui::DragFloat("density", &terrain.fog_density_, 0.0001);
        ImGui::DragFloat("power", &terrain.fog_power_, 0.005);

        terrain.fog_color_[0] = fog_color[0];
        terrain.fog_color_[1] = fog_color[1];
        terrain.fog_color_[2] = fog_color[2];

        water.fog_type_ = terrain.fog_type_;
        water.fog_start_ = terrain.fog_start_;
        water.fog_end_ = terrain.fog_end_;
        water.fog_color_[0] = fog_color[0];
        water.fog_color_[1] = fog_color[1];
        water.fog_color_[2] = fog_color[2];
        water.fog_density_ = terrain.fog_density_;
        water.fog_power_ = terrain.fog_power_;

        glClearColor(fog_color[0], fog_color[1], fog_color[2], 1.0);
    }

    if (first_run)
        ImGui::SetNextTreeNodeOpen(true);

    if (ImGui::CollapsingHeader("Texture Options")) {
        ImGui::DragFloat("fcolor", &terrain.fcolor_, 0.005);
        ImGui::DragFloat("fslope", &terrain.fslope_, 0.005);
        ImGui::DragFloat("fheight", &terrain.fheight_, 0.005);
        ImGui::DragFloat("hsnow", &terrain.hsnow_, 0.005);
        ImGui::DragFloat("fsnow", &terrain.fsnow_, 0.005);
    }

    if (first_run)
        ImGui::SetNextTreeNodeOpen(true);

    if (ImGui::CollapsingHeader("Harmonic Options")) {
        ImGui::RadioButton("fBm", &heightmap.type_, 0); ImGui::SameLine();
        ImGui::RadioButton("ridged fBm", &heightmap.type_, 1); ImGui::SameLine();
        ImGui::RadioButton("billowy fBm", &heightmap.type_, 2);
        ImGui::RadioButton("swiss turbulence", &heightmap.type_, 3);

        ImGui::SliderFloat("H", &heightmap.H_, 0.01, 2.0);
        ImGui::SliderFloat("lacunarity", &heightmap.lacunarity_, 0.8, 3.0);
        ImGui::SliderFloat("warp", &heightmap.warp_, 0.01, 2.0);
        ImGui::SliderInt("octaves", &heightmap.octaves_, 1, 24);
    }

    if (first_run)
        ImGui::SetNextTreeNodeOpen(true);

    if (show_lighting) {
        ImGui::Begin("Lighting", &show_lighting, 0);

        ImGui::SliderFloat("hour", &hour, 6.0, 20.0);
        ImGui::SliderFloat("sun delta", &skybox.sun_delta_, -1.0, 1.0);

        float diffuse_color_arr[3] = {
            terrain.diffuse_color_[0],
            terrain.diffuse_color_[1],
            terrain.diffuse_color_[2],
        };

        ImGui::DragFloat("diffuse", &terrain.diffuse_, 0.005);
        ImGui::ColorEdit3("diffuse color", diffuse_color_arr);
        //ImGui::DragFloat("specular", &terrain.specular_, 0.005);
        //ImGui::DragFloat("alpha", &terrain.alpha_, 0.5);

        ImGui::SliderFloat("light bias min", &terrain.light_bias_min_, 0.1, 10);
        ImGui::SliderFloat("light bias max", &terrain.light_bias_max_, 0.1, 20);
        ImGui::SliderFloat2("SM edges", sm_edges, 0.0, 300.0);

        ImGui::Image((void*)shadows_tex_id, ImVec2(512, 512));

        ImGui::SliderFloat("rotate X", &skybox.rotX_, 0.0, 2*3.142);
        ImGui::SliderFloat("rotate Y", &skybox.rotY_, 0.0, 2*3.142);
        ImGui::SliderFloat("rotate Z", &skybox.rotZ_, 0.0, 2*3.142);
        ImGui::End();
    }

    skybox.hour_ = 24.0 - hour + 2.0;
    terrain.diffuse_color_ = evaluate_piecewise_bezier(diffuse_controls, hour / 24.0f);

    if(first_run)
        ImGui::SetNextTreeNodeOpen(true);

    // Updating lighting
    float sun_angle = M_PI * (skybox.hour_ - 7.0) / 12.0;
    light_pos = vec3(300.0 * cos(sun_angle), 0.0, 300.0 * sin(sun_angle));
    mat4 light_projection = ortho(-sm_edges[0], sm_edges[1], -300.0f, 300.0f, 0.1f, 800.0f);
    mat4 light_view = lookAt(light_pos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    light_matrix = light_projection * light_view;

    terrain.SetLighting(light_pos, light_matrix);

    cam_dir.x -= cam_vel[3] * dt;

    vec3 cam_target;

    // Updating camera
    if (cam_type == CAMERA_FREE || cam_type == CAMERA_FPS) {
        float cam_speed = 1.0f;

        if (cam_type == CAMERA_FREE)
            cam_speed = glm::max(0.5f, (float)pow(abs(cam_pos.z), 0.8f));

        for (int i = 0; i < 4; i++)
            cam_vel[i] += dt * (30.0 * cam_acc[i] - cam_vel[i] * 15.0);

        cam_target = vec3(
            sin(cam_dir.y) * cos(cam_dir.x),
            sin(cam_dir.y) * sin(cam_dir.x),
            cos(cam_dir.y)
        );

        vec2 cam_dir_2d(-cos(cam_dir.x), -sin(cam_dir.x));

        if (cam_type == CAMERA_FREE && !lock_height)
            cam_pos.z += dt * 60.0f * cam_speed * cam_vel[0] * cam_target.z;
        else if (cam_type == CAMERA_FPS) {
            float dz = heightmap.GetCenterHeight(cam_pos[0], cam_pos[1]) + 1.0f - cam_pos.z;
            cam_vel.z += dt * (200.0 * dz - cam_vel.z * 10.0);
        }

        cam_pos.z += dt * cam_vel[2] * cam_speed;

        if (cam_type == CAMERA_FREE) {
            hoffset[0] += speed * dt * cam_target.x + dt * cam_vel[0] * cam_speed * cam_target.x - dt * cam_vel[1] * cam_speed * cam_target.y;
            hoffset[1] += speed * dt * cam_target.y + dt * cam_vel[0] * cam_speed * cam_target.y + dt * cam_vel[1] * cam_speed * cam_target.x;
        } else {
            hoffset[0] += speed * dt * cam_dir_2d.x + dt * cam_vel[0] * cam_speed * cam_dir_2d.x - dt * cam_vel[1] * cam_speed * cam_dir_2d.y;
            hoffset[1] += speed * dt * cam_dir_2d.y + dt * cam_vel[0] * cam_speed * cam_dir_2d.y + dt * cam_vel[1] * cam_speed * cam_dir_2d.x;
        }
    } else if (cam_type == CAMERA_PATH) {
        timer = glm::clamp(timer + cam_acc[0] * dt, 0.0f, 1.0f);
        cam_dir = evaluate_piecewise_bezier(orientation_controls, timer);
        vec3 b_cam_pos = evaluate_piecewise_bezier(camera_controls, timer);
        hoffset[0] = b_cam_pos.x;
        hoffset[1] = b_cam_pos.y;
        cam_pos.z = b_cam_pos.z;

        cam_target = vec3(
            sin(cam_dir.y) * cos(cam_dir.x),
            sin(cam_dir.y) * sin(cam_dir.x),
            cos(cam_dir.y)
        );
    } else {
        hour = glm::clamp(hour + cam_acc[0] * 2 * dt, 6.0f, 20.0f);
        skybox.hour_ = 24.0 - hour + 2.0;
        terrain.diffuse_color_ = bezier(diffuse_controls, hour / 24.0f);

        cam_target = vec3(
            sin(cam_dir.y) * cos(cam_dir.x),
            sin(cam_dir.y) * sin(cam_dir.x),
            cos(cam_dir.y)
        );
    }

    terrain.hoffset_.x = heightmap.dx_ = floor(hoffset[0]*grid_area*2/grid_tesselation)*grid_tesselation/grid_area/2;
    terrain.hoffset_.y = heightmap.dy_ = floor(hoffset[1]*grid_area*2/grid_tesselation)*grid_tesselation/grid_area/2;
    cam_pos.x = (hoffset[0] - heightmap.dx_)*100;
    cam_pos.y = (hoffset[1] - heightmap.dy_)*100;

    heightmap.Draw();

    vec3 cam_look = cam_pos + cam_target;
    view_matrix = lookAt(cam_pos, cam_look, cam_up);

    terrain.cam_pos_ = cam_pos;
    water.cam_pos_ = cam_pos;

    first_run = false;
}

void Display() {
    float time = glfwGetTime();


    /*
    vec3 bPoints = getBezierPoint(time);
    cam_pos[2] = bPoints[2];
    cam_dir = vec2(bPoints[0], bPoints[1]);
    */

    {
        water_reflection.Bind();

        vec3 cam_target(
            sin(cam_dir.y) * cos(cam_dir.x),
            sin(cam_dir.y) * sin(cam_dir.x),
            -cos(cam_dir.y)
        );

        vec3 cam_pos_invert = cam_pos;
        cam_pos_invert.z = -cam_pos_invert.z;
        vec3 cam_look = cam_pos_invert + cam_target;
        mat4 view_matrix_ = lookAt(cam_pos_invert, cam_look, cam_up);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       
        skybox.Draw(cam_pos_invert, IDENTITY_MATRIX, view_matrix_, framebuffer_projection_matrix);

        glEnable(GL_CLIP_DISTANCE0);
        terrain.SetClipPlane(vec4(0.0f, 0.0f, 1.0f, 0.0f));
        terrain.Draw(IDENTITY_MATRIX, view_matrix_, framebuffer_projection_matrix);
        terrain.SetClipPlane(vec4(0.0f));
        glDisable(GL_CLIP_DISTANCE0);

        water_reflection.Unbind();
    }

    {
        shadows.Bind();
        glClear(GL_DEPTH_BUFFER_BIT);
        terrain.Draw(light_matrix);
        shadows.Unbind();
    }

    glViewport(0, 0, window_width, window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skybox.Draw(cam_pos, IDENTITY_MATRIX, view_matrix, projection_matrix);
    terrain.Draw(IDENTITY_MATRIX, view_matrix, projection_matrix);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    water.Draw(IDENTITY_MATRIX, view_matrix, projection_matrix);
    glDisable(GL_BLEND);
}

void ResizeCallback(GLFWwindow* window, int width, int height) {
    window_width = width;
    window_height = height;
    water_framebuffer_width = (int) window_width*FRAMEBUFFER_RATIO;
    water_framebuffer_height = (int) window_height*FRAMEBUFFER_RATIO;

    projection_matrix = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
    framebuffer_projection_matrix = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);

    glViewport(0, 0, width, height);

    water_reflection.Cleanup();
    water.SetReflection(water_reflection.Init(water_framebuffer_width, water_framebuffer_height));
}

void ErrorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (ImGui::GetIO().WantCaptureKeyboard) {
        ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);
        return;
    }

    if (action == GLFW_PRESS) {
        switch (key) {

            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_W:
                cam_acc[0] += CAMERA_SPEED;
                break;
            case GLFW_KEY_S:
                cam_acc[0] -= CAMERA_SPEED;
                break;
            case GLFW_KEY_A:
                cam_acc[1] += CAMERA_SPEED;
                break;
            case GLFW_KEY_D:
                cam_acc[1] -= CAMERA_SPEED;
                break;
            case GLFW_KEY_F:
                cam_acc[2] -= CAMERA_SPEED * 20;
                break;
            case GLFW_KEY_R:
                cam_acc[2] += CAMERA_SPEED * 20;
                break;
            case GLFW_KEY_Q:
                cam_acc[3] -= CAMERA_SPEED * 20;
                break;
            case GLFW_KEY_E:
                cam_acc[3] += CAMERA_SPEED * 20;
                break;

            default:
                ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);
        }
    }

    if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_W:
                cam_acc[0] -= CAMERA_SPEED;
                break;
            case GLFW_KEY_S:
                cam_acc[0] += CAMERA_SPEED;
                break;
            case GLFW_KEY_A:
                cam_acc[1] -= CAMERA_SPEED;
                break;
            case GLFW_KEY_D:
                cam_acc[1] += CAMERA_SPEED;
                break;
            case GLFW_KEY_F:
                cam_acc[2] += CAMERA_SPEED * 20;
                break;
            case GLFW_KEY_R:
                cam_acc[2] -= CAMERA_SPEED * 20;
                break;
            case GLFW_KEY_Q:
                cam_acc[3] += CAMERA_SPEED * 20;
                break;
            case GLFW_KEY_E:
                cam_acc[3] -= CAMERA_SPEED * 20;
                break;

            default:
                ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);
        }
    }

    if(abs(cam_acc[0]) <  10e-4) {
        cam_acc[0] = 0;
    }

}

void CharCallback(GLFWwindow *window, unsigned int codepoint) {
    ImGui_ImplGlfwGL3_CharCallback(window, codepoint);
}

bool cameraMoving = false;
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);

    cameraMoving = button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !ImGui::GetIO().WantCaptureMouse;
}

void CursorPosCallback(GLFWwindow *window, double posx, double posy) {
    const float da = M_PI / 2000.0;
    static double last_posx = posx, last_posy = posy;

    if (cameraMoving && !ImGui::GetIO().WantCaptureMouse) {
        double dx = posx - last_posx, dy = posy - last_posy;

        cam_dir.x -= dx * da;
        cam_dir.y -= dy * da;
    }

    last_posx = posx; last_posy = posy;
}

void MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    ImGui_ImplGlfwGL3_ScrollCallback(window, xoffset, yoffset);
}

int main(int argc, char *argv[]) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(ErrorCallback);

    // http://www.glfw.org/faq.html#how-do-i-create-an-opengl-30-context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(window_width, window_height,
                                          WINDOW_TITLE, NULL, NULL);
    if(!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCharCallback(window, CharCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, MouseScrollCallback);

    glfwSetFramebufferSizeCallback(window, ResizeCallback);

    // https://www.opengl.org/wiki/OpenGL_Loading_Library
    glewExperimental = GL_TRUE; // fixes glew error (see above link)
    if(glewInit() != GLEW_NO_ERROR) {
        fprintf( stderr, "Failed to initialize GLEW\n");
        return EXIT_FAILURE;
    }

    cout << "OpenGL" << glGetString(GL_VERSION) << endl;

    // init ImGui
    ImGui_ImplGlfwGL3_Init(window, false);

    Init(window);

    float lastTime = glfwGetTime();
    // render loop
    while(!glfwWindowShouldClose(window)){
        float dt = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        ImGui_ImplGlfwGL3_NewFrame();

        Update(dt);

        Display();
        ImGui::Render();
        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    
    terrain.Cleanup();
    heightmap.Cleanup();
    water.Cleanup();
    water_reflection.Cleanup();
    skybox.Cleanup();
    shadows.Cleanup();
    sq.Cleanup();

    ImGui_ImplGlfwGL3_Shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
