#include "raylib.h"
#include "getResolution/getResolution.h"
#include "dijkstra/dijkstra.h"
#include "TDA/nodoDijkstra/NodoDijkstra.h"
#include "dStarLite/dStarLite.h"
#include <iostream>
#include <vector>
#include <limits>
#include <tuple>
using namespace std;

struct Celda
{
    int fila;
    int columna;
    Celda(int f, int c) : fila(f), columna(c) {}
};

auto heuristic_fn = [](pair<int, int> start, pair<int, int> goal) -> float
{
    return abs(start.first - goal.first) + abs(start.second - goal.second); // Manhattan distance
};

auto prev_fn = [](pair<int, int> node) -> std::vector<pair<int, int>>
{
    std::vector<pair<int, int>> neighbors;
    int x = node.first, y = node.second;
    neighbors.push_back({x - 1, y}); // Left
    neighbors.push_back({x + 1, y}); // Right
    neighbors.push_back({x, y - 1}); // Up
    neighbors.push_back({x, y + 1}); // Down
    return neighbors;
};

auto next_fn = prev_fn; // For simplicity, using the same logic as prev_fn

auto cost_fn = [](pair<int, int> a, pair<int, int> b) -> float
{
    return 1.0f; // Uniform cost
};

void on_expand(tuple<int, int> node)
{
    cout << "Expanding node: (" << std::get<0>(node) << ", " << std::get<1>(node) << ")" << endl;
}

int main()
{
    int screenWidth = 0, screenHeight = 0;
    GetDesktopResolution(screenWidth, screenHeight);
    InitWindow(screenWidth, screenHeight - 60, "Grilla Celeste Claro");

    Color colorCelda = {184, 237, 255, 255};
    Color colorBorde = {200, 200, 200, 255};
    Color colorCeldaBloqueada = {255, 0, 0, 255};
    Color colorCeldaOrigen = {4, 255, 0, 255};
    Color colorCeldaDestino = {255, 234, 0, 255};
    Color colorCamino = {0, 0, 255, 255};

    std::vector<std::tuple<int, int>> desplazamientos = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int columnas = 20, filas = 15;
    int tamanoCeldaAncho = screenWidth / columnas;
    int tamanoCeldaAlto = (screenHeight - 60) / filas;

    std::vector<std::vector<Color>> celdas(filas, std::vector<Color>(columnas, colorCelda));
    std::vector<Celda> celdas_bloqueadas;
    tuple<int, int> celdaOrigen = {-1, -1};
    tuple<int, int> celdaDestino = {-1, -1};
    bool modoEdicion = false;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_LEFT_CONTROL) || IsKeyPressed(KEY_RIGHT_CONTROL))
        {
            modoEdicion = !modoEdicion;
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            std::vector<NodoDijkstra *> nodos;
            for (int i = 0; i < filas; i++)
            {
                for (int j = 0; j < columnas; j++)
                {
                    // Crear coordenadas para el nodo
                    std::tuple<int, int> coordenadas = {i, j};

                    // Crear el nodo con las coordenadas actuales
                    NodoDijkstra *nodo = new NodoDijkstra(coordenadas);

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

        if (IsKeyPressed(KEY_SPACE))
        {
            pair<int, int> origen = make_pair(std::get<0>(celdaOrigen), std::get<1>(celdaOrigen));
            pair<int, int> destino = make_pair(std::get<0>(celdaDestino), std::get<1>(celdaDestino));

            DStarLite dstar(heuristic_fn, prev_fn, next_fn, cost_fn);
            std::vector<std::pair<int, int>> path;
            std::vector<std::pair<int, int>> celdas_bloqueadas_pairs;
            for (const auto &celda : celdas_bloqueadas)
            {
                celdas_bloqueadas_pairs.push_back({celda.fila, celda.columna});
            }

            path = dstar.plan(origen, destino, on_expand, celdas_bloqueadas_pairs);

            for (const auto &p : path)
            {
                celdas[p.first][p.second] = colorCamino;
            }
        }

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

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
                DrawRectangle(j * tamanoCeldaAncho, i * tamanoCeldaAlto, tamanoCeldaAncho, tamanoCeldaAlto, celdas[i][j]);
                DrawRectangleLines(j * tamanoCeldaAncho, i * tamanoCeldaAlto, tamanoCeldaAncho, tamanoCeldaAlto, colorBorde);
            }
        }

        DrawText("Presiona CTRL para cambiar a modo edicion", 10, screenHeight - 50, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
