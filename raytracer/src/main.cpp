#include "Demo1.hpp"
#include "Demo2.hpp"
#include "Demo3.hpp"
#include "Demo4.hpp"
#include "Demo5.hpp"
#include "Demo6.hpp"

#include <chrono>
#include <iomanip>
#include <fstream>
#include <memory>

int main(void)
{

    std::vector<std::shared_ptr<DemoScene>> demos;
    demos.push_back(std::make_shared<Demo4>());
    demos.push_back(std::make_shared<Demo6>());
    demos.push_back(std::make_shared<Demo5>());
    demos.push_back(std::make_shared<Demo1>());

    for(auto demo : demos) {
        std::cout << "----- ESCENA '" << demo->name << "' -----\n";

        // ----- Carga de texturas -----
        std::cout << "Cargando texturas...\n";
        const auto texturesStartTime = std::chrono::steady_clock::now();

        demo->loadTextures();

        const auto texturesEndTime = std::chrono::steady_clock::now();
        const std::chrono::duration<double> texturesElapsedSeconds = texturesEndTime - texturesStartTime;
        std::cout << std::fixed << std::setprecision(2)
                  << "Tiempo de carga de texturas: " << texturesElapsedSeconds.count() << " s\n";


        // ----- Creación de materiales -----
        std::cout << "Cargando materiales...\n";
        const auto materialsStartTime = std::chrono::steady_clock::now();

        demo->loadMaterials();

        const auto materialsEndTime = std::chrono::steady_clock::now();
        const std::chrono::duration<double> materialsElapsedSeconds = materialsEndTime - materialsStartTime;
        std::cout << std::fixed << std::setprecision(2)
                  << "Tiempo de creacion de materiales: " << materialsElapsedSeconds.count() << " s\n";


        // ----- Instanciación de la escena ------
        std::cout << "Instanciando escena...\n";
        const auto sceneStartTime = std::chrono::steady_clock::now();

        demo->loadScene();

        const auto sceneEndTime = std::chrono::steady_clock::now();
        const std::chrono::duration<double> sceneElapsedSeconds = sceneEndTime - sceneStartTime;
        std::cout << std::fixed << std::setprecision(2)
                << "Tiempo de instanciacion de escena: " << sceneElapsedSeconds.count() << " s\n";


        // ----- Renderizamos -----
        try {
            demo->renderScene();
        }
        catch(std::string e) {
            std::cerr << "Error: " << e << "\n";
        }
        catch(char* e) {
            std::cerr << "Error: " << e << "\n";
        }
        catch(std::exception e) {
            std::cerr << "Error: " << e.what() << "\n";
        }

        // ----- Exportamos el resultado a un archivo PPM -----
        std::cout << "Exportando resultado...\n";
        const auto writeStart = std::chrono::steady_clock::now();

        demo->exportResult();

        const auto writeEnd = std::chrono::steady_clock::now();
        const std::chrono::duration<double> writeElapsed = writeEnd - writeStart;
        std::cout << std::fixed << std::setprecision(2)
                  << "Tiempo de exportacion PPM: " << writeElapsed.count() << " s\n\n";

    }

    return 0;
}
