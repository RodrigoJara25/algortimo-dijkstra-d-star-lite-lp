from typing import Hashable, Tuple

class NodoDStartLite:
    def __init__(self):
        self.q = {}

    def insert(self, node: Hashable, key: Tuple[float, float]):
        self.q[node] = key

    def update(self, node: Hashable, key: Tuple[float, float]):
        self.q[node] = key

    def remove(self, node: Hashable):
        self.q.pop(node)

    def top_key(self):
        return min(self.q.values())

    def top(self):
        node, key = min(self.q.items(), key=lambda e: e[1])
        return node

    def __contains__(self, node):
        return node in self.q

