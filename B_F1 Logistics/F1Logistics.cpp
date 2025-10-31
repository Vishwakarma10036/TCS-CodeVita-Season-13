#include <bits/stdc++.h>
using namespace std;
struct Race{
    int x, y, day;
};

bool canTravel(const Race &a, const Race &b){
    int travelTime=abs(a.x-b.x)+abs(a.y-b.y);
    return(b.day-a.day)>=travelTime;
}

bool dfsMatch(int u, vector<vector<int>> &graph, vector<int> &visited, vector<int> &linked){
    for(int v:graph[u]){
        if(visited[v])continue;
        visited[v]=1;
        if(linked[v]==-1||dfsMatch(linked[v], graph, visited, linked)){
            linked[v]=u;
            return true;
        }
    }
    return false;
}

int main(){
    int totalRaces;
    cin>>totalRaces;
    vector<Race>events(totalRaces);
    for(int i=0; i<totalRaces; i++)
        cin>>events[i].x>>events[i].y>>events[i].day;

    sort(events.begin(), events.end(), [](const Race &a, const Race &b){
        return a.day<b.day;
    });
    vector<vector<int>> graph(totalRaces);
    for(int i = 0; i < totalRaces; i++){
        for(int j = i + 1; j < totalRaces; j++){
            if(canTravel(events[i], events[j])){
                graph[i].push_back(j);
            }
        }
    }
    vector<int>linked(totalRaces, -1);
    int maxMatching=0;
    for(int i=0; i<totalRaces; i++){
        vector<int> visited(totalRaces, 0);
        if(dfsMatch(i, graph, visited, linked))
            maxMatching++;
    }
    int minCars=totalRaces-maxMatching;
    cout<<minCars;
    return 0;
}