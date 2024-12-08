#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

// Function to generate the Playfair matrix from the key
vector<vector<char>> generateMatrix(const string& key) {
    string alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
    unordered_map<char, bool> used;
    vector<vector<char>> matrix(5, vector<char>(5));
    string processedKey = "";

    for (char ch : key) {
        if (ch == 'J') ch = 'I'; // Convert 'J' to 'I'
        if (!used[ch] && isalpha(ch)) {
            processedKey += toupper(ch); // Ensure uppercase letters
            used[ch] = true;
        }
    }

    for (char ch : alphabet) {
        if (!used[ch]) {
            processedKey += ch;
            used[ch] = true;
        }
    }

    int index = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            matrix[i][j] = processedKey[index++];
        }
    }
    return matrix;
}

// Function to prepare the message for encryption/decryption
string prepareMessage(const string& message) {
    string preparedMessage = message;
    if (preparedMessage.length() % 2 != 0) preparedMessage += 'X'; // Add padding if needed
    return preparedMessage;
}

// Function to find the position of a character in the Playfair matrix
pair<int, int> findPosition(const vector<vector<char>>& matrix, char ch) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (matrix[i][j] == ch) {
                return {i, j};
            }
        }
    }
    return {-1, -1}; // Should not happen if the matrix is correct
}

// Function to get digraphs from the message
vector<pair<char, char>> getBigrams(const string& s) {
    vector<pair<char, char>> result;
    for (size_t i = 0; i < s.size(); i += 2) {
        char a = s[i];
        char b = (i + 1 < s.size()) ? s[i + 1] : 'X'; // Handle odd length
        if (a == b) b = (a == 'X') ? 'Z' : 'X'; // Replace duplicate letters
        result.push_back({a, b});
    }
    return result;
}

// Function to encrypt a message using the Playfair cipher
string encryptMessage(const string& message, const vector<vector<char>>& matrix) {
    string ciphertext = "";
    vector<pair<char, char>> bigrams = getBigrams(message);
    for (const auto& bigram : bigrams) {
        pair<int, int> posA = findPosition(matrix, bigram.first);
        pair<int, int> posB = findPosition(matrix, bigram.second);
        int rowA = posA.first, colA = posA.second;
        int rowB = posB.first, colB = posB.second;

        if (rowA == rowB) { // Same row
            ciphertext += matrix[rowA][(colA + 1) % 5];
            ciphertext += matrix[rowB][(colB + 1) % 5];
        } else if (colA == colB) { // Same column
            ciphertext += matrix[(rowA + 1) % 5][colA];
            ciphertext += matrix[(rowB + 1) % 5][colB];
        } else { // Rectangle
            ciphertext += matrix[rowA][colB];
            ciphertext += matrix[rowB][colA];
        }
    }
    return ciphertext;
}

// Function to decrypt a message using the Playfair cipher
string decryptCipher(const string& ciphertext, const vector<vector<char>>& matrix) {
    string message = "";
    vector<pair<char, char>> bigrams = getBigrams(ciphertext);
    for (const auto& bigram : bigrams) {
        pair<int, int> posA = findPosition(matrix, bigram.first);
        pair<int, int> posB = findPosition(matrix, bigram.second);
        int rowA = posA.first, colA = posA.second;
        int rowB = posB.first, colB = posB.second;

        if (rowA == rowB) { // Same row
            message += matrix[rowA][(colA - 1 + 5) % 5];
            message += matrix[rowB][(colB - 1 + 5) % 5];
        } else if (colA == colB) { // Same column
            message += matrix[(rowA - 1 + 5) % 5][colA];
            message += matrix[(rowB - 1 + 5) % 5][colB];
        } else { // Rectangle
            message += matrix[rowA][colB];
            message += matrix[rowB][colA];
        }
    }
    return message;
}

// Function to perform a Known Plaintext Attack
string break_playfair(const string& ciphertext, const string& P_partial, const vector<string>& keys) {
    string preparedPartial = prepareMessage(P_partial); // Ensure partial plaintext is prepared
    string bestKey;
    string bestPlaintext;
    
    for (const string& key : keys) {
        vector<vector<char>> matrix = generateMatrix(key);
        string decryptedText = decryptCipher(ciphertext, matrix);
        
        // Debugging output
        cout << "Trying key: " << key << endl;
        cout << "Decrypted Text: " << decryptedText << endl;

        if (decryptedText.find(preparedPartial) != string::npos) {
            bestKey = key;
            bestPlaintext = decryptedText;
            break; // Exit after finding the key
        }
    }

    if (!bestKey.empty()) {
        cout << "Key: " << bestKey << ", Decrypted Text: " << bestPlaintext << endl;
    } else {
        cout << "No key matched the partial plaintext." << endl;
    }
    return bestKey;
}

int main() {
    string message;
    int n;

    // Task 1: Encrypt the message with multiple keys
    cout << "Enter the message to encrypt (use capital letters only): ";
    cin >> message;

    cout << "Enter the number of keys: ";
    cin >> n;

    vector<string> keys(n);
    cout << "Enter the keys (use capital letters only):" << endl;
    for (int i = 0; i < n; ++i) {
        cin >> keys[i];
    }

    string preparedMessage = prepareMessage(message);
    vector<string> ciphers;

    for (const string& key : keys) {
        vector<vector<char>> playfairSquare = generateMatrix(key);
        string ciphertext = encryptMessage(preparedMessage, playfairSquare);
        ciphers.push_back(ciphertext);
        cout << "Key: " << key << ", Ciphertext: " << ciphertext << endl;
    }

    // Task 2: Known Plaintext Attack
    string ciphertext = ciphers[0]; // For the known plaintext attack, take the first ciphertext as an example
    string partialPlaintext;
    cout << "Enter the known partial plaintext: ";
    cin >> partialPlaintext;

    break_playfair(ciphertext, partialPlaintext, keys);

    return 0;
}