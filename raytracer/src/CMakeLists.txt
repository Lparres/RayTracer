cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(MiProyecto)

# Estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Recoger todos los archivos .cpp del directorio
file(GLOB SOURCES "*.cpp")

# Crear el ejecutable
add_executable(${PROJECT_NAME} ${SOURCES})