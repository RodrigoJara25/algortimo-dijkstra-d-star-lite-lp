import pygame
import sys
from dijkstra.dijkstra import Dijkstra
from dstartlite.dstartlite import DStarLite
from TDA.nodo.nodoDijkstra import NodoDijkstra
from TDA.nodo.nodoDstartlite import NodoDStartLite


def crear_grilla():
    # Inicializar pygame
    pygame.init()
    
    # Obtener el tamaño de la pantalla del ordenador
    info_pantalla = pygame.display.Info()
    ancho_pantalla = info_pantalla.current_w
    alto_pantalla = info_pantalla.current_h - 76  # Usamos el tamaño completo disponible de la pantalla
    
    # Configurar pantalla
    pantalla = pygame.display.set_mode((ancho_pantalla, alto_pantalla), pygame.RESIZABLE)
    pygame.display.set_caption("Grilla Celeste Claro")

    # Colores
    color_celda = (184, 237, 255)  # Fondo de las celdas
    color_borde = (200, 200, 200)  # Borde de las celdas
    color_celda_bloqueadas = (255, 0, 0)  # Celdas bloqueadas
    color_celda_origen = (4, 255, 0)  # Celda de origen
    color_celda_destino = (255, 234, 0)  # Celda de destino
    color_celda_camino = (0, 0, 255)  # Camino más corto
    
    # Configuración de la grilla
    columnas = 20
    filas = 15
    
    # Diccionario para colores de las celdas
    colores_celdas = {
        color_celda_bloqueadas: color_celda_bloqueadas,
        color_celda_origen: color_celda_origen,
        color_celda_destino: color_celda_destino,
        color_celda_camino: color_celda_camino
    }

    # Calcular tamaño de cada celda
    tamano_celda_ancho = ancho_pantalla // columnas
    tamano_celda_alto = alto_pantalla // filas
    
    # Inicializar matriz de celdas
    celdas = [[color_celda for _ in range(columnas)] for _ in range(filas)]
    celda_origen = None
    celda_destino = None
    modoEdicion = False
    celdas_bloqueadas = []
    desplazamientos = [(-1, 0), (1, 0), (0, -1), (0, 1)]

    def configurar_default():
        nonlocal celdas, celda_origen, celda_destino, celdas_bloqueadas
        celdas = [[color_celda for _ in range(columnas)] for _ in range(filas)]
        celda_origen = None
        celda_destino = None
        celdas_bloqueadas = []

   
        


    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LCTRL or event.key == pygame.K_RCTRL:
                    modoEdicion = not modoEdicion
                elif event.key == pygame.K_ESCAPE:
                    configurar_default()
                elif event.key == pygame.K_RETURN:
                   
                    nodos = []
                    # Recorremos cada cuadrilla de la grilla
                    for fila in range(filas):
                        for columna in range(columnas):
                            # Vertice sera definido por una tuplas en la cual el cual tomara eje X y Y
                            vertice =(fila, columna)
                            # Creamos la clase Nodo que despues agregaremos a la lista de nodos
                            nodo = NodoDijkstra(vertice)
                            # Agregamos a sus nodos adyacentes
                            # tuple(fila - 1, columna - 1)
                            # tuple(fila, columna - 1)
                            # tuple(fila + 1, columna - 1)
                            # tuple(fila - 1, columna)
                            # tuple(fila + 1, columna)
                            # tuple(fila - 1, columna + 1)
                            # tuple(fila, columna + 1)
                            # tuple(fila + 1, columna + 1)
                            for dx, dy in desplazamientos:
                                adyacente_fila = fila + dx
                                adyacente_columna = columna + dy
                                # Verifica que el vecino esté dentro de los límites de la grilla
                                if 0 <= adyacente_fila < filas and 0 <= adyacente_columna < columnas:
                                    adyacente = (adyacente_fila, adyacente_columna)
                                    esCeldaBloqueada = (fila, columna) not in celdas_bloqueadas
                                    nodo.configurarAdyacente(adyacente, 1 if esCeldaBloqueada else float('inf'))

                            # Añade el nodo a la lista de nodos
                            nodos.append(nodo)
                           
                    # Llamamos al funcion dijkstra
                    camino , distancia = Dijkstra(nodos, celda_origen, celda_destino)
                    for x,y in camino:
                        if (x,y) != celda_origen and (x,y) != celda_destino:
                            celdas[x][y] = color_celda_camino
                elif event.key == pygame.K_SPACE:
                    def heuristic_fn(ref, node):
                        # Example heuristic function (Manhattan Distance)
                        return abs(ref[0] - node[0]) + abs(ref[1] - node[1])

                    def prev_fn(node):
                        # Example previous nodes (for simplicity, just return neighbors)
                        x, y = node
                        return [(x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)]

                  

                    def next_fn(node):
                        x, y = node
                        vecinos = [(x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)]
                       
                        return vecinos

                    def cost_fn(u, v):
                        # Example cost function (uniform cost for all edges)
                        return 1.0

                    # Example on_expand callback function to process a node during the pathfinding process
                    def on_expand(node):
                        print(f"Expanding node: {node}")
                        

                    print(celda_origen, celda_destino)

                    # Initialize DStar with the functions
                    dstar_lite = DStarLite(heuristic_fn, prev_fn, next_fn, cost_fn)
                    path = list(dstar_lite.plan(celda_origen, celda_destino, on_expand, celdas_bloqueadas))
                    print("Path:", path)
                 

                    # Visualize the path
                    if path:
                        for x, y in path:
                            if (x, y) != celda_origen and (x, y) != celda_destino:
                                celdas[x][y] = color_celda_camino  # Color the cells that are part of the path


            elif event.type == pygame.MOUSEBUTTONDOWN:
                x_mouse, y_mouse = pygame.mouse.get_pos()
                columna = x_mouse // tamano_celda_ancho
                fila = y_mouse // tamano_celda_alto
                if 0 <= columna < columnas and 0 <= fila < filas:
                    if modoEdicion: 
                        if event.button == 1:
                            celdas_bloqueadas.append((fila, columna))
                            celdas[fila][columna] = color_celda_bloqueadas
                        elif event.button == 3:
                            celdas_bloqueadas.remove((fila, columna))
                            celdas[fila][columna] = color_celda
                    elif celda_origen is None:
                        celda_origen = (fila, columna)
                        celdas[fila][columna] = color_celda_origen
                    elif celda_destino is None:
                        celda_destino = (fila, columna)
                        celdas[fila][columna] = color_celda_destino

        pantalla.fill((255, 255, 255))
        for fila in range(filas):
            for columna in range(columnas):
                x = columna * tamano_celda_ancho
                y = fila * tamano_celda_alto
                color_fondo = colores_celdas.get(celdas[fila][columna], color_celda)
                pygame.draw.rect(pantalla, color_fondo, (x, y, tamano_celda_ancho, tamano_celda_alto))
                pygame.draw.rect(pantalla, color_borde, (x, y, tamano_celda_ancho, tamano_celda_alto), 1)
        pygame.display.flip()

crear_grilla()
