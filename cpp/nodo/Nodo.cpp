#include "Nodo.h"
#include <limits> 

Nodo::Nodo(int x, int y)
{
    this->x = x;
    this->y = y;
    distancia_origen = std::numeric_limits<float>::infinity();  // distancia infinita
    nodo_anterior = nullptr;
}

int Nodo::getX() const
{
    return x;
}

int Nodo::getY() const
{
    return y;
}

float Nodo::getDistancia() const
{
    return distancia_origen;
}

Nodo *Nodo::getNodoAnterior() const
{
    return nodo_anterior;
}

void Nodo::setDistancia(float distancia)
{
    this->distancia_origen = distancia;
}

void Nodo::setNodoAnterior(Nodo *anterior)
{
    this->nodo_anterior = anterior;
}
