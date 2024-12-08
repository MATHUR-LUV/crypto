#include<bits/stdc++.h>
#include<random>
using namespace std;

random_device device;
mt19937 generator(device());

int randomGen(){
    uniform_int_distribution<int> distribution(0, 25);
    return distribution(generator);
}

string randomKey(int len){
    string key = "";
    for(int i = 0; i < len; i++){
        key += (char)(randomGen() + 'a');
    }
    return key;
}

string encrypt(string &plaintext, string &key){
    string ans = "";
    for(int i = 0; i < plaintext.length(); i++){
        int val = (plaintext[i] - 'a') + (key[i] - 'a');
        ans += (val % 26) + 'a';
    }
    return ans;
}

string decrypt(string &ciphertext, string &key){
    string ans = "";
    for(int i = 0; i < ciphertext.length(); i++){
        int val = (ciphertext[i] - 'a') - (key[i] - 'a');
        if(val < 0){
            val += 26;
        }
        ans += (val % 26) + 'a';
    }
    return ans;
}

int main(){
    string plaintext;
    cin >> plaintext;

    for(int i = 0; i < plaintext.size(); i++){
        plaintext[i] = tolower(plaintext[i]);
    }

    string key = randomKey(plaintext.length());

    cout << "Key: " << key << endl;
    string encryptedKey = encrypt(plaintext, key);
    cout << "Encrypted String: " << encryptedKey << endl;

    string decryptedKey = decrypt(encryptedKey, key);
    cout << "Decrypted String: " << decryptedKey << endl;

    return 0;
}
