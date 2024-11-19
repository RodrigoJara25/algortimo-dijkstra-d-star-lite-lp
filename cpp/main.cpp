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
    SetExitKey(KEY_Q);

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
        if (IsKeyPressed(KEY_ESCAPE)) 
        {
            // Restablece todas las celdas al color original
            for (int i = 0; i < filas; i++) 
            {
                for (int j = 0; j < columnas; j++) 
                {
                    celdas[i][j] = colorCelda;
                }
            }

            // Limpia las celdas bloqueadas y resetea origen y destino
            celdas_bloqueadas.clear();
            celdaOrigen = {-1, -1};
            celdaDestino = {-1, -1};

            std::cout << "Grilla reiniciada\n";
        }
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
            if (std::get<0>(celdaOrigen) == -1 || std::get<1>(celdaDestino) == -1) {
                    std::cout << "Selecciona un origen y destino válidos antes de ejecutar el algoritmo.\n";
                } else {
                    const int INF = 1e9; // Representa "infinito"
                    const int filas = 20; // Tamaño de la grilla (ejemplo)
                    const int columnas = 20;

                    // Inicializa la grilla y los pesos
                    std::vector<std::vector<int>> costos(filas, std::vector<int>(columnas, 1)); // Todos los pesos son 1 inicialmente
                    for (const auto& bloqueada : celdas_bloqueadas) {
                        costos[bloqueada.fila][bloqueada.columna] = INF; // Marca celdas bloqueadas
                    }

                    // Variables necesarias
                    std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, std::greater<>> openList;
                    std::vector<std::vector<int>> g_values(filas, std::vector<int>(columnas, INF));
                    std::vector<std::vector<int>> rhs_values(filas, std::vector<int>(columnas, INF));
                    std::vector<std::vector<bool>> visited(filas, std::vector<bool>(columnas, false));

                    // Coordenadas de origen y destino
                    int xOrigen = std::get<0>(celdaOrigen), yOrigen = std::get<1>(celdaOrigen);
                    int xDestino = std::get<0>(celdaDestino), yDestino = std::get<1>(celdaDestino);

                    // Inicialización
                    rhs_values[xDestino][yDestino] = 0; // rhs del destino es 0
                    openList.push({0, xDestino, yDestino}); // Agrega el destino a la lista abierta

                    auto heuristica = [](int x1, int y1, int x2, int y2) {
                        return abs(x1 - x2) + abs(y1 - y2); // Distancia de Manhattan
                    };

                    // Función para obtener vecinos
                    auto obtenerVecinos = [&](int x, int y) {
                        std::vector<std::pair<int, int>> vecinos;
                        if (x > 0) vecinos.push_back({x - 1, y});
                        if (x < filas - 1) vecinos.push_back({x + 1, y});
                        if (y > 0) vecinos.push_back({x, y - 1});
                        if (y < columnas - 1) vecinos.push_back({x, y + 1});
                        return vecinos;
                    };

                    // Actualiza el valor de una celda
                    auto actualizar = [&](int x, int y) {
                        if (x == xDestino && y == yDestino) return;
                        int min_rhs = INF;
                        for (const auto& [nx, ny] : obtenerVecinos(x, y)) {
                            min_rhs = std::min(min_rhs, g_values[nx][ny] + costos[nx][ny]);
                        }
                        rhs_values[x][y] = min_rhs;
                        openList.push({rhs_values[x][y] + heuristica(x, y, xOrigen, yOrigen), x, y});
                    };

                    // Algoritmo principal
                    while (!openList.empty()) {
                        auto [_, x, y] = openList.top();
                        openList.pop();

                        if (visited[x][y]) continue;
                        visited[x][y] = true;

                        if (g_values[x][y] != rhs_values[x][y]) {
                            g_values[x][y] = rhs_values[x][y];
                            for (const auto& [nx, ny] : obtenerVecinos(x, y)) {
                                actualizar(nx, ny);
                            }
                        }
                    }

                    // Reconstrucción del camino
                    std::vector<std::pair<int, int>> camino;
                    int cx = xOrigen, cy = yOrigen;
                    while (!(cx == xDestino && cy == yDestino)) {
                        camino.push_back({cx, cy});
                        int min_cost = INF;
                        std::pair<int, int> siguiente = {cx, cy};
                        for (const auto& [nx, ny] : obtenerVecinos(cx, cy)) {
                            if (g_values[nx][ny] < min_cost) {
                                min_cost = g_values[nx][ny];
                                siguiente = {nx, ny};
                            }
                        }
                        cx = siguiente.first;
                        cy = siguiente.second;
                    }
                    camino.push_back({xDestino, yDestino});

                    // Pintar el camino en la grilla
                    for (const auto& [px, py] : camino) {
                        if (!(px == std::get<0>(celdaOrigen) && py == std::get<1>(celdaOrigen)) &&
                            !(px == std::get<0>(celdaDestino) && py == std::get<1>(celdaDestino))) {
                            celdas[px][py] = colorCamino; // Define un color para el camino
                        }
                    }

                    std::cout << "Camino encontrado y pintado.\n";
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
