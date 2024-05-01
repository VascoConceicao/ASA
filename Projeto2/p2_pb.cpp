#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
using namespace std;

int v, e;
vector<vector<int>> g;
vector<vector<int>> gT;
vector<int> order;
int res;

const int WHITE = 0;
const int GREY = 1;
const int BLACK = 2;

void parseDimensions() {
    scanf("%d %d", &v, &e);
    g = vector<vector<int>>(v + 1);
    gT = vector<vector<int>>(v + 1);
}

void parseEdges() {
    for (int n = 1; n <= e; n++) {
        int i, j;
        scanf("%d %d", &i, &j);
        g[i].push_back(j);
        gT[j].push_back(i);
    }
}

int DFSVisit(vector<int> &colors, int i, int time) {  
    stack<int> s;
    s.push(i);
    while (!s.empty()) {
        int j = s.top();
        s.pop();
        if (colors[j] == WHITE) {
            time++;
            colors[j] = GREY;
            s.push(j);
            int size = g[j].size();
            for (int k = 0; k < size; k++)
                if (colors[g[j][k]] == WHITE)
                    s.push(g[j][k]);
        } else if (colors[j] == GREY) {
            order.push_back(j);
            colors[j] = BLACK;
        }
    }
    return time;
}

void firstDFS() {
    order = vector<int>();
    vector<int> colors = vector<int>(v + 1);
    for (int i = 1; i <= v; i++)
        colors[i] = WHITE;
    int time = 1;
    for (int i = 1; i <= v; i++)
        time = DFSVisit(colors, i, time);
}

int SCCVisit(vector<int> &colors, vector<int> &jumps, int i) {
    int max = 0;
    stack<int> s;
    s.push(i);
    while (!s.empty()) {
        int j = s.top();
        s.pop();
        if (colors[j] == WHITE) {
            colors[j] = GREY;
            s.push(j);
            int size = gT[j].size();
            for (int k = 0; k < size; k++) {
                if (colors[gT[j][k]] == WHITE)
                    s.push(gT[j][k]);
                else if (colors[gT[j][k]] == BLACK && max < jumps[gT[j][k]] + 1)
                    max = jumps[gT[j][k]] + 1;
            }
        } else if (colors[j] == GREY) {
            jumps[j] = max;
            colors[j] = BLACK;
        }
    }
    return max;
}

void secondDFS() {
    vector<int> jumps = vector<int>(v + 1);
    vector<int> colors = vector<int>(v + 1);
    for (int i = 1; i <= v; i++)
        colors[i] = WHITE;
    int max = 0;
    while (order.size() != 0) {
        max = SCCVisit(colors, jumps, order.back());
        if (max > res)
            res = max;
        order.pop_back();
    }
}

int main() {
    parseDimensions();
    parseEdges();
    firstDFS();
    secondDFS();
    printf("%d\n", res);
}
