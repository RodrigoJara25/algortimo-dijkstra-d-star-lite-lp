def dijkstra(nodos, vertice_origen, vertice_destino):
    # Busca el nodo de origen y el nodo de destino por posición
    origen = next((nodo for nodo in nodos if nodo.vertice == vertice_origen), None)
    destino = next((nodo for nodo in nodos if nodo.vertice == vertice_destino), None)

    # Verifica que el origen y el destino existen en la grilla de nodos
    if origen is None or destino is None:
        raise ValueError("Origen o destino no encontrados en la lista de nodos.")
    
    # Inicializamos el origen con distancia 0
    origen.distancia = 0
    unexplored = list(nodos)  # Copia de la lista de nodos sin explorar

    while unexplored:
        # Encontrar el nodo no explorado con la menor distancia
        v = min(unexplored, key=lambda nodo: nodo.distancia)
        unexplored.remove(v)  # Marcamos a v como explorado

        # Si llegamos al destino, terminamos
        if v == destino:
            break

        # Revisar cada adyacente del vertice
        for coordenada, peso in v.adyacentes.items():
            # Buscar el nodo correspondiente a la coordenada
            adyacente = next((nodo for nodo in nodos if nodo.vertice == coordenada), None)
            if adyacente is None:
                continue  # Si no se encuentra el nodo, continuar con el siguiente adyacente
            # Halla la nueva distancia
            nueva_distancia = v.distancia + peso
            # Compara si la nueva distancia es menor a la distancia actual
            if nueva_distancia < adyacente.distancia:
                # Si la distancia es menor, esta sera reemplazada
                adyacente.distancia = nueva_distancia
                adyacente.predecesor = v

    # Reconstruir el camino desde el destino al origen
    camino = []
    nodo_actual = destino
    while nodo_actual is not None:
        # Inserta al inicio de la lista el nombre del nodo actual
        camino.insert(0, nodo_actual.vertice)
        # Obtiene el predecesor para poder seguir construyendo el camino
        nodo_actual = nodo_actual.predecesor

    return camino, destino.distancia

