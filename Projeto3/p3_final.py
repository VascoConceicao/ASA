from pulp import *
import time

start_time = time.time()

prob = LpProblem("The_UbiquintyInc_problem", LpMaximize)

n, p, m = map(int, input().split())

A = [[] for _ in range(n + 1)]
x = []
b = []
c = []

for i in range(n):
    profit, max_prod = map(int, input().split())
    x.append(LpVariable(f"x{i + 1}", 0, max_prod, LpInteger))
    A[i].append(x[i])
    A[n].append(x[i])
    b.append(max_prod)
    c.append(profit)

for i in range(n, n + p):
    xi, xj, xk, profit = map(int, input().split())
    x.append(LpVariable(f"x{i + 1}", 0, min(b[xi - 1], b[xj - 1], b[xk - 1]), LpInteger))
    A[xi - 1].append(x[i])
    A[xj - 1].append(x[i])
    A[xk - 1].append(x[i])
    A[n].append(3 * x[i])
    c.append(profit)

b.append(m)

prob += lpSum(c[i] * x[i] for i in range(n + p))

for i in range(n + 1):
    prob += lpSum(A[i]) <= b[i]

prob.solve(GLPK(msg=0))

print(prob.objective.value())

end_time = time.time()
elapsed_time_ms = (end_time - start_time) * 1000
print("Execution time: {:.0f} milliseconds".format(elapsed_time_ms))