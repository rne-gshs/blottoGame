import sys
import time

input = lambda : sys.stdin.readline().rstrip()
ip = lambda : int(input())
ips = lambda : input().split()
ipm = lambda : map(int, ips())
ipl = lambda : list(ipm())
sys.setrecursionlimit(100000)

import numpy as np
import random as rn


def f(n, k):
    #n, k = ipm()
    A = np.array(np.random.default_rng().dirichlet(np.ones(n), size=1), dtype=np.longdouble)
    B = [round(i*k) for i in A[0]]
    #print(B)
    cnt = sum(B)
    if k < cnt:
        for i in range(cnt - k):
            a = rn.randint(0, n-1)
            while B[a] <= 0:
                a = rn.randint(0, n-1)

            B[a] -= 1
    elif cnt < k:
        for i in range(k - cnt):
            B[rn.randint(0, n-1)] += 1

    #print(*B)
    #print(f"{sum(B)}")

    if sum(B) != k:
        print()
        print(*B)
        print(f"err sum {sum(B)} : {k}")
        exit()

    for i in range(n):
        if B[i] < 0:
            print()
            print(*B)
            print(f"err {i} : {B[i]}")
            exit()

    #print(*B)
    return B

A = [0 for i in range(21)]
#t = 0
for i in range(10000):
    k = f(5, 20)
    for i in k:
        A[i] += 1
    #t += 1
    #print(f"\r{t}", end="")
    #time.sleep(0.3)

print(*A)
