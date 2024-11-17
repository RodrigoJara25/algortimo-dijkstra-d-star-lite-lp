#ifndef NODODSTARTLITE_H
#define NODODSTARTLITE_H

#include <unordered_map>
#include <utility>
#include <limits>
#include <stdexcept>

class NodoDStartLite {
public:
    using Node = int; 
    using Key = std::pair<double, double>;

    NodoDStartLite();
    void insert(Node node, const Key& key);
    void update(Node node, const Key& key);
    void remove(Node node);
    Key topKey() const;
    Node top() const;
    bool contains(Node node) const;

private:
    std::unordered_map<Node, Key> q;

    static bool keyLessThan(const Key& a, const Key& b);
};

#endif // NODODSTARTLITE_H
