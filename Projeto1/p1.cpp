#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

int x, y;
vector<vector<int>> price;
vector<vector<int>> maxPrice;
int minRows, minColumns;
int max = 0;

void parseDimensions() {
    scanf("%d %d", &x, &y);
    price = vector<vector<int>>(x, vector<int>(y, 0));
    maxPrice = vector<vector<int>>(x, vector<int>(y, -1));
    minRows = x, minColumns = y;
}

void parsePrice() {
    int n = 0;
    scanf("%d", &n);
    for (int k = 0; k < n; k++) {
        int i, j, p;
        scanf("%d %d %d", &i, &j, &p);
        price[i - 1][j - 1] = p;
        // if (minRows > i)
            minRows = i;
        if (minColumns > j)
            minColumns = j;
    }
}

int computeMaxValue(int m, int n) {
    // if (m < minRows || n < minColumns)
    //     maxPrice[m - 1][n - 1] = 0;
    if (maxPrice[m - 1][n - 1] >= 0)
        return maxPrice[m - 1][n - 1];
    int maxValue = price[m - 1][n - 1];
    for (int i = 1; i <= m/2; i++) {
        int value = computeMaxValue(i, n) + computeMaxValue(m - i, n);
        if (maxValue < value)
            maxValue = value;
    }
    for (int j = 1; j <= n/2; j++) {
        int value = computeMaxValue(m, j) + computeMaxValue(m, n - j);
        if (maxValue < value)
            maxValue = value;
    }
    maxPrice[m - 1][n - 1] = maxValue;
    return maxValue;
}

void printMaxValue() {
    printf("%d\n", computeMaxValue(x, y));
}

int main() {
    auto start = chrono::high_resolution_clock::now();

    parseDimensions();
    parsePrice();
    printMaxValue();

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    return 0;
}
