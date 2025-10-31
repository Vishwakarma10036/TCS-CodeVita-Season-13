#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    cin>>n;
    vector<string> names(n);
    for(int i=0; i<n; i++) cin>>names[i];
    vector<int> skill(n);
    for (int i=0; i<n; i++) cin>>skill[i];
    int f;
    cin>>f;
    map<string, int> nameIndex;
    for(int i = 0; i<n; i++) nameIndex[names[i]]=i;

    vector<int> pairIdx(n, -1);
    for(int i=0; i < f; i++){
        string a, b;
        cin>>a>>b;
        int x=nameIndex[a], y=nameIndex[b];
        pairIdx[x]=y;
        pairIdx[y]=x;
    }

    int r;
    cin>>r;
    set<pair<int, int>>rivals;
    for(int i = 0; i < r; i++){
        string a, b;
        cin>>a>>b;
        int x=nameIndex[a], y=nameIndex[b];
        rivals.insert({min(x, y), max(x, y)});
    }
    int limit;
    cin>>limit;
    vector<pair<int, vector<int>>> groups;
    vector<int> used(n, 0);
    for(int i=0; i < n; i++){
        if(used[i]) continue;
        if(pairIdx[i]!=-1){
            int j=pairIdx[i];
            groups.push_back({skill[i]+skill[j], {i, j}});
            used[i]=used[j]=1;
        }else{
            groups.push_back({skill[i], {i}});
            used[i] = 1;
        }
    }

    int m=groups.size();
    vector<int> dp(limit + 1, 0);
    vector<set<int>> chosen(limit + 1);

    for(int i=0; i<m; i++){
        int w=groups[i].first;
        int cnt=groups[i].second.size();
        for(int j=limit; j>=w; j--){
            bool ok=true;
            for(auto rv : rivals){
                if(chosen[j-w].count(rv.first) && find(groups[i].second.begin(), groups[i].second.end(), rv.second) != groups[i].second.end())
                    ok=false;
                if(chosen[j-w].count(rv.second) && find(groups[i].second.begin(), groups[i].second.end(), rv.first) != groups[i].second.end())
                    ok=false;
            }
            if(!ok)continue;

            if(dp[j-w]+cnt>dp[j]){
                dp[j]=dp[j-w] + cnt;
                chosen[j]=chosen[j-w];
                for (int x:groups[i].second) chosen[j].insert(x);
            }
        }
    }
    cout<< *max_element(dp.begin(), dp.end());
    return 0;
}