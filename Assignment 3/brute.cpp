#include <bits/stdc++.h>
using namespace std;

string encrypt(string s, int k)
{
    for (int i = 0; i < s.size(); i++)
    {
        s[i] = ((s[i] - 'a' + k) % 26) + 'a';
    }
    return s;
}

void bruteforce(string cipher)
{
    for (int i = 1; i < 26; i++)
    {
        string temp = encrypt(cipher, i);
        cout << temp << " for key = " << 26 - i << endl;
    }
}

int main()
{

    int key;
    cin >> key;
    
    string s;
    cin >> s;

    string newstr = encrypt(s, key);
    bruteforce(newstr);

    return 0;
}