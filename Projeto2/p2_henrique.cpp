#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
using namespace std;

const int WHITE = 0;
const int GREY = 1;
const int BLACK = 2;

int v, e;
vector<vector<int>> g;

void parseDimensions() {
    scanf("%d %d", &v, &e);
    g = vector<vector<int>>(v + 1, vector<int>(v + 1, 0));
}

void parseEdges() {
    for (int i = 0; i < e; i++) {
        int o, d;
        scanf("%d %d", &o, &d);
        g[o][d] = 1;
    }
}

void printVector(vector<int> v) {
    for (int i = 1; i < v.size(); i++) {
        if (i != 1)
            printf(" ");
        printf("%d", v[i]);
    }
    printf("\n");
}

void printGraph(vector<vector<int>> g) {
    for (int o = 1; o <= v; o++)
        printVector(g[o]);
}

void swap(vector<int> *v, int p1, int p2) {
    int aux = (*v)[p2];
    (*v)[p2] = (*v)[p1];
    (*v)[p1] = aux;
}

int DFSVisit(vector<vector<int>> g, vector<int> order, vector<int> *colors, vector<int> *parents, vector<int> *f, int i, int time) {  
    stack<int> s;
    s.push(i);
    while (!s.empty()) {
        int j = s.top();
        s.pop();
        if ((*colors)[j] == WHITE) {
            time++;
            (*colors)[j] = GREY;
            s.push(j);
            for (int k = v; k >= 1; k--) {
                if (g[j][order[k]] && (*colors)[order[k]] == WHITE) {
                    (*parents)[order[k]] = j;
                    s.push(order[k]);
                }
            }
        } else if ((*colors)[j] == GREY) {
            (*f)[j] = time++;
            (*colors)[j] = BLACK;
        }
    }
    return time;
}

void DFS(vector<vector<int>> g, vector<int> order, vector<int> *parents, vector<int> *f) {
    vector<int> colors = vector<int>(v + 1);
    for (int i = 1; i <= v; i++) {
        colors[i] = WHITE;
        (*parents)[i] = 0;
    }
    int time = 1;
    for (int i = 1; i <= v; i++)
        time = DFSVisit(g, order, &colors, parents, f, order[i], time);
}

void DAGLongestPath(vector<vector<int>> g, int s, vector<int> order, vector<int> *l) {
    for (int i = 1; i <= v; i++)
        (*l)[i] = 0;
    for (int i = s; i <= v; i++) {
        for (int j = i; j <= v; j++)
            if (g[order[i]][order[j]] && (*l)[order[i]] >= (*l)[order[j]])
                (*l)[order[j]] = (*l)[order[i]] + 1;
    }
}

int main() {

    parseDimensions();
    parseEdges();
    printf("Graph g\n");
    printGraph(g);

    vector<int> parents = vector<int>(v + 1);
    vector<int> f = vector<int>(v + 1);
    vector<int> order = vector<int>(v + 1);
    for(int i = 1; i <= v; i++)
        order[i] = i;
    DFS(g, order, &parents, &f);
    printf("\nPrimeira DFS executada\n\n");
    printf("Order\n");
    printVector(order);
    printf("Parents\n");
    printVector(parents);
    printf("f\n");
    printVector(f);

    vector<int> newOrder = vector<int>(1);
    for (int i = 1; i <= v; i++) {
        newOrder.push_back(i);
        int j = i;
        while (j > 1 && f[newOrder[j]] > f[newOrder[j - 1]]) {
            swap(&newOrder, j, j - 1);
            j--;
        }
        printf("i -> %d | ", i);
        printVector(newOrder);
    }
    printf("NewOrder\n");
    printVector(newOrder);

    for (int i = 1; i <= v; i++)
        for (int j = 1; j <= v; j++)
            if (g[i][j] == 1) {
                if (g[j][i] == 0)
                    g[i][j] = 0;
                g[j][i] = 2;
            }
    printf("Graph gT\n");
    printGraph(g);
    DFS(g, newOrder, &parents, &f);
    printf("\nSegunda DFS executada\n(Grafo transposto com ordem de maior tempo de fim)\n\n");
    printf("Order\n");
    printVector(newOrder);
    printf("Parents\n");
    printVector(parents);
    printf("f\n");
    printVector(f);

    vector<int> newParents = vector<int>(v + 1);
    for (int i = 1; i <= v; i++) {
        int newParent = parents[i];
        while (parents[newParent])
            newParent = parents[newParent];
        newParents[i] = newParent;
    }
    for (int i = 1; i <= v; i++) {
        if (newParents[i]) {
            for (int j = 1; j <= v; j++)
                if (g[i][j]) {
                    g[i][j] = 0;
                    if (newParents[i] != newParents[j] && newParents[i] != j)
                        g[newParents[i]][j] = 1;
                }
            for (int j = 1; j <= v; j++)
                if (g[j][i]) {
                    g[j][i] = 0;
                    if (newParents[i] != newParents[j] && j != newParents[i])
                        g[j][newParents[i]] = 1;
                }
        }
    }
    printf("Graph SCC\n");
    printGraph(g);
    
    vector<int> topologicalOrder = vector<int>(1);
    for (int i = 1; i <= v; i++) {
        topologicalOrder.push_back(i);
        int j = i;
        while (j > 1 && f[topologicalOrder[j]] > f[topologicalOrder[j - 1]]) {
            swap(&topologicalOrder, j, j - 1);
            j--;
        }
        printf("i -> %d | ", i);
        printVector(topologicalOrder);
    }
    printf("TopologicalOrder\n");
    printVector(topologicalOrder);
    int max = 0;
    vector<int> l = vector<int>(v + 1);
    for (int i = 1; i <= v; i++) {
        DAGLongestPath(g, i, topologicalOrder, &l);
        for (int j = 1; j <= v; j++)
            if (l[j] > max)
                max = l[j];
    }

    printf("max -> %d\n", max);
    return 0;      
}
