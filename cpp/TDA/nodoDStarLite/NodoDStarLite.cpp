#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <limits>
#include <functional>

using namespace std;

class NodoDStarLite {
public:
    NodoDStarLite() {}

    void insert(const pair<int, int>& node, pair<float, float> key) {
        q[node] = key;
    }

    void update(const pair<int, int>& node, pair<float, float> key) {
        q[node] = key;
    }

    void remove(const pair<int, int>& node) {
        q.erase(node);
    }

    pair<float, float> top_key() {
        pair<float, float> min_key = {numeric_limits<float>::infinity(), numeric_limits<float>::infinity()};
        for (const auto& item : q) {
            min_key = min(min_key, item.second);
        }
        return min_key;
    }

    pair<int, int> top() {
        pair<int, int> min_node = {0, 0};
        pair<float, float> min_key = {numeric_limits<float>::infinity(), numeric_limits<float>::infinity()};
        for (const auto& item : q) {
            if (item.second < min_key) {
                min_key = item.second;                                                                                                                          
                min_node = item.first;
            }
        }
        return min_node;
    }

    bool contains(const pair<int, int>& node) {
        return q.find(node) != q.end();
    }

private:
    map<pair<int, int>, pair<float, float>> q;
};
