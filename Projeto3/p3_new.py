from pulp import *

prob = LpProblem("The_UbiquintyInc_problem", LpMaximize)

def print_matrix(matrix):
    for row in matrix:
        print(row)

t, p, m = map(int, input().split())

A = [[] for _ in range(t + 1)]
x = []
b = []
c = []

for i in range(t):
    profit, max_prod = map(int, input().split())
    x.append(LpVariable(f"x{i}", 0, max_prod))
    A[i].append(i)
    A[t].append(1)
    b.append(max_prod)
    c.append(profit)

for j in range(1, p + 1):
    xi, xj, xk, profit = map(int, input().split())
    x.append(LpVariable(f"x{i + j}", 0, min(b[xi - 1], b[xj - 1], b[xk - 1])))
    A[xi - 1].append(i + j)
    A[xj - 1].append(i + j)
    A[xk - 1].append(i + j)
    A[t].append(3)
    c.append(profit)

b.append(m)

prob += lpSum(c[i] * x[i] for i in range(t + p))

for i in range(t):
    prob += lpSum(x[j] for j in A[i]) <= b[i]

prob += lpSum(A[t][j] * x[j] for j in range(t + p)) <= b[t]

prob.solve(GLPK(msg=0))

print(int(prob.objective.value()))