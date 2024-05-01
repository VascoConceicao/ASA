#include <iostream>
#include <vector>
using namespace std;

int main() {
    int x, y;
    vector<vector<int>> maxValue;
    scanf("%d %d", &x, &y);
    maxValue = vector<vector<int>>(x + 1, vector<int>(y + 1, 0));
    int n = 0;
    scanf("%d", &n);
    for (int k = 0; k < n; k++) {
        int i, j, p;
        scanf("%d %d %d", &i, &j, &p);
        if (i <= x && j <= y) {
            maxValue[i][j] = p;
            if (j <= x && i <= y)
                maxValue[j][i] = p;
        }
    }
    for (int i = 1; i <= x; i++)
        for (int j = 1; j <= y; j++) {
            for (int k = 1; k < i; k++)
                maxValue[i][j] = max(maxValue[i][j], maxValue[k][j] + maxValue[i - k][j]);
            for (int k = 1; k < j/2 + 1; k++)
                maxValue[i][j] = max(maxValue[i][j], maxValue[i][k] + maxValue[i][j - k]);
        }
    printf("%d\n", maxValue[x][y]);
    return 0;
}

// TRIIIIINTA CARALHOOOO