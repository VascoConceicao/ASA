from pulp import *

prob = LpProblem("The_UbiquintyInc_problem", LpMaximize)

def print_matrix(matrix):
    for row in matrix:
        print(row)

t, p, m = map(int, input().split())

print(f"t -> {t}, p -> {p}, m -> {m}")

A = [[] for _ in range(t + 1)]
x = []
b = []
c = []

for i in range(t):
    profit, max_prod = map(int, input().split())
    x.append(LpVariable(f"x{i}", 0, max_prod))
    A[i].append(x[i])
    A[t].append(xvar)
    b.append(max_prod)
    c.append(profit)

for j in range(1, p + 1):
    xi, xj, xk, profit = map(int, input().split())
    xvar = LpVariable(f"x{i + j}", 0, min(b[xi - 1], b[xj - 1], b[xk - 1]))
    x.append(xvar)
    print(xi, xj, xk)
    A[xi - 1].append(xvar)
    A[xj - 1].append(xvar)
    A[xk - 1].append(xvar)
    A[t].append(xvar)
    A[t].append(xvar)
    A[t].append(xvar)
    c.append(profit)

b.append(m)

print("Matriz A:")
print_matrix(A)

print("Matriz B:")
print(b)

print("Matriz C:")
print(c)

print("Vetor X:")
print(x)

prob += lpSum(c[i] * x[i] for i in range(t + p))

for i in range(t + 1):
    prob += lpSum(A[i]) <= b[i]

prob.solve(GLPK(msg=0))

print("Status:", prob.status)
print("Objective value:", prob.objective.value())
print("Values of decision variables:")
print(x)
for i, var in enumerate(x):
    print(f"{var.name} =", var.value())

print(int(prob.objective.value()))