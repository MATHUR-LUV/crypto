#include<iostream>
#include<vector>
using namespace std;

class info{
    public:
        int x;
        int y;
        int gcd;
};

class Solution{
public:
    info solve(int a, int b){
        if(b==0){
            int x = 1;
            int y = 0;
            int gcd = a;
            
            info ans;
            ans.x = x;
            ans.y = y;
            ans.gcd = gcd;
            return ans;
        }
        
        info temp = solve(b, a%b);
        int x1 = temp.x;
        int y1 = temp.y;
        info res;
        res.gcd = temp.gcd;
        res.x = y1;
        res.y = (x1 - (a/b)*y1);
        
        return res;
    }
    vector<int> gcd(int a, int b){
        
        info ans = solve(a,b);
        return {ans.gcd, ans.x, ans.y};
        
    }
};

int main(){
    int a, b;
    cin >> a >> b;
    Solution sol;
    vector<int> ans = sol.gcd(a,b);
    cout<<"gcd : "<<ans[0]<<endl;
    cout<<"x: "<<ans[1]<<endl;
    cout<<"y: "<<ans[2]<<endl;

    return 0;
}