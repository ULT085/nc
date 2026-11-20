class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        int n1 = text1.size();
        int n2 = text2.size();
        if (n1 < n2) {
            swap(n1, n2);
            swap(text1, text2);
        }
        vector<int> dp1(n2 + 1, 0);
        vector<int> dp2(n2 + 1, 0);
        // if (text1[i] == text[j]) dp[i][j] = dp[i - 1][j - 1] + 1
        // else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])
        for (int i = 1; i <= n1; ++i) {
            for (int j = 1; j <= n2; ++j) {
                if (text1[i - 1] == text2[j - 1]) {
                    dp2[j] = dp1[j - 1] + 1;
                } else {
                    dp2[j] = max(dp1[j], dp2[j - 1]);
                }
            }
            swap(dp1, dp2);
        }

        return dp1[n2];
    }
};