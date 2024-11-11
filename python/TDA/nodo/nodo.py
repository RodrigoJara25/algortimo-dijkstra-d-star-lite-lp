class Nodo:
    def __init__(self, vertice):
        self.vertice = vertice # Vertice
        self.adyacentes = {}  # Diccionario de vecinos y sus pesos
        self.distancia = float('inf')  # Inicia con distancia infinita
        self.predecesor = None  # Nodo previo en el camino más corto

    def configurarAdyacente(self, adyacente, peso):
        self.adyacentes[adyacente] = peso

    def __repr__(self):
        # Obtener datos sobre el nodo
        return f"Nodo({self.vertice}, distancia={self.distancia}, predecesor={self.predecesor})"
