#include<bits/stdc++.h>
using namespace std;

int gcd(int a, int b){
    if(b==0){
        return a;
    }
    if(a==0){
        return b;
    }

    return gcd(b%a,a);
}
int main(){
    int a,b;
    cin>>a>>b;

    int ans = gcd(a,b);
    cout<<ans<<endl;
    return 0;
}