//二分法+贪心
class Solution {
public:
    long long minSumSquareDiff(vector<int>& nums1, vector<int>& nums2, int k1, int k2) {
        int n = nums1.size();
        long long K = k1 + k2;
        vector<int> d(n);
        long long total = 0;
        int maxD = 0;
        for (int i = 0; i < n; i++) {
            d[i] = abs(nums1[i] - nums2[i]);
            total += d[i];
            maxD = max(maxD, d[i]);
        }
        if (total <= K) return 0;

        // 二分目标阈值 X
        int l = 0, r = maxD;
        while (l < r) {
            int mid = (l + r) / 2;
            long long need = 0;
            for (int x : d) {
                if (x > mid) need += x - mid;
            }
            if (need <= K) {
                r = mid;
            } else {
                l = mid + 1;
            }
        }
        int X = l;

        // 计算把所有大于 X 的降到 X 需要的操作数
        long long need = 0;
        for (int x : d) {
            if (x > X) need += x - X;
        }
        long long rem = K - need;

        // 统计压完后等于 X 的元素个数：所有原本 >= X 的元素
        int cntX = 0;
        long long res = 0;
        for (int x : d) {
            if (x < X) {
                res += 1LL * x * x;
            } else {
                cntX++;  // x >= X，压完后都会变成 X
            }
        }
        long long use = min(rem, (long long)cntX);
        res += use * 1LL * (X - 1) * (X - 1);
        res += (cntX - use) * 1LL * X * X;

        return res;
    }
};