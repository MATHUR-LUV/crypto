#include<bits/stdc++.h>
using namespace std;

string encrypt(string sstr, int k){
    string s= sstr;
    for(int i=0;i<s.length();i++){
        s[i] = ((s[i] - 'a' + k )%26) + 'a';
    }
    return s;
}

void solve(string s){
    for(int i=1;i<=25;i++){
        string ns = encrypt(s,i);
        cout<<ns<<endl;
    }
}
int main(){
    string plaintext;
    int key;
    cin>>plaintext;
    cin>>key;

    string encryptedText = encrypt(plaintext, key);
    cout<<"encrypted text : "<<encryptedText<<endl;
    solve(encryptedText);

    return 0;
    
}