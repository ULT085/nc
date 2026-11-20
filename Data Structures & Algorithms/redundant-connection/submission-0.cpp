class DSU {
    vector<int> p;
    vector<int> rank;
public:
    DSU(int n) {
        p.resize(n + 1);
        iota(p.begin(), p.end(), 0);
        rank.assign(n + 1, 0);
    }

    bool unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) return false;
        if (rank[ra] > rank[rb]) swap(ra, rb);
        p[ra] = rb;
        if (rank[ra] == rank[rb]) rank[rb]++;
        return true;
    }

    int find(int n) {
        if (p[n] == n) return n;
        return p[n] = find(p[n]);
    }
};

class Solution {
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        int n = edges.size() + 1;
        DSU dsu(n);
        for (auto& e : edges) {
            if (!dsu.unite(e[0], e[1])) return e;
        }

        return {};
    }
};
