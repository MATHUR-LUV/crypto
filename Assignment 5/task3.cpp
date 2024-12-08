#include<string>
#include<algorithm>
#include<unordered_map>
#include<vector>
#include "frequency_table.cpp"
#include<iostream>

using namespace std;


vector<pair<char,char>> getBigrams(string s){
  vector<pair<char,char>> result;
  for(int i=0;i<s.size();i = i+2){
    if(i+1 <= s.size()-1){
      if(s[i] != s[i+1]){
        result.push_back(make_pair(s[i],s[i+1]));
      }
      else{
        result.push_back(make_pair(s[i],s[i] == 'z' ? 'x' : 'z'));
        i--;
      }
    }
    else{
      result.push_back(make_pair(s[i],s[i] == 'z' ? 'x' : 'z'));
    }
  }
  return result;
}

pair<int,int> getCoords(vector<vector<char>> keyMat, char c){
  for(int i=0;i<5;i++){
    for(int j=0;j<5;j++){
      if(keyMat[i][j] == c) return make_pair(i,j);
    }
  }
  return make_pair(-1,-1);
}

string encrypt(string s, string k){
  vector<int> flag(26,0);
  vector<vector<char>> keymatrix (5, vector<char>(5, '\0'));
  int keyPoint = 0;
  char a = 'a';
  for(int i=0;i<5;i++){
    for(int j=0;j<5;j++){
      bool notWritten = true;
      if(keyPoint < k.size()){
        while (flag[k[keyPoint]-'a'])
        {
          keyPoint++;
        }
        if(keyPoint < k.size()){
          // coords[k[keyPoint]-'a'].first = i;
          // coords[k[keyPoint]-'a'].second = j;
          keymatrix[i][j] = k[keyPoint];
          flag[k[keyPoint]-'a']++;
          keyPoint++;
          notWritten = false;
        }
      }
      if(notWritten){
        while(flag[a-'a'] || a == 'j'){
          a++;
        }
        flag[a-'a'] = 1;
        // coords[a-'a'].first = i;
        // coords[a-'a'].second = j;
        keymatrix[i][j] = a;
      }
    }
  }
  // for(auto x : keymatrix){
  //   for(auto y : x){
  //     cout << y << " ";
  //   }
  //   cout << endl;
  // }
  string cipher = "";
  vector<pair<char,char>> bigrams = getBigrams(s);
  for(auto c : bigrams){
    // cout << c.first << " " << c.second << endl;
    pair<int,int> firstLetter = getCoords(keymatrix, c.first);
    pair<int,int> secondLetter = getCoords(keymatrix,c.second);
    
    if(firstLetter.first == secondLetter.first){
      cipher += keymatrix[firstLetter.first][(firstLetter.second+1)%5];
      cipher += keymatrix[secondLetter.first][(secondLetter.second+1)%5];
    }
    else if(firstLetter.second == secondLetter.second){
      cipher += keymatrix[(firstLetter.first+1)%5][firstLetter.second];
      cipher += keymatrix[(secondLetter.first+1)%5][secondLetter.second];
    }
    else{
      cipher += keymatrix[firstLetter.first][secondLetter.second];
      cipher += keymatrix[secondLetter.first][firstLetter.second];
    }
  }
  return cipher;
}

string decrypt(string s, string k){
  vector<int> flag(26,0);
  vector<vector<char>> keymatrix (5, vector<char>(5, '\0'));
  int keyPoint = 0;
  char a = 'a';
  for(int i=0;i<5;i++){
    for(int j=0;j<5;j++){
      bool notWritten = true;
      if(keyPoint < k.size()){
        while (flag[k[keyPoint]-'a'])
        {
          keyPoint++;
        }
        if(keyPoint < k.size()){
          // coords[k[keyPoint]-'a'].first = i;
          // coords[k[keyPoint]-'a'].second = j;
          keymatrix[i][j] = k[keyPoint];
          flag[k[keyPoint]-'a']++;
          keyPoint++;
          notWritten = false;
        }
      }
      if(notWritten){
        while(flag[a-'a'] || a == 'j'){
          a++;
        }
        flag[a-'a'] = 1;
        // coords[a-'a'].first = i;
        // coords[a-'a'].second = j;
        keymatrix[i][j] = a;
      }
    }
  }
  // for(auto x : keymatrix){
  //   for(auto y : x){
  //     cout << y << " ";
  //   }
  //   cout << endl;
  // }
  string cipher = "";
  vector<pair<char,char>> bigrams = getBigrams(s);
  for(auto c : bigrams){
    // cout << c.first << " " << c.second << endl;
    pair<int,int> firstLetter = getCoords(keymatrix, c.first);
    pair<int,int> secondLetter = getCoords(keymatrix,c.second);
    
    if(firstLetter.first == secondLetter.first){
      cipher += keymatrix[firstLetter.first][(firstLetter.second ? firstLetter.second-1 : 4)];
      cipher += keymatrix[secondLetter.first][(secondLetter.second ? secondLetter.second-1 : 4)];
    }
    else if(firstLetter.second == secondLetter.second){
      cipher += keymatrix[(firstLetter.first ? firstLetter.first-1 : 4)][firstLetter.second];
      cipher += keymatrix[(secondLetter.first ? secondLetter.first-1 : 4)][secondLetter.second];
    }
    else{
      cipher += keymatrix[firstLetter.first][secondLetter.second];
      cipher += keymatrix[secondLetter.first][firstLetter.second];
    }
  }
  return cipher;
}

string replaceJTI(string s){
  string result = "";
  for(auto x : s){
    if(x != 'j') result += x;
  }
  return result;
}

string break_playfair(string cipher, string partial_plain){
  vector<vector<pair<char,char>>> pairMap(26,vector<pair<char,char>>(26,make_pair('-','-')));
  vector<pair<char,char>> chipherBi = getBigrams(cipher);
  vector<pair<char,char>> partial_plainBi = getBigrams(partial_plain);
  // cout << partial_plainBi.size() << endl;
  for(int i=0;i<partial_plainBi.size();i++){
    pairMap[chipherBi[i].first-'a'][chipherBi[i].second-'a'] = make_pair(partial_plainBi[i].first, partial_plainBi[i].second);
    pairMap[chipherBi[i].second-'a'][chipherBi[i].first-'a'] = make_pair(partial_plainBi[i].second, partial_plainBi[i].first);
  }
  string result = "";
  for(auto x : chipherBi){
    result += pairMap[x.first-'a'][x.second-'a'].first;
    result += pairMap[x.first-'a'][x.second-'a'].second;
  }
  return result;
}

void findDiff(string s, string t){
  int same = 0;
  int diff = 0;
  for(int i=0;i<s.size();i++){
    if(s[i] == t[i]) same ++;
    else diff++;
  }
  cout << same << " " << diff << endl;
}

string freq_attack(string cipher){
    unordered_map<string, double> freq_CiBi;
    for(int i=0;i<cipher.size()-1;i+=2){
      freq_CiBi[cipher.substr(i,2)]++;
    }
    int totalFreq = 0;
    for(auto x : freq_CiBi){
      totalFreq += x.second;
    }
    vector<string> keys_ci;
    vector<string> keys_std;
    for(auto x : bigram_map){
      keys_std.push_back(x.first);
    }
    // cout << freq_CiBi[keys_ci[0]] << endl;
    for(auto &x : freq_CiBi){
      keys_ci.push_back(x.first);
      x.second = x.second/totalFreq;
    }
    // cout << totalFreq << endl;
    sort(keys_ci.begin(),keys_ci.end(), [&freq_CiBi](string &a, string &b){
      return freq_CiBi[a] > freq_CiBi[b];
    });
    // for(auto x : keys_ci){
    //   cout << x << " " << freq_CiBi[x] <<endl; 
    // }
    sort(keys_std.begin(),keys_std.end(), [](string &a, string &b){
      return bigram_map[a] > bigram_map[b];
    });
    // cout << totalFreq << " " << << endl;
    string result = cipher;
    for(int i=0;i<keys_ci.size();i+=2){
      size_t pos = 0;
      while ((pos = result.find(keys_ci[i], 2)) != std::string::npos) {
        result.replace(pos, 2, keys_std[i]);
        pos += keys_std[i].length();
        }
    }
    return result;
}

int main() {
    string plain, key;
    vector<string> keys;

    // Input the plain text message
    cout << "Enter the message to encrypt (use lowercase letters only, replace 'j' with 'i'): ";
    cin >> plain;
    plain = replaceJTI(plain);

    // Input the number of keys
    int numKeys;
    cout << "Enter the number of keys: ";
    cin >> numKeys;

    // Input the keys
    cout << "Enter the keys (use lowercase letters only, replace 'j' with 'i'):" << endl;
    for (int i = 0; i < numKeys; ++i) {
        cin >> key;
        keys.push_back(key);
    }

    // Process each key
    for (const auto& currentKey : keys) {
        string cipher = encrypt(plain, currentKey);
        cout << "Key = " << currentKey << ", Cipher = " << cipher << endl << endl;
        cout << "Decryption : " << decrypt(cipher, currentKey) << endl;

        // Perform Known Plaintext Attack
        string kpa = break_playfair(cipher, plain.substr(0, plain.size() / 2));
        cout << "KPA attempt : " << kpa << endl;

        // Perform Frequency Analysis Attack
        string freq_at = freq_attack(cipher);
        cout << "Freq attempt : " << freq_at << endl;

        // Find difference between KPA and Frequency Analysis results
        findDiff(kpa, freq_at);
    }

    return 0;
}