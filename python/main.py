import pygame
import sys



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

    # Color celeste claro (fondo de la celda)
    color_celda = (184, 237, 255)  # RGB para #b8edff
    # Color del borde de la celda
    color_borde = (200, 200, 200)  # RGB para color plomo
    # Color cuando una celda es clickeada 
    color_celda_bloqueadas = (255, 0, 0) # RGB para color #ff0000
    # Color celda de origen
    color_celda_origen = (4, 255, 0) # RGB para color #04ff00 
    # Color celda de destino
    color_celda_destino = (255, 234, 0) # RGB para color #ffea00
    # Número de filas y columnas en la grilla
    columnas = 20
    filas = 15
    
    #Diccionario para obtener de manera optimizada los colores de la celda
    colores_celdas = {
        color_celda_bloqueadas: color_celda_bloqueadas,
        color_celda_origen: color_celda_origen,
        color_celda_destino: color_celda_destino
    }   

    # Calcular tamaño de cada celda
    tamano_celda_ancho = ancho_pantalla // columnas
    tamano_celda_alto = alto_pantalla // filas
    
    # Inicializar una matriz para saber qué celdas han sido clickeadas
    celdas = [[color_celda for _ in range(columnas)] for _ in range(filas)]

    # Inicializar celda de origen
    celda_origen = None

    # Inicializar celda de destino
    celda_destino = None

    #Inicaliza variable de modo edición
    modoEdicion = False

    # Bucle principal
    while True:
        # Manejar eventos
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LCTRL or event.key == pygame.K_RCTRL:
                    modoEdicion = not modoEdicion

            elif event.type == pygame.MOUSEBUTTONDOWN:
                # Obtener la posición del mouse
                x_mouse, y_mouse = pygame.mouse.get_pos()
                
                # Calcular las coordenadas de la celda donde se hizo clic
                columna = x_mouse // tamano_celda_ancho # Redondea a entero //
                fila = y_mouse // tamano_celda_alto # Redondea a entero //
               
                # Cambiar el estado de la celda clickeada (activar o desactivar)
                print(event.button)
                if 0 <= columna < columnas and 0 <= fila < filas:
                    if modoEdicion: 
                        if event.button == 1:
                            celdas[fila][columna] = color_celda_bloqueadas
                        elif event.button == 3:
                            celdas[fila][columna] = color_celda
                    elif celda_origen == None:
                    # Obtener la posición del mouse
                        celda_origen = celdas[fila][columna]
                        celdas[fila][columna] = color_celda_origen
                    elif celda_destino == None:
                        celda_destino = celdas[fila][columna]
                        celdas[fila][columna] = color_celda_destino

        # Dibujar la grilla
        pantalla.fill((255, 255, 255))  # Fondo blanco
        for fila in range(filas):
            for columna in range(columnas):
                x = columna * tamano_celda_ancho
                y = fila * tamano_celda_alto

                color_fondo = colores_celdas.get(celdas[fila][columna], color_celda) 

                # Dibujar la celda (fondo)
                pygame.draw.rect(pantalla, color_fondo, (x, y, tamano_celda_ancho, tamano_celda_alto))

                # Dibujar el borde de la celda
                pygame.draw.rect(pantalla, color_borde, (x, y, tamano_celda_ancho, tamano_celda_alto), 1)  # '2' es el grosor del borde
        
        # Actualizar pantalla
        pygame.display.flip()

# Llamada a la función para crear la grilla
crear_grilla()
