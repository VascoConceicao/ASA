#include <iostream>
#include <vector>

using namespace std;

int lines = 3, columns = 20;

void visualizar(int lines, int columns) {
    printf("\nlines -> %d, columns -> %d\n", lines, columns);
    if (lines == 0 && columns == 0) return;
    while (lines>0) {
        for (int j=0; j<columns; j++)
            printf("□ ");
        printf("\n");
        lines--;
    }
}

int main() {

    visualizar(lines, columns);

    return 0;
}