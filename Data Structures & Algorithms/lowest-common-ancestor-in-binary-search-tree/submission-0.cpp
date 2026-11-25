/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        int small = (p->val > q->val) ? q->val : p->val;
        int big = (p->val > q->val) ? p->val : q->val;
        TreeNode* res = root;
        while (small > res->val || res->val > big) {
            if (small <= res->val && big <= res->val) res = res->left;
            else res = res->right;
        }
        return res;
    }
};
