get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")

# Only Copied once
file(GLOB TEXTURES
    terrain/texture/*.png
    terrain/terrain_texture.tga
    terrain/terrain_snow_texture.tga
    water/*.jpg
    quad/quad_texture.tga
    skybox/*.tga)
copy_files_once(${TEXTURES})


file(GLOB SHADERS
    skybox/*.glsl
    heightmap/*.glsl
    terrain/*.glsl
    quad/*.glsl
    water/*.glsl
    screenquad/*.glsl)
deploy_shaders_to_build_dir(${SHADERS})

add_executable(${EXERCISENAME} ${SOURCES} ${HEADERS} ${SHADERS})
target_link_libraries(${EXERCISENAME} ${COMMON_LIBS})
