#include "NodoDijkstra.h"
#include <tuple>
#include <limits> 

NodoDijkstra::NodoDijkstra(std::tuple<int, int> coordenadas)
{
    this->coordenadas = coordenadas;
    distancia = std::numeric_limits<float>::infinity();  // distancia infinita
    adyacentes = {};
    predecesor = nullptr;
}

// Getters
float NodoDijkstra::getDistancia() const
{
    return distancia;
}

NodoDijkstra *NodoDijkstra::getPredecesor() const
{
    return predecesor;
}
std::tuple<int,int> NodoDijkstra::getCoordenadas() const
{
    return coordenadas;
}
std::map<std::tuple<int,int>, int> NodoDijkstra::getAdyacentes() const
{
    return adyacentes;
}
// Setters
void NodoDijkstra::setDistancia(float distancia)
{
    this->distancia = distancia;
}

void NodoDijkstra::setPredecesor(NodoDijkstra* predecesor)
{
    this->predecesor = predecesor;
}
void NodoDijkstra::setAdyacentes(std::map<std::tuple<int,int>, int> adyacentes)
{
    this->adyacentes = adyacentes;
}
void NodoDijkstra::setCoordenadas(std::tuple<int, int> coordenadas)
{
    this->coordenadas = coordenadas;
}

void NodoDijkstra::configurarAdyacente(std::tuple<int, int> coordenadas, float peso) {
    this->adyacentes[coordenadas] = peso;  
}