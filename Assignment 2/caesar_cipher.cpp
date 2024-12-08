#include<bits/stdc++.h>
using namespace std;


string Cipher(string s, int k)
{
    for (int i = 0; i < s.size(); i++)
    {
        s[i] = ((s[i] - 'a' + k) % 26) + 'a';
    }
    return s;
}

int main(){
    int key;
    cin>>key;
    string s;
    cin>>s;

    string cipherText = Cipher(s,key);
    cout<<"Caesar Cipher : "<<cipherText<<endl;

    return 0;
}