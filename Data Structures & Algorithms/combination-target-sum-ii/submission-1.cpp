class Solution {
public:
    vector<vector<int>> res;
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        res.clear();
        sort(candidates.begin(), candidates.end()); // sort by ascending order
        vector<int> path;
        dfs(path, 0, target, candidates);
        return res;
    }

    void dfs(vector<int>& path, int idx, int target, vector<int>& candidates) {
        if (target == 0 || idx == candidates.size()) {
            if (target == 0) res.push_back(path);
            return;
        }

        for (int i = idx; i < candidates.size() && candidates[i] <= target; ++i) {
            if (i != idx && candidates[i] == candidates[i - 1]) continue;
            path.push_back(candidates[i]);
            dfs(path, i + 1, target - candidates[i], candidates);
            path.pop_back();
        }
    }
};
