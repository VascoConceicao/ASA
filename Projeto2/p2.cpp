#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <chrono>
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

int DFSVisit(vector<vector<int>> &g, vector<int> &colors, vector<int> &jumps, int i) {  
    int max = 0;
    stack<int> s;
    s.push(i);
    while (!s.empty()) {
        int j = s.top();
        s.pop();
        if (colors[j] == WHITE) {
            colors[j] = GREY;
            s.push(j);
            int size = g[j].size();
            for (int k = 0; k < size; k++) {
                if (colors[g[j][k]] == WHITE) {
                    s.push(g[j][k]);
                } else if (colors[g[j][k]] == BLACK && max < jumps[g[j][k]] + 1) {
                    max = jumps[g[j][k]] + 1;
                }
            }
        } else if (colors[j] == GREY) {
            if (jumps[j] > max)
                max = jumps[j];
            jumps[j] = max;
            order.push_back(j);
            colors[j] = BLACK;
        }
    }
    return max;
}

void firstDFS() {
    order = vector<int>();
    vector<int> jumps = vector<int>(v + 1, 0);
    vector<int> colors = vector<int>(v + 1, 0);
    for (int i = 1; i <= v; i++)
        DFSVisit(g, colors, jumps, i);
}

void secondDFS() {
    vector<int> jumps = vector<int>(v + 1, 0);
    vector<int> colors = vector<int>(v + 1, 0);
    while (order.size() != 0) {
        res = max(res, DFSVisit(gT, colors, jumps, order.back()));
        order.pop_back();
    }
}

vector<int> dfs(const vector<vector<int>>* g, vector<int>* order) {
    vector<int> newOrder;
    newOrder.reserve(v + 1);
    newOrder.push_back(0);
    vector<int> parents = vector<int>(v + 1, 0);
    vector<bool> visited(v + 1, false);
    stack<pair<int, int>> s;

    for (int i = v; i > 0; i--) {
        int start = (*order)[i];
        if (!visited[start]) {
            s.push({start, 0});
            parents[start] = start;

            while (!s.empty()) {
                int current = s.top().first;
                int& neighbor = s.top().second;

                if (!visited[current]) 
                    visited[current] = true;

                int size = (*g)[current].size();
                while (neighbor < size && visited[(*g)[current][neighbor]])
                    ++neighbor;

                if (neighbor < size) {
                    int next = (*g)[current][neighbor++];
                    parents[next] = current;
                    s.push({next, 0});
                }
                else {
                    newOrder.push_back(current);
                    s.pop();
                }
            }
        }
    }
    *order = newOrder;
    return parents;
}

void append2Root(vector<int> *pi) {
    int size = (*pi).size();
    for (int i = 1; i < size; ++i) {
        int j = i;
        while (j != (*pi)[j])
            j = (*pi)[j];
        (*pi)[i] = j;
    }
}

void graphRoot(vector<int> *pi) {
    for (int i = 1; i < v + 1; i++) {
        if ((*pi)[i] != i) 
            while (!g[i].empty()) {
                int k = 0;
                int found = 0;
                int size = g[(*pi)[i]].size();
                if ((*pi)[g[i][g[i].size() - 1]] == (*pi)[i])
                    found = 1;
                while (k < size && !found) {
                    if (g[(*pi)[i]][k] == g[i][g[i].size() - 1])
                        found = 1; 
                    k++;
                }
                if (!found)
                    g[(*pi)[i]].push_back((*pi)[g[i][g[i].size() - 1]]);
                g[i].pop_back();
            }
        else {
            int size = g[i].size();
            for (int j = 0; j < size; j++)
                if (i == (*pi)[g[i][j]]) {
                    g[i].erase(g[i].begin() + j);
                    size --;
                    j--;
                }
                else 
                    g[i][j] = (*pi)[g[i][j]];
        }
    }
}

int findMaxSteps(vector<vector<int>> *g) {
    vector<int> incomingEdges(v + 1, 0);
    vector<int> steps(v + 1, 0);

    for (int i = 1; i < v + 1; ++i)
        for (int j : (*g)[i]) 
            ++incomingEdges[j];

    queue<int> topological;
    for (int i = 1; i < v + 1; ++i)
        if (incomingEdges[i] == 0) {
            topological.push(i);
            steps[i] = 1;
        }

    int maxSteps = 0;
    while (!topological.empty()) {
        int current = topological.front();
        topological.pop();

        for (int neighbor : (*g)[current]) {
            --incomingEdges[neighbor]; 
            steps[neighbor] = max(steps[neighbor], steps[current] + 1);
            if (steps[neighbor] > maxSteps) 
                maxSteps = steps[neighbor];
            if (incomingEdges[neighbor] == 0)
                topological.push(neighbor);
        }
    }

    return (maxSteps == 0) ? 0 : maxSteps - 1;
}

int main() {
    parseDimensions();
    parseEdges();
    if (v >= 35) {
        firstDFS();
        secondDFS();
        printf("%d\n", res);
    }
    else {
        vector<int> order(v + 1, 0);
        for (int i = 1; i <= v; i++)
            order[i] = v - i + 1;
        vector<int> parents = vector<int>(v + 1, 0);
        dfs(&g, &order);
        parents = dfs(&gT, &order);
        append2Root(&parents);
        graphRoot(&parents);
        printf("%d\n", findMaxSteps(&g));
    }
}