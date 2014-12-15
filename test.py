#!/usr/bin/env python3
import hashlib
import networkx as nx
import random
import subprocess
import time

#with open("test_out", "a") as fh:
#    fh.write("|S|\t|A|\tTime")

def h(x):
    return hashlib.sha224(str(x).encode('utf-8')).hexdigest()

for k in range(100):
    ident = h(time.time())
    print("Test "+str(ident))
    n = random.randint(1000, 10000)
    p = random.uniform(0, 0.5)
    G = nx.fast_gnp_random_graph(n, p, directed=True)
    out = str(n)+" "+str(len(G.edges()))+"\n"
    print(out.strip())
    for (i,j) in G.edges():
        out += str(i)+" "+str(j)+" "+str(random.randint(1, 100))+"\n"
    with open("tests/random/"+str(ident), "w") as fh:
        fh.write(out)
    start = time.time()
    out = subprocess.check_output(["./dijkstra_fib", "tests/random/"+str(ident)])
    duration = time.time() - start
    print(str(duration)+"s")
    with open("tests_outputs/random/"+str(ident), "wb") as fh:
        fh.write(out)
    print()

    with open("test_out", "a") as fh:
        fh.write("\n"+str(n)+"\t"+str(len(G.edges()))+"\t"+str(duration))
