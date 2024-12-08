#include<bits/stdc++.h>
#include<random>
using namespace std;

int randomGen(int num){
    random_device device;
    mt19937 generator(device());
    uniform_int_distribution<int> distribution(0,255);
    return distribution(generator);
}

string binary(int num){
    string ans = "";
    while(num!=0){
        int bit = num & 1;
        num = num >> 1;
        ans += (bit + '0');
    }

    while(ans.length() < 8){
        ans += '0';
    }

    reverse(ans.begin(), ans.end());
    return ans;
}
void convertASCII(string &s, vector<string> &ascii){
    for(char c: s){
        int num = int(c);
        string bin = binary(num);
        ascii.push_back(bin);

    }
}


string encrypt(string &s, vector<int> &key_list1, vector<int> &key_list2){
    string ans = "";
    for(char c: s){
        int num = int(c);

        int key1 = randomGen(num);
        key_list1.push_back(key1);
        num ^= key1;

        int key2 = randomGen(num);
        key_list2.push_back(key2);
        num ^= key2;

        string numstr = binary(num);
        ans += numstr + " ";
    }

    return ans;
}

string KPA(string &cipherText, vector<int>& key_list1, vector<int>& key_list2){
    istringstream ss(cipherText);
    string binarySegment;
    string ans = "";


    int index = 0;
    while(ss >> binarySegment){
        int num = stoi(binarySegment, nullptr, 2);

        int key2 = key_list2[index];
        num ^= key2;

        int key1 = key_list1[index];
        num ^= key1;

        ans += char(num);
        index++;
    }

    return ans;
}
int main(){
    string s;
    cin>>s;

    vector<int> key_list1, key_list2;
    vector<string> ascii;

    convertASCII(s,ascii);

    string encryptedText = encrypt(s, key_list1, key_list2);

    cout<<"encrypted text : "<<encryptedText<<endl;

    string decryptedText = KPA(encryptedText, key_list1, key_list2);
    cout<<"Decrypted Text : "<<decryptedText<<endl;


    return 0;
}