#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int v, e;
vector<vector<int>> g;
vector<vector<int>> gT;

void parseDimensions() {
    scanf("%d %d", &v, &e);
    g = vector<vector<int>>(v + 1);
    g = vector<vector<int>>(v + 1);
}

void parseEdges() {
    for (int i = 0; i < e; i++) {
        int o, d;
        scanf("%d %d", &o, &d);
        g[o].push_back(d);
    }
}

void swap(vector<int> *v, int p1, int p2) {
    int aux = (*v)[p1];
    (*v)[p1] = (*v)[p2];
    (*v)[p2] = aux;
}

void insertionSort(vector<int> *adj, vector<int> orderIndexed) {
    for (int i = (*adj).size() - 1; i > 0; i--) {
        if (orderIndexed[(*adj)[i - 1]] > orderIndexed[(*adj)[i]])
            swap(adj, i, i - 1);
        else
            break;
    }
}

vector<vector<int>> transposeGraph(vector<vector<int>> g, vector<int> orderIndexed) {
    vector<vector<int>> gT = vector<vector<int>>(v + 1);
    for (int o = 1; o <= v; o++) {
        int size = g[o].size();
        for (int d = 0; d < size; d++) {
            gT[g[o][d]].push_back(o);
            insertionSort(&gT[g[o][d]], orderIndexed);
        }
    }
    return gT;
}

int DFSvisit(vector<vector<int>> g, int i, int time, vector<int> *color, vector<int> *pi, vector<int> *d, vector<int> *f, vector<int> *newOrder, vector<int> *orderIndexed) {
    (*color)[i] = 1;
    time++;
    (*d)[i] = time;
    int size = g[i].size();
    for (int j = 0; j < size; j++) 
        if ((*color)[g[i][j]] == 0) {
            (*pi)[g[i][j]] = i;
            time = DFSvisit(g, g[i][j], time, color, pi, d, f, newOrder, orderIndexed);
        }
    (*color)[i] = 2;
    time++;
    (*f)[i] = time;
    (*orderIndexed)[i] = (*newOrder).size();
    (*newOrder).push_back(i);
    return time;
}

vector<int> DFS(vector<vector<int>> g, vector<int> *order, vector<int> *orderIndexed) {
    vector<int> color = vector<int>(v + 1, 0);
    vector<int> d = vector<int>(v + 1, 0);
    vector<int> f = vector<int>(v + 1, 0);
    vector<int> newOrder = vector<int>(1, 0);
    vector<int> pi = vector<int>(v + 1, 0);
    for (int i = 1; i < v + 1; i++) {
    int size = g[i].size();
    vector<int> aux;
    for (int j = 0; j < size; j++) {
        aux.push_back(g[i][j]);
        insertionSort(&aux, *orderIndexed);
    }
    g[i] = aux;
    }
    int size1 = g.size();
    for (int i = 1; i < size1; i++)
        pi[i] = i;
    int time = 0;
    for (int i = v; i > 0; i--) {
        if (color[(*order)[i]] == 0)
            time = DFSvisit(g, (*order)[i], time, &color, &pi, &d, &f, &newOrder, orderIndexed);
    }
    *order = newOrder;
    return pi;
}

void append2Root(vector<int> *pi) {
    int size = (*pi).size();
    for (int i = 1; i < size; i++) {
        int j = i;
        for (; j != (*pi)[j]; j = (*pi)[j]);
        (*pi)[i] = j;
    }
}

void graphRoot(vector<int> pi) {
    for (int i = 1; i < v + 1; i++) {
        if (pi[i] != i) 
            while (!g[i].empty()) {
                int k = 0;
                int found = 0;
                int size = g[pi[i]].size();
                while (k < size && !found) {
                    if (g[pi[i]][k] == g[i][g[i].size() - 1] || g[i][g[i].size() - 1] == pi[i])
                        found = 1;
                    k++;
                }
                if (!found)
                    g[pi[i]].push_back(g[i][g[i].size() - 1]);
                g[i].pop_back();
            }
        else {
            int size = g[i].size();
            for (int j = 0; j < size; j++) {
                g[i][j] = pi[g[i][j]];
            }
        }
    }
    for (int i = 1; i < v + 1; i++)
        unique(g[i].begin(), g[i].end());
}

int main() {

    parseDimensions();
    parseEdges();
    vector<int> order(v + 1, 0);
    for (int i = 1; i <= v; i++)
        order[i] = order.size() - i;
    vector<int> orderIndexed(v + 1, 0);
    DFS(g, &order, &orderIndexed);
    gT = transposeGraph(g, orderIndexed);
    vector<int> pi = vector<int>(v + 1, 0);
    pi = DFS(gT, &order, &orderIndexed);
    append2Root(&pi);
    graphRoot(pi);
    int max = 0;
    for (int i = 1; i <= v; i++)
        order[i] = order.size() - i;
    vector<int> orderCopy3(order.size());
    copy(order.begin(), order.end(), orderCopy3.begin());
    for (int i = 1; i <= v; i++) {
        vector<int> orderCopy1(order.size());
        vector<int> orderCopy2(order.size());
        copy(order.begin(), order.end(), orderCopy1.begin());
        copy(orderCopy3.begin(), orderCopy3.end(), orderCopy2.begin());
        pi = DFS(g, &order, &orderCopy3);
        int size = pi.size();
        for (int k = 1; k < size; k++) {
            int l = 0;
            int j = k;
            for (; j != pi[j]; j = pi[j])
                l++;
            if (l > max)
                max = l;
        }
        order = orderCopy1;
        orderCopy3 = orderCopy2;
        swap(&order, v - i, v);
        swap(&orderCopy3, 1, i + 1);
    }
    printf("%d\n", max);

}
