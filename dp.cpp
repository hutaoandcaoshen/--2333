2333. 最小差值平方和 —— 个人 DP 解法记录
一、我的原始 DP 思路
定义状态：

text
dp[i][j] = 前 i+1 个元素，恰好使用 j 次操作时，平方差之和的最小值
其中 res[i] = abs(nums1[i] - nums2[i])，K = k1 + k2 为总操作次数。

转移：

text
dp[i][j] = min_{0 <= m <= j} ( (res[i] - m)^2 + dp[i-1][j-m] )
含义：第 i 个元素分到 m 次操作，剩余 j-m 次给前 i 个元素。

二、我踩过的坑（错误总结）
1. dp 第二维大小少 1
❌ 错误写法：

cpp
vector<vector<long long>> dp(nums1.size(), vector<long long>(k1+k2, 0));
return dp[nums1.size()-1][k1+k2-1];
操作次数可以取 0,1,...,K，共 K+1 种，所以第二维必须是 K+1。

✅ 正确写法：

cpp
vector<vector<long long>> dp(n, vector<long long>(K + 1, INF));
return dp[n-1][K];
2. 初始化时把「行」和「列」混用
❌ 错误写法：

cpp
for (int i = 0; i < k1+k2; i++) {
    dp[0][i] = fastPow(res[0] - i, 2);   // 第 0 行
    dp[i][0] = dp[i-1][0] + chenfan[i];  // 第 i 行第 0 列
}
同一个 i 一会儿当列索引，一会儿当行索引，而且当 K > n 时会越界。

✅ 正确写法：分开初始化。

cpp
// 第 0 个元素，用 j 次操作
for (int j = 0; j <= K; j++) {
    int newD = max(res[0] - j, 0);
    dp[0][j] = 1LL * newD * newD;
}

// 前 i+1 个元素都不操作 —— 前缀和
for (int i = 1; i < n; i++) {
    dp[i][0] = dp[i-1][0] + chenfan[i];
}
3. dp[i][0] 忘记写前缀和
❌ 错误写法：

cpp
for (int i = 0; i < n; i++) {
    dp[i][0] = chenfan[i];   // 只算了当前元素
}
✅ 正确写法：

cpp
dp[0][0] = chenfan[0];
for (int i = 1; i < n; i++) {
    dp[i][0] = dp[i-1][0] + chenfan[i];
}
4. 差值可以减到负数（最隐蔽的坑）
❌ 错误写法：

cpp
dp[0][i] = fastPow(res[0] - i, 2);
// 以及转移
long long now = fastPow(res[i] - m, 2) + dp[i-1][j-m];
当操作次数多于差值本身时，真实情况是差值已经变成 0，多余操作只能浪费掉。
但 (res[i] - m)^2 在 m > res[i] 时会算成负数平方，结果偏大。

✅ 正确写法：先取 max(..., 0)。

cpp
int newD = max(res[0] - i, 0);
dp[0][i] = 1LL * newD * newD;

// 转移
int newD = max(res[i] - m, 0);
long long now = 1LL * newD * newD + dp[i-1][j-m];
5. 转移里一直用 res[0] 而不是 res[i]
❌ 错误写法：

cpp
long long now = fastPow(res[0] - m, 2) + dp[i-1][j-m];
当前处理的是第 i 个元素，应该用 res[i]。

✅ 正确写法：

cpp
int newD = max(res[i] - m, 0);
long long now = 1LL * newD * newD + dp[i-1][j-m];
6. 用 LLONG_MAX 做初始值，加法会溢出
❌ 错误写法：

cpp
vector<vector<long long>> dp(n, vector<long long>(K+1, LLONG_MAX));
long long mins = LLONG_MAX;
LLONG_MAX + 正数 会溢出，产生 UB。

✅ 正确写法：用安全无穷大。

cpp
const long long INF = 1e18;
vector<vector<long long>> dp(n, vector<long long>(K+1, INF));
long long mins = INF;
7. j 循环漏掉 j = K
❌ 错误写法：

cpp
for (int j = 1; j < k1 + k2; j++)   // 最大只到 K-1
✅ 正确写法：

cpp
for (int j = 1; j <= K; j++)
8. chenfan 写成成员变量，多次调用会累积
❌ 错误写法：

cpp
class Solution {
    vector<long long> chenfan;   // 成员变量
public:
    ...
};
同一对象被多次调用时会越积越多。

✅ 正确写法：改成局部变量。

cpp
vector<long long> chenfan;







class Solution {
public:
    long long fastPow(long long a, long long b) {
        long long res = 1;
        while (b > 0) {
            if (b & 1) res *= a;
            a *= a;
            b >>= 1;
        }
        return res;
    }

    long long minSumSquareDiff(vector<int>& nums1, vector<int>& nums2, int k1, int k2) {
        int n = nums1.size();
        int K = k1 + k2;
        const long long INF = 1e18;

        // 特判：无操作
        if (K == 0) {
            long long arr = 0;
            for (int i = 0; i < n; i++) {
                arr += fastPow(nums1[i] - nums2[i], 2);
            }
            return arr;
        }

        // 预处理差值及其平方
        vector<int> res(n);
        vector<long long> chenfan(n);
        for (int i = 0; i < n; i++) {
            res[i] = abs(nums1[i] - nums2[i]);
            chenfan[i] = fastPow(res[i], 2);
        }

        // dp[i][j] = 前 i+1 个元素，恰好使用 j 次操作的最小平方和
        vector<vector<long long>> dp(n, vector<long long>(K + 1, INF));

        // 第 0 个元素，用 j 次操作
        for (int j = 0; j <= K; j++) {
            int newD = max(res[0] - j, 0);
            dp[0][j] = fastPow(newD, 2);
        }

        // 前 i+1 个元素，0 次操作 —— 前缀和
        for (int i = 1; i < n; i++) {
            dp[i][0] = chenfan[i] + dp[i - 1][0];
        }

        // 转移
        for (int i = 1; i < n; i++) {
            for (int j = 1; j <= K; j++) {
                long long mins = INF;
                for (int m = 0; m <= j; m++) {
                    int newD = max(res[i] - m, 0);
                    long long now = fastPow(newD, 2) + dp[i - 1][j - m];
                    mins = min(mins, now);
                }
                dp[i][j] = mins;
            }
        }

        return dp[n - 1][K];
    }
};