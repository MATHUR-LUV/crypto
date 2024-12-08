#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <random>

using namespace std;

// Key Scheduling Algorithm (KSA)
vector<int> rc4_ksa(const vector<int>& key) {
    int key_length = key.size();
    vector<int> S(256);
    for (int i = 0; i < 256; i++) {
        S[i] = i;
    }

    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % key_length]) % 256;
        swap(S[i], S[j]);
    }

    return S;
}

// Pseudo-Random Generation Algorithm (PRGA)
vector<int> rc4_prga(vector<int> S, int message_length) {
    int i = 0, j = 0;
    vector<int> keystream(message_length);

    for (int k = 0; k < message_length; k++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        swap(S[i], S[j]);
        keystream[k] = S[(S[i] + S[j]) % 256];
    }

    return keystream;
}

int main() {
    vector<int> key_lengths = {5, 16, 32}; // Key lengths in bytes
    vector<int> message_sizes = {1024, 10240, 102400, 1048576}; // Message sizes

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 255);

    for (int key_length : key_lengths) {
        vector<int> key(key_length);
        for (int &k : key) {
            k = dist(gen);
        }

        cout << "Key Length : " << key_length * 8 << " bits\n\n";

        for (int message_size : message_sizes) {
            vector<int> message(message_size);
            for (int &m : message) {
                m = dist(gen);
            }

            cout << "Message length = " << message.size() << endl;

            // Measure KSA time
            auto start_time = chrono::high_resolution_clock::now();
            vector<int> S = rc4_ksa(key);
            auto ksa_time = chrono::high_resolution_clock::now() - start_time;

            // Measure PRGA time
            start_time = chrono::high_resolution_clock::now();
            vector<int> keystream = rc4_prga(S, message.size());
            auto prga_time = chrono::high_resolution_clock::now() - start_time;

            cout << "Time for KSA : " 
                 << chrono::duration<double>(ksa_time).count() << " seconds\n";
            cout << "Time for PRGA : " 
                 << chrono::duration<double>(prga_time).count() << " seconds\n";
            cout << "------------------------------------------------------------------\n";
        }
        cout << "##########################################################\n";
    }

    return 0;
}
