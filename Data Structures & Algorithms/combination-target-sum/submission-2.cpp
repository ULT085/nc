class Solution {
public:
    vector<vector<int>> res;
    vector<vector<int>> combinationSum(vector<int>& nums, int target) {
        vector<int> path;
        sort(nums.begin(), nums.end()); // ascending order
        dfs(nums, target, 0, path);
        return res;
    }

    void dfs(vector<int>& nums, int target, int idx, vector<int>& path) {
        if (target == 0) {
            res.push_back(path);
            return;
        }

        for (int i = idx; i < nums.size() && nums[i] <= target; ++i) {
            path.push_back(nums[i]);
            dfs(nums, target - nums[i], i, path);
            path.pop_back();
        }
    }
};
