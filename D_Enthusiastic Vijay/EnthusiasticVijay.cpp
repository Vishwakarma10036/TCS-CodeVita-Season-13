#include <bits/stdc++.h>
using namespace std;

int main() {
    string s1, s2, s3;
    getline(cin, s1);
    getline(cin, s2);
    getline(cin, s3);
    int K;
    cin >> K;
    
    int len = s1.length();
    int N = (len + 1) / 5;
    vector<int> digits;
    for(int i = 0; i < N; i++) {
        string p = "";
        p += s1.substr(5 * i, 3);
        p += s2.substr(5 * i, 3);
        p += s3.substr(5 * i, 3);
        set<int> on;
        for(int j = 0; j < 9; j++) {
            if(p[j] != ' ') on.insert(j);
        }
        int d = -1;
        if(on == set<int>{0,2,3,5,6,8}) d = 0;
        else if(on == set<int>{2,5,8}) d = 1;
        else if(on == set<int>{3,4,5,6,7,8}) d = 2;
        else if(on == set<int>{0,4,8}) d = 4;
        else if(on == set<int>{0,2,3,5,8}) d = 6;
        else if(on == set<int>{0,2,5,8}) d = 7;
        else if(on == set<int>{0,2,3,5,6,8}) d = 8;
        else if(on == set<int>{0,2,5,6,8}) d = 9;
        digits.push_back(d);
    }
    
    long long original = 0;
    for(int d : digits) original = original * 10 + d;
    
    vector<int> modified = digits;
    for(int i = 0; i < N && K > 0; i++) {
        if(modified[i] == 4) {
            modified[i] = 0;
            K--;
        } else if(modified[i] == 7) {
            modified[i] = 1;
            K--;
        } else if(modified[i] == 8) {
            modified[i] = 7;
            K--;
        }
    }
    
    string s;
    for(int d : modified) s += '0' + d;
    
    next_permutation(s.begin(), s.end());
    long long next_ana = stoll(s);
    long long diff = abs(original - next_ana);
    cout << diff << endl;
    return 0;
}