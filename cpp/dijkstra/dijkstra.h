#include <iostream>
#include <vector>
#include <tuple>
#include "../TDA/nodoDijkstra/NodoDijkstra.h"



std::pair<std::vector<std::tuple<int, int>>, float> dijkstra(std::vector<NodoDijkstra *> &nodos, std::tuple<int, int> vertice_origen, std::tuple<int, int> vertice_destino);