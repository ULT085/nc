class Solution {
public:
    bool helper(string& s1, string& s2) {
        int n1 = s1.size();
        int n2 = s2.size();
        if (n1 != n2) return false;
        int diff = 0;
        for (int i = 0; i < n1; ++i) {
            if (s1[i] != s2[i]) diff++;
            if (diff > 1) break;
        }
        return (diff == 1);
    }

    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        // viewed as a shortest path problem
        // node: word, edge: valid transform, source: beginWord, target: endWord, we want distance.
        // Node Index:
        //  0: beginWord;
        //  1~n: word in wordList
        auto it = find(wordList.begin(), wordList.end(), endWord);
        if (it == wordList.end() || beginWord == endWord) return 0;
        int targetIdx = it - wordList.begin() + 1;
        int n = wordList.size();
        vector<vector<int>> g(n + 1);
        vector<bool> visited(n + 1, false);
        for (int i = 0; i <= n; ++i) {
            string s1 = (i == 0) ? beginWord : wordList[i - 1];
            for (int j = i + 1; j <= n; ++j) {
                string s2 = wordList[j - 1];
                if (helper(s1, s2)) {
                    g[i].push_back(j);
                    g[j].push_back(i);
                }
            }
        }

        // do bfs (as unit edge weight)
        bool found = false;
        int dis = 0;
        queue<int> q;
        q.push(0);
        visited[0] = true;
        while (!q.empty() && !found) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                int u = q.front();
                q.pop();
                for (int& v: g[u]) {
                    if (!visited[v]) {
                        q.push(v);
                        visited[v] = true;
                    }
                    if (targetIdx == v) {
                        found = true;
                        break;
                    }
                }
                if (found) break;
            }
            dis++;
        }

        return found ? dis + 1 : 0;
    }
};
