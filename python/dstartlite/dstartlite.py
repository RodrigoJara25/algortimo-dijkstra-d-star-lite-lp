import numpy as np
from typing import Callable, Hashable, Tuple
from collections import defaultdict
from TDA.nodo.nodoDstartlite import NodoDStartLite

class DStarLite:
    def __init__(self,
                 heuristic_fn: Callable[[Hashable, Hashable], float],
                 prev_fn: Callable[[Hashable], Tuple[Hashable, ...]],
                 next_fn: Callable[[Hashable], Tuple[Hashable, ...]],
                 cost_fn: Callable[[Hashable, Hashable], float]
                 ):
        self._h = heuristic_fn
        self._p = prev_fn
        self._n = next_fn
        self._c = cost_fn

        self._k_m = 0
        self._g = {}
        self._rhs = {}
        self._U = None

    def _key(self, ref: Hashable, node: Hashable):
        """Compute the lexicographical key by which nodes will be sorted."""
        g = self._g[node]
        rhs = self._rhs[node]
        k2 = min(g, rhs)
        k1 = k2 + self._h(ref, node) + self._k_m
        return (k1, k2)

    def _initialize(self, init: Hashable, goal: Hashable, blocked_nodes: set):
        k_m = 0
        inf = float('inf')
        rhs = defaultdict(lambda: inf)
        g = defaultdict(lambda: inf)
        rhs[goal] = 0.0
        U = NodoDStartLite()
        U.insert(goal, (self._h(init, goal), 0))

        # Reset state-specific variables.
        self._k_m = k_m
        self._rhs = rhs
        self._g = g
        self._U = U
        self._blocked_nodes = blocked_nodes  # Save blocked nodes

    def _update_vertex(self, ref: Hashable, u: Hashable):
        # NOTE(ycho): Unpack state variables bound to class instance.
        rhs, g, U = self._rhs, self._g, self._U
        if u in self._blocked_nodes:  # Skip blocked nodes
            return

        if g[u] != rhs[u]:
            key = self._key(ref, u)
            if u in U:
                U.update(u, key)
            else:
                U.insert(u, key)
        elif (u in U):
            U.remove(u)

    def _compute_shortest_path(self, ref: Hashable, goal: Hashable,
                               on_expand, blocked_nodes: set):
        rhs, g, U = self._rhs, self._g, self._U

        while (U.top_key() < self._key(ref, ref)) or (rhs[ref] != g[ref]):
            u = U.top()
            k_old = U.top_key()
            on_expand(u)
            k_new = self._key(ref, u)
            if u in blocked_nodes:  # Skip blocked nodes during expansion
                continue

            if k_old < k_new:
                U.update(u, k_new)
            elif g[u] > rhs[u]:
                g[u] = rhs[u]
                U.remove(u)

                for s in self._p(u):
                    if (s != goal) and (s not in blocked_nodes):  # Skip blocked nodes
                        rhs[s] = min(rhs[s], g[u] + self._c(s, u))
                    self._update_vertex(ref, s)  # update `s`
            else:
                g_old = g[u]
                g[u] = np.inf
                for s in [u] + self._p(u):
                    if (rhs[s] == self._c(s, u) + g_old and s != goal) and (s not in blocked_nodes):
                        rhs[s] = min([g[s1] + self._c(s, s1) for s1 in self._n(s) if s1 not in blocked_nodes])
                    self._update_vertex(ref, s)  # update `s`

    def plan(self, init: Hashable, goal: Hashable, on_expand, blocked_nodes: set):
        s_last = init  # s_last, L#29
        self._initialize(init, goal, blocked_nodes)

        try:
            self._compute_shortest_path(init, goal, on_expand, blocked_nodes)
        except ValueError:
            yield None
            return
        g = self._g

        s_start = init
        while s_start != goal:
            # Determine best next state.
            s_nexts = self._n(s_start)
            if len(s_nexts) <= 0:
                yield None  # No valid path found, return None
                return

            # Filter valid neighbors (not blocked)
            valid_neighbors = [s for s in s_nexts if s not in blocked_nodes]
            if not valid_neighbors:
                yield None  # No valid next state
                return

            costs = [self._c(s_start, s) + g[s] for s in valid_neighbors]
            s_start = valid_neighbors[np.argmin(costs)]

            # Option#1 : yield only the coordinates
            yield s_start  # Yield only the coordinates (not a tuple with empty dictionary)

            self._k_m += self._h(s_last, s_start)
            s_last = s_start