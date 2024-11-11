#include <iostream>
#include <vector>
#include <tuple>
#include "Nodo.h"

std::pair<std::vector<std::tuple<int, int>>, float> dijkstra(std::vector<Nodo *> &nodos, std::tuple<int, int> vertice_origen, std::tuple<int, int> vertice_destino);