class Nodo:
    def __init__(self, nodo_anterior=None, nodo_siguiente=None):
        self.nodo_anterior = nodo_anterior
        self.nodo_siguiente = nodo_siguiente
    
    def obtenerSiguiente(self):
        return self.nodo_siguiente
    
    def establecerSiguiente(self, nodo_siguiente):
        self.nodo_siguiente = nodo_siguiente
    
    def obtenerAnterior(self):
        return self.nodo_anterior
    
    def establecerAnterior(self, nodo_anterior):
        self.nodo_anterior = nodo_anterior
