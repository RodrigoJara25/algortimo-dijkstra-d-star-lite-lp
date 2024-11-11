#include "Nodo.h"
#include <tuple>
#include <limits> 

Nodo::Nodo(std::tuple<int, int> coordenadas)
{
    this->coordenadas = coordenadas;
    distancia = std::numeric_limits<float>::infinity();  // distancia infinita
    adyacentes = {};
    predecesor = nullptr;
}

// Getters
float Nodo::getDistancia() const
{
    return distancia;
}

Nodo *Nodo::getPredecesor() const
{
    return predecesor;
}
std::tuple<int,int> Nodo::getCoordenadas() const
{
    return coordenadas;
}
std::map<std::tuple<int,int>, int> Nodo::getAdyacentes() const
{
    return adyacentes;
}
// Setters
void Nodo::setDistancia(float distancia)
{
    this->distancia = distancia;
}

void Nodo::setPredecesor(Nodo* predecesor)
{
    this->predecesor = predecesor;
}
void Nodo::setAdyacentes(std::map<std::tuple<int,int>, int> adyacentes)
{
    this->adyacentes = adyacentes;
}
void Nodo::setCoordenadas(std::tuple<int, int> coordenadas)
{
    this->coordenadas = coordenadas;
}

void Nodo::configurarAdyacente(std::tuple<int, int> coordenadas, float peso) {
    this->adyacentes[coordenadas] = peso;  
}