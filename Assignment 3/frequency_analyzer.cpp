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

void frequencyAnalyzer(string cipher)
{
    map<char, int> freq;
    char mostFreq = 'e';
    char lastFreq = 'e';
    for (auto x : cipher)
    {
        freq[x]++;
        if (freq[x] > freq[mostFreq])
        {
            lastFreq = mostFreq;
            mostFreq = x;
        }
    }
    int key = 26 + mostFreq - 'e';
    cout << "key 1: " << key % 26 << endl;
    cout << "" << encrypt(cipher, 26 - key % 26) << endl;
    key = 26 + lastFreq - 'e';
    cout << "key 2: " << key % 26 << endl;
    cout << "" << encrypt(cipher, 26 - key % 26) << endl;
}

int main()
{
    int key;
    cin >> key;
    string s;
    cin >> s;

    string encryptedCipher = encrypt(s, key);
    cout << "encryptedCipher : " << encryptedCipher << endl;
    frequencyAnalyzer(encryptedCipher);
    return 0;
}