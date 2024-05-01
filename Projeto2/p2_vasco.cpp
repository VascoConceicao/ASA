#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
using namespace std;

int v, e;
vector<vector<int>> g;
vector<vector<int>> gT;

int parseDimensions() {
    if (scanf("%d %d", &v, &e) != 2)
        return 1;
    if (v < 2 || e < 1)
        return 1;
    g = vector<vector<int>>(v + 1);
    gT = vector<vector<int>>(v + 1);
    return 0;
}

int parseEdges() {
    for (int i = 0; i < e; i++) {
        int o, d;
        if (scanf("%d %d", &o, &d) != 2) 
            return 1; 
        g[o].push_back(d);
        gT[d].push_back(o);
    }
    return 0;
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
                int& neighborIndex = s.top().second;

                if (!visited[current]) 
                    visited[current] = true;

                int size = (*g)[current].size();
                while (neighborIndex < size && visited[(*g)[current][neighborIndex]])
                    ++neighborIndex;

                if (neighborIndex < size) {
                    int next = (*g)[current][neighborIndex++];
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
    int controlo = parseDimensions();
    if (controlo) {
        printf("0\n");
        return 0;
    }
    controlo = parseEdges();
    if (controlo) {
        printf("0\n");
        return 0;
    }
    vector<int> order(v + 1, 0);
    for (int i = 1; i <= v; i++)
        order[i] = v - i + 1;
    vector<int> parents = vector<int>(v + 1, 0);
    dfs(&g, &order);
    parents = dfs(&gT, &order);
    append2Root(&parents);
    graphRoot(&parents);
    printf("%d\n", findMaxSteps(&g));
    return 0;
}