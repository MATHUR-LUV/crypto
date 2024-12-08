#include <bits/stdc++.h>
#include <random>
using namespace std;

int randomGenerator(int num) {
    random_device device;
    mt19937 generator(device());
    uniform_int_distribution<int> distribution(0, 255); 
    return distribution(generator);
}

string binary(int num) {
    string ans = "";
    while (num != 0) {
        int bit = num & 1;
        num >>= 1;
        ans += (bit + '0');
    }
    while (ans.length() < 8) {  
        ans += '0';
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

void convertASCII(const string& s, vector<string>& ascii) {
    for (char c : s) {
        int asc = static_cast<int>(c);
        string bin = binary(asc);
        ascii.push_back(bin);
    }
}

string streamCipherEncryption(const string& s, vector<int>& key1_list, vector<int>& key2_list) {
    string cipherText = "";
    for (char c : s) {
        int num = static_cast<int>(c);
       
        int key1 = randomGenerator(num); 
        key1_list.push_back(key1);      
        num ^= key1;  

        int key2 = randomGenerator(num); 
        key2_list.push_back(key2);       
        num ^= key2;  

        string numStr = binary(num); 
        cipherText += numStr + " ";
    }
    return cipherText;
}

string knownPlaintextAttack(const string& cipherText, const vector<int>& key1_list, const vector<int>& key2_list) {
    istringstream ss(cipherText);
    string binarySegment;
    string recoveredText = "";
   
    int index = 0;
    while (ss >> binarySegment) {
        int encryptedNum = stoi(binarySegment, nullptr, 2);  
       
        int key2 = key2_list[index]; 
        encryptedNum ^= key2;  
       
        int key1 = key1_list[index]; 
        encryptedNum ^= key1;  
       
        recoveredText += static_cast<char>(encryptedNum);
        index++;
    }

    return recoveredText;
}

int main() {
    string s;
    cin >> s;

    vector<int> key1_list, key2_list;


    vector<string> ascii;
    convertASCII(s, ascii);

  
    string encryptedText = streamCipherEncryption(s, key1_list, key2_list);
    cout << "Encrypted Binary Text: " << encryptedText << endl;


    string recoveredText = knownPlaintextAttack(encryptedText, key1_list, key2_list);

    cout << "Recovered Text : " << recoveredText << endl;

    return 0;
}