cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(RayTracer)

# Estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar todos los .cpp dentro de /src
file(GLOB SOURCES "src/*.cpp")

# Crear el ejecutable
add_executable(${PROJECT_NAME} ${SOURCES})