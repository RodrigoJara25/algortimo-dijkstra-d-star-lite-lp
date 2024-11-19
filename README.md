Indicaciones Generales:
- Con CTRL alternas en modo edicion
- Cuando el modo edicion esta desactivado, puedes seleccionar dos casillas. Una de orgien y otra de destino.
- Cuando el modo edicion esta activado, puedes bloquear las celdas que desees.
- Al presionar ENTER, se imprime el camino con el algoritmo Dijkstra
- Al presionar ESPACE, se imprime el camino con el algoritmo D* Lite (de manera esática, no dinámica)
- Con la tecla ESC puedes limpiar todo el grid y volver a seleccionar Origen, Destino y Celdas Bloqueadas.

Ejecutar algoritmo en Python:
 1- Descargar el ambiente de trabajo para Python.
 
 2- Instalar Pygame (Ejecutar en erminal: pip install pygame)
 
 3- Colocarse dentro del directorio donde se encuentra el main.py y ejecutar desde la terminal (python main.py)

 Ejecutar algoritmo en C++:
 1- Entrar a la carpeta "cpp" desde la terminal
 
 2- Instalar el ambiente necesario para ejecutar con el compilador g++
 
 3- Ejecutar el comando "g++ -Wall -Icpp -Iinclude -Llib main.cpp -o main getResolution/getResolution.cpp TDA/nodoDijkstra/nodoDijkstra.cpp TDA/nodoDStarLite/NodoDStarLite.cpp dStarLite/dStarLite.cpp dijkstra/dijkstra.cpp -lraylib -lgdi32 -lwinmm"
