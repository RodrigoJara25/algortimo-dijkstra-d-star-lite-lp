#include "DStarLite.h"
#include <algorithm>  // Para std::find

DStarLite::DStarLite(
    function<float(pair<int, int>, pair<int, int>)> heuristic_fn,
    function<vector<pair<int, int>>(pair<int, int>)> prev_fn,
    function<vector<pair<int, int>>(pair<int, int>)> next_fn,
    function<float(pair<int, int>, pair<int, int>)> cost_fn)
    : _h(heuristic_fn), _p(prev_fn), _n(next_fn), _c(cost_fn), _k_m(0) {}

pair<float, float> DStarLite::_key(pair<int, int> ref, pair<int, int> node)
{
    float g = _g[node];
    float rhs = _rhs[node];
    float k2 = min(g, rhs);
    float k1 = k2 + _h(ref, node) + _k_m;
    return {k1, k2};
}

void DStarLite::_initialize(pair<int, int> init, pair<int, int> goal, vector<pair<int, int>>& blocked_nodes)
{
    _k_m = 0;
    _rhs.clear();
    _g.clear();
    _rhs[goal] = 0.0;

    // Reset U as a priority queue
    while (!_U.empty())
    {
        _U.pop();
    }

    _U.push({_h(init, goal), goal});
}

void DStarLite::_update_vertex(pair<int, int> ref, pair<int, int> u)
{
    if (_g[u] != _rhs[u])
    {
        auto key = _key(ref, u);
        _U.push({key.first, u});
    }
    else if (_U.top().second == u)
    {
        // Remove from priority queue if necessary (implement removal logic here)
    }
}

void DStarLite::_compute_shortest_path(pair<int, int> ref, pair<int, int> goal, function<void(pair<int, int>)> on_expand, vector<pair<int, int>>& blocked_nodes)
{
    while (_U.top().first < _key(ref, ref).first || _rhs[ref] != _g[ref])
    {
        pair<int, int> u = _U.top().second;
        _U.pop();
        on_expand(u);

        // Verificar si el nodo está bloqueado usando std::find
        if (std::find(blocked_nodes.begin(), blocked_nodes.end(), u) != blocked_nodes.end())
            continue;

        auto k_new = _key(ref, u);
        if (_g[u] > _rhs[u])
        {
            _g[u] = _rhs[u];
            _U.push({k_new.first, u});
        }
        else
        {
            float g_old = _g[u];
            _g[u] = INFINITY;
            for (auto &s : _p(u))
            {
                if (_rhs[s] == _c(s, u) + g_old)
                {
                    _rhs[s] = min({_g[s] + _c(s, s)});
                }
                _update_vertex(ref, s);
            }
        }
    }
}

std::vector<std::pair<int, int>> DStarLite::plan(std::pair<int, int> start, std::pair<int, int> goal,
                                                 std::function<void(std::pair<int, int>)> on_expand,
                                                 std::vector<std::pair<int, int>> &blocked_nodes) {
    // Inicializar el vector de camino
    std::vector<std::pair<int, int>> path;

    // Variables de inicio y meta
    std::pair<int, int> s_start = start;
    std::pair<int, int> s_goal = goal;

    // Verificar si el punto de inicio es igual al de destino (sin necesidad de planificar)
    if (s_start == s_goal) {
        path.push_back(s_start);
        return path;
    }

    // Lógica de expansión y planificación (simplificada para ejemplo)
    while (s_start != s_goal) {
        // Definir los vecinos de la celda actual (arriba, abajo, izquierda, derecha)
        std::vector<std::pair<int, int>> neighbors = {
            {s_start.first - 1, s_start.second},  // Arriba
            {s_start.first + 1, s_start.second},  // Abajo
            {s_start.first, s_start.second - 1},  // Izquierda
            {s_start.first, s_start.second + 1}   // Derecha
        };

        // Filtrar los nodos bloqueados y agregar los válidos al camino
        for (auto& neighbor : neighbors) {
            bool is_blocked = false;
            for (const auto& blocked : blocked_nodes) {
                if (blocked == neighbor) {
                    is_blocked = true;
                    break;
                }
            }

            if (!is_blocked) {
                // Si el nodo no está bloqueado, agrégalo al camino
                path.push_back(neighbor);
                on_expand(neighbor);  // Llamada a la función de expansión
            }
        }

        // Actualiza la celda de inicio a la siguiente celda en el camino
        // Este es un ejemplo simple, puedes cambiar la lógica aquí según el algoritmo utilizado
        s_start = path.back();  // Este es un ejemplo, puedes cambiar la lógica aquí según el algoritmo utilizado.

        // Si no se puede avanzar o ya llegamos al destino, termina la planificación
        if (path.empty() || s_start == s_goal) {
            break;
        }
    }

    return path;  // Devuelve el camino encontrado
}