#include <bits/stdc++.h>
using namespace std;

char extra_char = 'z';

void toLowerCase(string& str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void removeSpaces(string& str) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
}

void generateKeyTable(const string& key, vector<vector<char>>& keyTable) {
    vector<bool> used(26, false);
    int index = 0;

    for (char ch : key) {
        if (ch == 'j') ch = 'i';  
        if (isalpha(ch) && !used[ch - 'a']) {
            used[ch - 'a'] = true;
            keyTable[index / 5][index % 5] = ch;
            ++index;
        }
    }

    for (char ch = 'a'; ch <= 'z'; ++ch) {
        if (ch == 'j') continue;  
        if (!used[ch - 'a']) {
            keyTable[index / 5][index % 5] = ch;
            ++index;
        }
    }
}

void search(const vector<vector<char>>& keyTable, char a, char b, vector<int>& positions) {
    if (a == 'j') a = 'i';
    if (b == 'j') b = 'i';

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (keyTable[i][j] == a) {
                positions[0] = i;
                positions[1] = j;
            }
            if (keyTable[i][j] == b) {
                positions[2] = i;
                positions[3] = j;
            }
        }
    }
}

void prepare(string& str) {
    if (str.length() % 2 != 0) {
        str += extra_char;
    }
}

void encrypt(string& str, const vector<vector<char>>& keyTable) {
    vector<int> positions(4);
    for (int i = 0; i < str.length(); i += 2) {
        search(keyTable, str[i], str[i + 1], positions);
        if (positions[0] == positions[2]) {
            str[i] = keyTable[positions[0]][(positions[1] + 1) % 5];
            str[i + 1] = keyTable[positions[2]][(positions[3] + 1) % 5];
        } else if (positions[1] == positions[3]) {
            str[i] = keyTable[(positions[0] + 1) % 5][positions[1]];
            str[i + 1] = keyTable[(positions[2] + 1) % 5][positions[3]];
        } else {
            str[i] = keyTable[positions[0]][positions[3]];
            str[i + 1] = keyTable[positions[2]][positions[1]];
        }
    }
}

void decrypt(string& str, const vector<vector<char>>& keyTable) {
    vector<int> positions(4);
    for (int i = 0; i < str.length(); i += 2) {
        search(keyTable, str[i], str[i + 1], positions);
        if (positions[0] == positions[2]) {
            str[i] = keyTable[positions[0]][(positions[1] - 1 + 5) % 5];
            str[i + 1] = keyTable[positions[2]][(positions[3] - 1 + 5) % 5];
        } else if (positions[1] == positions[3]) {
            str[i] = keyTable[(positions[0] - 1 + 5) % 5][positions[1]];
            str[i + 1] = keyTable[(positions[2] - 1 + 5) % 5][positions[3]];
        } else {
            str[i] = keyTable[positions[0]][positions[3]];
            str[i + 1] = keyTable[positions[2]][positions[1]];
        }
    }
}

void encryptByPlayfairCipher(string& str, const string& key) {
    vector<vector<char>> keyTable(5, vector<char>(5));
    string processedKey = key;
    string processedText = str;

    toLowerCase(processedKey);
    removeSpaces(processedKey);

    toLowerCase(processedText);
    removeSpaces(processedText);

    prepare(processedText);

    generateKeyTable(processedKey, keyTable);
    encrypt(processedText, keyTable);

    str = processedText;
}

void decryptByPlayfairCipher(string& str, const string& key) {
    vector<vector<char>> keyTable(5, vector<char>(5));
    string processedKey = key;
    string processedText = str;

    toLowerCase(processedKey);
    removeSpaces(processedKey);

    toLowerCase(processedText);
    removeSpaces(processedText);

    prepare(processedText);

    generateKeyTable(processedKey, keyTable);
    decrypt(processedText, keyTable);

    str = processedText;
}

int main() {
    string plaintext, key;
    cin >> key;
    cin >> plaintext;

    encryptByPlayfairCipher(plaintext, key);
    cout << "Cipher text: " << plaintext << endl;

    decryptByPlayfairCipher(plaintext, key);
    plaintext.pop_back();
    cout << "Decrypted text: " << plaintext << endl;

    return 0;
}