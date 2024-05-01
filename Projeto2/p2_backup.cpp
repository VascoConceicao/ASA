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

void printList(vector<int> v) {
    for (int i = 1; i < v.size(); i++) {
        if (i != 1)
            printf(" ");
        printf("%d", v[i]);
    }
    printf("\n");
}

void printGraph(vector<vector<int>> g) {
    for (int o = 1; o <= v; o++) {
        printf("%d ->", o);
        for (int d = 0; d < g[o].size(); d++)
            printf(" %d", g[o][d]);
        printf("\n");
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
    for (int o = 1; o <= v; o++)
        for (int d = 0; d < g[o].size(); d++) {
            gT[g[o][d]].push_back(o);
            insertionSort(&gT[g[o][d]], orderIndexed);
        }
    return gT;
}

int DFSvisit(vector<vector<int>> g, int i, int time, vector<int> *color, vector<int> *π, vector<int> *d, vector<int> *f, vector<int> *newOrder, vector<int> *orderIndexed) {
    (*color)[i] = 1;
    time++;
    (*d)[i] = time;
    for (int j = 0; j < g[i].size(); j++) 
        if ((*color)[g[i][j]] == 0) {
            (*π)[g[i][j]] = i;
            time = DFSvisit(g, g[i][j], time, color, π, d, f, newOrder, orderIndexed);
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
    vector<int> π = vector<int>(v + 1, 0);
    for (int i = 1; i < v + 1; i++) {
        int size = g[i].size();
        vector<int> aux;
        for (int j = 0; j < size; j++) {
            aux.push_back(g[i][j]);
            insertionSort(&aux, *orderIndexed);
        }
        g[i] = aux;
    }
    printf("depoies de ordem\n");
    printGraph(g);
    int size = g.size();
    for (int i = 1; i < size; i++)
        π[i] = i;
    int time = 0;
    for (int i = v; i > 0; i--) {
        if (color[(*order)[i]] == 0)
            time = DFSvisit(g, (*order)[i], time, &color, &π, &d, &f, &newOrder, orderIndexed);
    }
    *order = newOrder;
    printf("\nπ\n");
    printList(π);
    printf("\nd\n");
    printList(d);
    printf("\nf\n");
    printList(f);
    return π;
}

void append2Root(vector<int> *π) {
    for (int i = 1; i < (*π).size(); i++) {
        int j = i;
        for (; j != (*π)[j]; j = (*π)[j]);
        (*π)[i] = j;
    }
}

void graphRoot(vector<int> π) {
    for (int i = 1; i < v + 1; i++) {
        if (π[i] != i)
            while (!g[i].empty()) {
                int k = 0;
                int found = 0;
                int size = g[π[i]].size();
                while (k < size && !found) {
                    if (g[π[i]][k] == g[i][g[i].size() - 1] || g[i][g[i].size() - 1] == π[i])
                        found = 1;
                    k++;
                }
                if (!found)
                    g[π[i]].push_back(g[i][g[i].size() - 1]);
                g[i].pop_back();
            }
        else {
            int size = g[i].size();
            for (int j = 0; j < size; j++) {
                g[i][j] = π[g[i][j]];
            }
        }
    }
    for (int i = 1; i < v + 1; i++)
        unique(g[i].begin(), g[i].end());
}

int main() {

    parseDimensions();
    parseEdges();
    printf("\nNao ordenado\n");
    printGraph(g);
    printf("\nOrdenado\n");
    for (int i = 1; i <= v; i++)
        sort(g[i].begin(), g[i].end());
    printGraph(g);
    vector<int> order(v + 1, 0);
    for (int i = 1; i <= v; i++)
        order[i] = order.size() - i;
    vector<int> orderIndexed(v + 1, 0);
    printf("\norder\n");
    printList(order);
    printf("\nDFS\n");
    DFS(g, &order, &orderIndexed);
    printf("\nnewOrder\n");
    printList(order);
    printf("\norderIndexed\n");
    printList(orderIndexed);
    gT = transposeGraph(g, orderIndexed);
    printf("\ngT\n");
    printGraph(gT);
    printf("\nDFS\n");
    vector<int> π = vector<int>(v + 1, 0);
    π = DFS(gT, &order, &orderIndexed);
    append2Root(&π);
    printf("\nappend2Root\n");
    printList(π);
    graphRoot(π);
    printf("\ngraphRoot\n");
    printGraph(g);
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
        printf("Order:\n");
        printList(orderCopy1);
        printList(orderCopy2);
        π = DFS(g, &order, &orderCopy3);
        for (int k = 1; k < π.size(); k++) {
            int l = 0;
            int j = k;
            for (; j != π[j]; j = π[j])
                l++;
            if (l > max)
                max = l;
        }
        order = orderCopy1;
        orderCopy3 = orderCopy2;
        swap(&order, v - i, v);
        swap(&orderCopy3, 1, i + 1);
    }
    printf("max -> %d\n", max);

}
