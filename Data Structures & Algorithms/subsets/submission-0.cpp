class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> path;
        traversal(nums, -1, path, res);
        return res;
    }

    void traversal(vector<int> nums, int idx, vector<int>& path, vector<vector<int>>& res) {
        res.push_back(path);
        for (int i = idx + 1; i < nums.size(); ++i) {
            path.push_back(nums[i]);
            traversal(nums, i, path, res);
            path.pop_back();
        }
    }
};
