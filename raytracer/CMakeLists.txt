cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(RayTracer)

# Estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar todos los .cpp dentro de /src
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Crear el ejecutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Incluir directorios para las cabeceras
file(GLOB_RECURSE SRC_DIRS CONFIGURE_DEPENDS LIST_DIRECTORIES true ${PROJECT_SOURCE_DIR}/src/*)

set(INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src)

foreach(path ${SRC_DIRS})
    if(IS_DIRECTORY "${path}")
        list(APPEND INCLUDE_DIRS "${path}")
    endif()
endforeach()

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/external
)

# Para búsquedas desde la raíz del proyecto
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        PROJECT_ROOT="${PROJECT_SOURCE_DIR}"
        IMAGES_DIR="${PROJECT_SOURCE_DIR}/images"
)