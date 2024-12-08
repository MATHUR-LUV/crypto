#include <iostream>
#include <vector>
using namespace std;

vector<int> generateLFSRSequence(vector<int> seed, vector<int> feedbackPoly, int length) {
    int n = seed.size();
    vector<int> sequence;

    for (int i = 0; i < length; ++i) {
        int newBit = 0;
        for (int j = 0; j < n; ++j) {
            newBit ^= seed[j] * feedbackPoly[j];
        }

        sequence.push_back(seed[0]);
        for (int j = 0; j < n - 1; ++j) {
            seed[j] = seed[j + 1];
        }
        seed[n - 1] = newBit;
    }

    return sequence;
}


vector<int> berlekampMassey(const vector<int>& sequence) {
    int n = sequence.size();
    vector<int> feedbackPoly(n, 0), candidatePoly(n, 0);
    feedbackPoly[0] = 1; 
    candidatePoly[0] = 1;

    int discrepancy, L = 0;
    for (int i = 0; i < n; ++i) {
        discrepancy = sequence[i];
        for (int j = 1; j <= L; ++j) {
            discrepancy ^= feedbackPoly[j] * sequence[i - j];
        }

        if (discrepancy != 0) {
            vector<int> temp = feedbackPoly;
            for (int j = 0; j < n - i; ++j) {
                feedbackPoly[i + j] ^= candidatePoly[j];
            }
            if (2 * L <= i) {
                candidatePoly = temp;
                L = i + 1 - L;
            }
        }
    }

    feedbackPoly.resize(L + 1);
    return feedbackPoly;
}

int main() {

    vector<int> seed = {1, 0, 1};              
    vector<int> feedbackPoly = {1, 0, 1};      

    int sequenceLength = 6; 
    cout << "Generating LFSR Sequence...\n";

    vector<int> sequence = generateLFSRSequence(seed, feedbackPoly, sequenceLength);

    cout << "Generated Sequence: ";
    for (int bit : sequence) {
        cout << bit;
    }
    cout << endl;


    cout << "Recovering Feedback Polynomial using Berlekamp-Massey Algorithm...\n";
    vector<int> recoveredPoly = berlekampMassey(sequence);

    cout << "Recovered Feedback Polynomial: ";
    for (int coeff : recoveredPoly) {
        cout << coeff << " ";
    }
    cout << endl;



    return 0;
}
