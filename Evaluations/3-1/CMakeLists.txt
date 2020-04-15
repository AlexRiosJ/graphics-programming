cmake_minimum_required(VERSION 3.0)
project( Sphere )

# Includes
include_directories(../../Utils)

# Executables
add_executable(main main.c)

# Libraries
add_library(utils ../../Utils/utils.c)
add_library(transforms ../../Utils/transforms.c)
add_library(mat4 ../../Utils/mat4.c)

# Link dirs

# Link Libs
target_link_libraries(main utils mat4 transforms glew32 freeglut opengl32)

# After build
file(GLOB LIBRARIES "../../Libraries/*.dll")
file(COPY ${LIBRARIES} DESTINATION .)


