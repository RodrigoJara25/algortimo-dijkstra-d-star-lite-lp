#include <iostream>
#include <vector>

#include <queue>
#include <utility>

#include "raylib.h"


int main(void)
{
    // Inicialización
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int celdaSize = 50;  // Tamaño de cada celda en la cuadrícula

    InitWindow(screenWidth, screenHeight, "Ejemplo de grilla en Raylib");

    // Define el color celeste claro
    Color celeste = {184, 237, 255, 255};  // #b8edff
    Color rojo = RED;  // Color rojo para celdas bloqueadas
    Color verde = {4, 255, 0, 255}; // Color verde para la celda de origen
    Color amarillo = {255, 234, 0, 255}; // Color amarillo para la celda de destino

    // Modo edicion
    bool modoEdicion = false;

    // Definir la cuadrícula
    const int num_columnas = screenWidth / celdaSize;  // Número de columnas
    const int num_filas = screenHeight / celdaSize; // Número de filas
    bool grid[num_columnas][num_filas] = {false}; // Inicializamos la cuadricula con todas las celdas desbloqueadas
    // false = desbloqueada
    // true = bloqueada

    // Celda de origen y destino (no deinidas, en raylib todas las coordenadas son positivas)
    Vector2 origen = {-1, -1};
    Vector2 destino = {-1, -1};

    SetTargetFPS(60);  // Configura el framerate deseado

    while (!WindowShouldClose())    // Detecta el botón de cierre de ventana o la tecla ESC
    {

        if (IsKeyPressed(KEY_LEFT_CONTROL)) {
            std::cout << "Modo edición " << (modoEdicion ? "desacivado" : "activado") << '\n';
            modoEdicion = !modoEdicion; // cada vez que se presione CTRL se altera el modo edicion            
        }

        // Dibujar la cuadrícula
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dibuja la cuadrícula
        for (int y = 0; y < num_filas; y++) {
            for (int x = 0; x < num_columnas; x++) {
                Color pintar = grid[x][y] ? rojo : celeste; // Color según el estado de bloqueo de la celda (activado : desactivado)
                
                // Comprobamos si estamos en el origen 
                if (origen.x == x && origen.y == y)
                {
                    pintar = verde;
                }
                // Comprobamos si estamos en el destino
                if (destino.x == x && destino.y == y)
                {
                    pintar = amarillo;
                }
                
                // Pintar celdas
                DrawRectangle(x * celdaSize, y * celdaSize, celdaSize, celdaSize, pintar); // Llena cada celda
                DrawLine(x * celdaSize, y * celdaSize, (x + 1) * celdaSize, y * celdaSize, BLACK); // Línea superior
                DrawLine(x * celdaSize, y * celdaSize, x * celdaSize, (y + 1) * celdaSize, BLACK); // Línea izquierda
                DrawLine((x + 1) * celdaSize, y * celdaSize, (x + 1) * celdaSize, (y + 1) * celdaSize, DARKGRAY); // Línea derecha
                DrawLine(x * celdaSize, (y + 1) * celdaSize, (x + 1) * celdaSize, (y + 1) * celdaSize, DARKGRAY); // Línea inferior
            }
        }

        // Manejo del modo de edicion
        if (modoEdicion == true)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // obtener posicion del mouse
                Vector2 posicion_mouse = GetMousePosition();

                // calcular la celda en que esta el mouse
                int pos_X = (int) posicion_mouse.x / celdaSize;
                int pos_Y = (int) posicion_mouse.y / celdaSize;
                    
                // Verificamos si las posiciones están dentro de los limites
                if (pos_X >= 0 && pos_X < num_columnas && pos_Y >= 0 && pos_Y < num_filas) {
                    // Colocamos la celda del mouse como bloqueada
                    grid[pos_X][pos_Y] = true;
                }
            }
            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
            {
                // obtener posicion del mouse
                Vector2 posicion_mouse = GetMousePosition();

                // calcular la celda en que esta el mouse
                int pos_X = (int) posicion_mouse.x / celdaSize;
                int pos_Y = (int) posicion_mouse.y / celdaSize;

                // Verificamos si las posiciones están dentro de los límites
                if (pos_X >= 0 && pos_X < num_columnas && pos_Y >= 0 && pos_Y < num_filas) {
                    // Colocamos la celda del mouse como desbloqueada
                    grid[pos_X][pos_Y] = false;
                }
            }
        }

        // Fuera del modo de edición, el aplicativo debe permitir seleccionar con el mouse (click izquierdo) una casilla origen y 
        // una casilla destino (de manera consecutiva). La casilla origen debe de pintarse del color verde (#04ff00) y la casilla 
        // destino de color amarillo (#ffea00).
        if (modoEdicion == false)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // Obtener posición del mouse
                Vector2 posicion_mouse = GetMousePosition();

                // Calcular la celda en la que está el mouse
                int pos_X = (int)posicion_mouse.x / celdaSize;
                int pos_Y = (int)posicion_mouse.y / celdaSize;

                // Verificamos si las posiciones están dentro de los límites
                if (pos_X >= 0 && pos_X < num_columnas && pos_Y >= 0 && pos_Y < num_filas) {
                    if (origen.x == -1 && origen.y == -1)
                    {
                        origen = {(float)pos_X, (float)pos_Y};
                    }
                    else{
                        destino = {(float)pos_X, (float)pos_Y};
                    }
                }
            }

            // Tecla ENTER para pinar la ruta
            if (IsKeyPressed(KEY_ENTER))
            {
                /* code */
            }
            
        }
        EndDrawing();
    }

    CloseWindow();  // Cierra la ventana y el contexto OpenGL

    return 0;
}
