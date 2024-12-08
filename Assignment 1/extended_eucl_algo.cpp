#include<bits/stdc++.h>
using namespace std;
class info{
    public:
        int x;
        int y;
        int gcd;
};

info solve(int a, int b){
    if(a==0){
        info res;
        res.x = 0;
        res.y = 1;
        res.gcd = b;
        
        return res;
    }
    
    info temp = solve(b%a,a);
    int x1 = temp.x;
    int y1 = temp.y;
    info ans;
    ans.gcd = temp.gcd;
    ans.y = x1;
    ans.x = y1 - (b/a)*x1;
    
    return ans;
}
int main(){
    int a,b;
    cin>>a>>b;
    
    info ans = solve(a,b);
    cout<<"x : "<<ans.x<<endl;
    cout<<"y : "<<ans.y<<endl;
    cout<<"gcd : "<<ans.gcd<<endl;
    
    
    return 0;
}