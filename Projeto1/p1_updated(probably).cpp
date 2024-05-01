#include <iostream>
#include <vector>
using namespace std;

int x, y;
vector<vector<int>> v;

void parseDimensions() {
    scanf("%d %d", &x, &y);
    v = vector<vector<int>>(x + 1, vector<int>(y + 1, 0));
}

void parsePrice() {
    int n = 0;
    scanf("%d", &n);
    for (int k = 0; k < n; k++) {
        int i, j, p;
        scanf("%d %d %d", &i, &j, &p);
        if (i <= x && j <= y) {
            v[i][j] = p;
            if (j <= x && i <= y)
                v[j][i] = p;
        }
    }
}

void computeMaxValue() {
    for (int i = 1; i <= x; i++)
        for (int j = 1; j <= y; j++) {
            for (int k = 1; k < i/2 + 1; k++)
                v[i][j] = max(v[i][j], v[k][j] + v[i - k][j]);
            for (int k = 1; k < j/2 + 1; k++)
                v[i][j] = max(v[i][j], v[i][k] + v[i][j - k]);
        }
}

int main() {

    parseDimensions();
    parsePrice();
    computeMaxValue();
    printf("%d\n", v[x][y]);

    return 0;
}
