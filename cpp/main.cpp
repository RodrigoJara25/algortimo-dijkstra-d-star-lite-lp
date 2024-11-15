#include "raylib.h"
#include "getResolution/getResolution.h"
#include "dijkstra/dijkstra.h"
#include "TDA/nodo/Nodo.h"
#include <iostream>
#include <vector>
#include <limits>
#include <unordered_set>
#include <utility>

struct Celda
{
    int fila;
    int columna;

    // Constructor para inicializar la celda
    Celda(int f, int c) : fila(f), columna(c) {}
};

int main()
{
    // Inicialización de la ventana
    int screenWidth = 0;
    int screenHeight = 0;
    GetDesktopResolution(screenWidth, screenHeight);
    InitWindow(screenWidth, screenHeight - 60, "Grilla Celeste Claro");

    // Configuración de colores
    Color colorCelda = {184, 237, 255, 255};      // Color celeste claro
    Color colorBorde = {200, 200, 200, 255};      // Color plomo
    Color colorCeldaBloqueada = {255, 0, 0, 255}; // Color rojo
    Color colorCeldaOrigen = {4, 255, 0, 255};    // Color verde
    Color colorCeldaDestino = {255, 234, 0, 255}; // Color amarillo
    Color colorCamino = {0, 0, 255, 255};         // Color azul

    std::vector<std::pair<int, int>> desplazamientos = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // Parámetros de la grilla
    int columnas = 20;
    int filas = 15;
    int tamanoCeldaAncho = screenWidth / columnas;
    int tamanoCeldaAlto = (screenHeight - 60)  / filas;

    // Inicialización de la grilla
    std::vector<std::vector<Color>> celdas(filas, std::vector<Color>(columnas, colorCelda));

    // Conjunto de celdas bloqueadas (usamos un unordered_set para eficiencia en búsquedas)
    std::vector<Celda> celdas_bloqueadas;

    // Variables de celdas especiales
    std::tuple<int, int> celdaOrigen = {-1, -1};
    std::tuple<int, int> celdaDestino = {-1, -1};
    bool modoEdicion = false;

    // Bucle principal
    while (!WindowShouldClose())
    {
        // Manejo de entrada
        if (IsKeyPressed(KEY_LEFT_CONTROL) || IsKeyPressed(KEY_RIGHT_CONTROL))
        {
            modoEdicion = !modoEdicion;
        }

        if (IsKeyPressed(KEY_ENTER))
        {

            std::vector<Nodo *> nodos;
            for (int i = 0; i < filas; i++)
            {
                for (int j = 0; j < columnas; j++)
                {
                    // Crear coordenadas para el nodo
                    std::tuple<int, int> coordenadas = {i, j};

                    // Crear el nodo con las coordenadas actuales
                    Nodo *nodo = new Nodo(coordenadas);

                    // Recorrer los posibles desplazamientos (adyacentes)
                    for (auto &[dx, dy] : desplazamientos)
                    {
                        int adyacente_fila = i + dx;
                        int adyacente_columna = j + dy;

                        // Verifica que el vecino esté dentro de los límites de la grilla
                        if (0 <= adyacente_fila && adyacente_fila < filas && 0 <= adyacente_columna && adyacente_columna < columnas)
                        {
                            std::pair<int, int> adyacente = {adyacente_fila, adyacente_columna};

                            bool esCeldaBloqueada = false;

                            // Verifica si la celda adyacente está bloqueada
                            for (const auto &celda : celdas_bloqueadas)
                            {
                                if (celda.fila == adyacente_fila && celda.columna == adyacente_columna)
                                {
                                    esCeldaBloqueada = true;
                                    break; // No necesitamos seguir buscando, ya que hemos encontrado la celda bloqueada
                                }
                            }

                            // Si la celda adyacente está bloqueada, no se añade como adyacente
                            if (!esCeldaBloqueada)
                            {
                                // Configura el adyacente con el peso correspondiente
                                nodo->configurarAdyacente(adyacente, 1.0f); // Se da un peso de 1 a las celdas no bloqueadas
                            }
                        }
                    }

                    // Agregar el nodo a la lista de nodos
                    nodos.push_back(nodo);
                }
            }

            // Llamada a dijkstra
            std::pair<std::vector<std::tuple<int, int>>, float> resultado = dijkstra(nodos, celdaOrigen, celdaDestino);

            std::vector<std::tuple<int, int>> camino = resultado.first;
            float distancia = resultado.second;

            // Imprimir resultado

            for (auto &[x, y] : camino)
            {
                if (std::make_tuple(x, y) != celdaOrigen && std::make_tuple(x, y) != celdaDestino)
                {
                    celdas[x][y] = colorCamino;
                }
            }

            // Liberar memoria dinámica
            for (auto &nodo : nodos)
            {
                delete nodo;
            }
        }

        // Detección del clic y posición del mouse en la grilla
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            int xMouse = GetMouseX() / tamanoCeldaAncho;
            int yMouse = GetMouseY() / tamanoCeldaAlto;

            if (xMouse >= 0 && xMouse < columnas && yMouse >= 0 && yMouse < filas)
            {
                if (modoEdicion)
                {

                    celdas[yMouse][xMouse] = colorCeldaBloqueada;
                    celdas_bloqueadas.push_back(Celda(yMouse, xMouse));
                    std::cout << "Celdas bloqueadas:" << std::endl;
                    for (const auto &celda : celdas_bloqueadas)
                    {
                        std::cout << "Celda: (" << celda.fila << ", " << celda.columna << ")\n";
                    }
                }
                else if (std::get<0>(celdaOrigen) == -1 && std::get<1>(celdaOrigen) == -1)
                {

                    celdaOrigen = std::make_tuple(yMouse, xMouse);
                    std::cout << "Origen: (" << std::get<0>(celdaOrigen) << ", " << std::get<1>(celdaOrigen) << ")\n";
                    celdas[yMouse][xMouse] = colorCeldaOrigen;
                }
                else if (std::get<0>(celdaDestino) == -1 && std::get<1>(celdaDestino) == -1)
                {
                    celdaDestino = std::make_tuple(yMouse, xMouse);
                    celdas[yMouse][xMouse] = colorCeldaDestino;
                    std::cout << "Destino: (" << std::get<0>(celdaDestino) << ", " << std::get<1>(celdaDestino) << ")\n";
                }
            }
        }

        // Comienzo del dibujo
        BeginDrawing();
        ClearBackground(RAYWHITE); // Limpia la pantalla con un color de fondo

        // Dibujar la grilla
        for (int y = 0; y < filas; y++)
        {
            for (int x = 0; x < columnas; x++)
            {
                // Calcula la posición de cada celda
                int posX = x * tamanoCeldaAncho;
                int posY = y * tamanoCeldaAlto;

                // Dibuja el fondo de la celda
                DrawRectangle(posX, posY, tamanoCeldaAncho, tamanoCeldaAlto, celdas[y][x]);

                // Dibuja el borde de la celda
                DrawRectangleLines(posX, posY, tamanoCeldaAncho, tamanoCeldaAlto, colorBorde);
            }
        }

        EndDrawing(); // Actualiza la pantalla con los dibujos realizados
    }

    // Finaliza la ventana
    CloseWindow();

    return 0;
}
