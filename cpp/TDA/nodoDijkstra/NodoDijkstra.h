#ifndef NODO_H  // Check if the class is already defined
#define NODO_H  // If not, define it

#include <iostream>
#include <tuple>
#include <map>

class NodoDijkstra {
private:
    std::tuple<int, int> coordenadas;  // Tupla de la coordenada
    float distancia;  // Distancia desde el nodo origen
    NodoDijkstra* predecesor;     // Puntero al nodo anterior en la ruta
    std::map<std::tuple<int,int>, int> adyacentes; // Diccionario sobre sus nodos adyacentes

public:
    // Constructor
    NodoDijkstra(std::tuple<int, int> coordenadas);

    // getters
    float getDistancia() const;
    NodoDijkstra* getPredecesor() const;
    std::tuple<int,int> getCoordenadas() const;
    std::map<std::tuple<int,int>, int> getAdyacentes() const;
    
    // setters
    void setDistancia(float distancia);
    void setPredecesor(NodoDijkstra* predecesor);
    void setCoordenadas(std::tuple<int, int> coordenadas);
    void setAdyacentes(std::map<std::tuple<int,int>, int> adyacentes);

    // Methods
    void configurarAdyacente(std::tuple<int, int> coordenadas, float peso);
};

#endif  // End of include guard
