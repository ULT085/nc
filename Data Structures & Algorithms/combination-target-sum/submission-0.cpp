class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& nums, int target) {
        vector<vector<int>> res;
        vector<int> path;
        sort(nums.begin(), nums.end()); // ascending order
        dfs(nums, target, 0, path, res);
        return res;
    }

    void dfs(vector<int>& nums, int target, int idx, vector<int>& path, vector<vector<int>>& res) {
        if (target == 0) {
            res.push_back(path);
            return;
        }

        for (int i = idx; i < nums.size() && target >= nums[i]; ++i) {
            path.push_back(nums[i]);
            dfs(nums, target - nums[i], i, path, res);
            path.pop_back();
        }
    }
};
