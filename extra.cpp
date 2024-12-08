#include <iostream>
#include <cmath>
using namespace std;

long long modular_exponentiation(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

int main() {
    long long p = 23;
    long long g = 5;

    cout << "Public parameters:\n";
    cout << "Prime (p): " << p << "\nGenerator (g): " << g << "\n\n";

    long long a = 6;
    long long b = 15;

    long long A = modular_exponentiation(g, a, p);
    long long B = modular_exponentiation(g, b, p);

    cout << "Alice's private key: " << a << "\n";
    cout << "Alice's public key: " << A << "\n";
    cout << "Bob's private key: " << b << "\n";
    cout << "Bob's public key: " << B << "\n\n";

    long long shared_secret_A = modular_exponentiation(B, a, p);
    long long shared_secret_B = modular_exponentiation(A, b, p);

    cout << "Shared secret computed by Alice: " << shared_secret_A << "\n";
    cout << "Shared secret computed by Bob: " << shared_secret_B << "\n";

    return 0;
}
