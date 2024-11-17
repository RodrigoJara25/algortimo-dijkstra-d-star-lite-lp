#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <cmath>
#include <queue>
#include <tuple>
#include <unordered_set>

using namespace std;

// Custom hash function for pair<int, int>
// This is no longer needed since we're using std::map which sorts keys.
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator ()(const std::pair<T1, T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        
        // Combine the two hash values
        return h1 ^ h2; // You can also use boost::hash_combine or other methods
    }
};

class DStarLite
{
public:
    DStarLite(
        function<float(pair<int, int>, pair<int, int>)> heuristic_fn,
        function<vector<pair<int, int>>(pair<int, int>)> prev_fn,
        function<vector<pair<int, int>>(pair<int, int>)> next_fn,
        function<float(pair<int, int>, pair<int, int>)> cost_fn);

    // Declaración de la función plan en el archivo .h
    std::vector<std::pair<int, int>> plan(std::pair<int, int> start, std::pair<int, int> goal,
                                          std::function<void(std::pair<int, int>)> on_expand,
                                          std::vector<std::pair<int, int>> &blocked_nodes);  // Usando vector aquí

private:
    function<float(pair<int, int>, pair<int, int>)> _h;
    function<vector<pair<int, int>>(pair<int, int>)> _p;
    function<vector<pair<int, int>>(pair<int, int>)> _n;
    function<float(pair<int, int>, pair<int, int>)> _c;

    float _k_m;
    // Cambié de unordered_map a map
    map<pair<int, int>, float> _g;  // Usando std::map, ahora no se necesita hash
    map<pair<int, int>, float> _rhs;  // Usando std::map también
    priority_queue<pair<float, pair<int, int>>, vector<pair<float, pair<int, int>>>, greater<pair<float, pair<int, int>>>> _U;

    void _update_vertex(pair<int, int> ref, pair<int, int> u);
    void _compute_shortest_path(pair<int, int> ref, pair<int, int> goal, function<void(pair<int, int>)> on_expand, std::vector<std::pair<int, int>> &blocked_nodes);  // Usando vector
    pair<float, float> _key(pair<int, int> ref, pair<int, int> node);
    void _initialize(pair<int, int> init, pair<int, int> goal, std::vector<std::pair<int, int>> &blocked_nodes);  // Usando vector
};
