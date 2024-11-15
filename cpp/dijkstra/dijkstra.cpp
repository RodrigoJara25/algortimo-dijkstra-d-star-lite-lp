#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <stdexcept>
#include "../TDA/nodo/Nodo.h"
#include "dijkstra.h"

std::pair<std::vector<std::tuple<int, int>>, float> dijkstra(std::vector<Nodo *> &nodos, std::tuple<int, int> vertice_origen, std::tuple<int, int> vertice_destino)
{
    Nodo *origen = nullptr;
    Nodo *destino = nullptr;
    std::cout << "FAdadaDA";
    for (auto &nodo : nodos)
    {

       std::cout << std::get<0>(nodo->getCoordenadas()) << std::get<1>(nodo->getCoordenadas()) << '\n';
        if (nodo->getCoordenadas() == vertice_origen)
        {
            origen = nodo;
        }
        if (nodo->getCoordenadas() == vertice_destino)
        {
            destino = nodo;
        }
    }
    std::cout << "Esta vaina va a acabar";
    if (!origen || !destino)
    {
        std::cout << "efectivamente";
        throw std::invalid_argument("Origen o destino no encontrados en la lista de nodos.");
    }

    origen->setDistancia(0);

    std::vector<Nodo *> unexplored = nodos;

    while (!unexplored.empty())
    {
        auto min_it = std::min_element(unexplored.begin(), unexplored.end(),
                                       [](Nodo *a, Nodo *b)
                                       { return a->getDistancia() < b->getDistancia(); });
        Nodo *v = *min_it;
        // std::cout << "Vertice usando : " << std::get<0>(v->getCoordenadas()) << " " << std::get<1>(v->getCoordenadas()) << '\n';

        // std::cout << "Adyacentes: " << '\n';
        // auto adyacentes = v->getAdyacentes();
        // for (const auto &adyacente : adyacentes)
        // {
        //     const auto &[key, value] = adyacente; // Desempaqueta el std::pair
        //     const auto &[y, x] = key;             // Desempaqueta el std::tuple<int, int>
        //     std::cout << "(" << x << ", " << y << ") -> " << value << "  " << "\n";
        // }
        unexplored.erase(min_it);

        if (v == destino)
        {
            break;
        }

        for (const auto &[coordenadas, peso] : v->getAdyacentes())
        {

            Nodo *adyacente = nullptr;
            for (auto &nodo : nodos)
            {
                if (nodo->getCoordenadas() == coordenadas)
                {
                    adyacente = nodo;
                    break;
                }
            }

            if (!adyacente)
                continue;

            float nueva_distancia = v->getDistancia() + peso;
            if (nueva_distancia < adyacente->getDistancia())
            {
                adyacente->setDistancia(nueva_distancia);
                adyacente->setPredecesor(v);
            }
        }
    }
    

    std::vector<std::tuple<int, int>> camino;
    Nodo *nodo_actual = destino;
    while (nodo_actual != nullptr)
    {

        auto coordenadas = nodo_actual->getCoordenadas();

        camino.insert(camino.begin(), coordenadas);
        nodo_actual = nodo_actual->getPredecesor();
        // if (nodo_actual != nullptr)
        // {
        //     std::cout << "HYPERMEGA GA" << std::get<0>(nodo_actual->getCoordenadas()) << std::get<1>(nodo_actual->getCoordenadas()) << '\n';
        // }
    }

    return {camino, destino->getDistancia()};
}