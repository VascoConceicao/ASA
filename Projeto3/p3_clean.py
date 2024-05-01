from pulp import *

prob = LpProblem("The_UbiquintyInc_problem", LpMaximize)

def print_matrix(matrix):
    for row in matrix:
        print(row)

n, p, m = map(int, input().split())

A = [[] for _ in range(n + 1)]
x = [0] * (n + p + 1)
b = [0] * (n + 2)
c = [0] * (n + p + 1)

print("Matriz A:")
print_matrix(A)

print("Matriz B:")
print(b)

print("Matriz C:")
print(c)

print("Vetor X:")
print(x)

for i in range(1, n + 1):
    profit, max_prod = map(int, input().split())
    x[i] = LpVariable(f"x{i}", 0, max_prod, LpInteger)
    A[i].append(x[i])
    A[0].append(x[i])
    b[i] = max_prod
    c[i] = profit

print("Matriz A:")
print_matrix(A)

print("Matriz B:")
print(b)

print("Matriz C:")
print(c)

print("Vetor X:")
print(x)

for i in range(n + 1, n + p + 1):
    xi, xj, xk, profit = map(int, input().split())
    x[i] = LpVariable(f"x{i}", 0, min(b[xi - 1], b[xj - 1], b[xk - 1]), LpInteger)
    A[xi].append(x[i])
    A[xj].append(x[i])
    A[xk].append(x[i])
    A[0].append(3 * x[i])
    c[i] = profit

b[0] = m

print("Matriz A:")
print_matrix(A)

print("Matriz B:")
print(b)

print("Matriz C:")
print(c)

print("Vetor X:")
print(x)

prob += lpSum(c[i] * x[i] for i in range(n + p))

print("Matriz A:")
print_matrix(A)

for i in range(n + 1):
    prob += lpSum(A[i]) <= b[i]

prob.solve(GLPK(msg=0))

print(prob.objective.value())