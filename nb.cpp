class Solution {
public:
    long long minSumSquareDiff(vector<int>& nums1, vector<int>& nums2, int k1, int k2) {
        int n=nums1.size();
        long long ans=0,sum=0;
        for(int i=0;i<n;i++){
            nums1[i]=fabs(nums1[i]-nums2[i]);
            ans+=(long long)nums1[i]*nums1[i];
            sum+=nums1[i];
        }
        int k=k1+k2;
        if(sum<=k) return 0; 
        sort(nums1.begin(),nums1.end(),greater());
        nums1.push_back(0);
        for(int i=0;i<n;i++){
            long long j=i+1;
            ans-=(long long)nums1[i]*nums1[i];
            long long c=(nums1[i]-nums1[j])*j;
            if(c<k){
                k-=c;
            }
            else{
                int div=k/j,mod=k%j;
                long long d=nums1[i]-div;
                ans+=d*d*(j-mod);
                ans+=(d-1)*(d-1)*mod;
                return ans;
            }
        }
        return ans;
    }
};