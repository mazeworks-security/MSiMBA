from functools import wraps
from inspect import getcallargs
import json
import hashlib

def memoize(f):
    '''
    Decorator to memoize a function.
    '''

    memdict = f.memdict = {}

    @wraps(f)
    def g(*args, **kwargs):

        callargs = getcallargs(f, *args, **kwargs)
        hd = json.dumps(callargs, sort_keys=True, default=repr)

        if len(hd) >= 64:
            # Save space by computing cryptographic hash:
            hd = hashlib.sha256(hd.encode('utf-8')).hexdigest()

        if hd in memdict:
            return memdict[hd]
        else:
            val = f(*args, **kwargs)
            memdict[hd] = val
            return val

    return g

@memoize
def ubtrees(n): 
    '''
    Recursively compute all isomorphism classes of unlabelled
    unary-binary trees. The trees are represented as lists of trees
    (recursively!).
    '''
    if n == 1: 
        return [[]] 
    y = [[a] for a in ubtrees(n-1)] 
    if (n & 1): 
        z = ubtrees(n // 2) 
        for i in range(len(z)): 
            for j in range(i, len(z)): 
                y.append([z[i], z[j]]) 
    for k in range(1, n // 2): 
        z = ubtrees(k) 
        w = ubtrees(n - 1 - k) 
        y += [[a, b] for a in z for b in w] 
    return y

def tree2dag(t):
    '''
    Convert a tree represented as a recursive list of trees to a
    representation which generalises to arbitrary digraphs.
    '''

    parents = []
    def digest(subtree, lp):
        parents.append(lp)
        x = len(parents) - 1
        for s in subtree:
            digest(s, [x])
    digest(t, [])
    digraph = [(x, []) for x in parents]
    for (i, l) in enumerate(parents):
        for j in l:
            digraph[j][1].append(i)

    return tuple([(tuple(x), tuple(y)) for (x, y) in digraph])

def get_adoptees(t, idx):

    if len(t[idx][1]) >= 2:
        return []

    inad = set()

    def remove_ancestors(i):
        if i not in inad:
            inad.add(i)
            for j in t[i][0]:
                remove_ancestors(j)

    remove_ancestors(idx)

    for k in range(4):
        for j in t[idx][k & 1]:
            inad.add(j)
            for i in t[j][k >> 1]:
                inad.add(i)

    return [x for x in range(len(t)) if x not in inad]

def adopt(t, idx, jdx):

    t = list(t)
    x, y = t[idx]
    t[idx] = x, tuple(sorted((y + (jdx,))))
    x, y = t[jdx]
    t[jdx] = tuple(sorted((x + (idx,)))), y
    return min_toposort(t)

def dag_successors(dags):
    '''
    Given a set of triangle-free directed acyclic graphs, produce all
    isomorphism classes of triangle-free directed acyclic graphs obtained
    by adding an edge.
    '''

    dags2 = set()

    for t in dags:
        for i in range(len(t)):
            for j in get_adoptees(t, i):
                dags2.add(adopt(t, i, j))

    return dags2

def permute_dag(digraph, sorder):
    '''
    Reorder the vertices of a DAG according to a specified permutation.
    '''

    vorder = [0 for _ in sorder]
    for (i, j) in enumerate(sorder):
        vorder[j] = i

    pp = [(tuple(sorted([vorder[i] for i in x])), tuple(sorted([vorder[i] for i in y]))) for (x, y) in digraph]
    return tuple([pp[j] for j in sorder])

def one_toposort(t):
    '''
    Reorder the vertices of a DAG to ensure it is topologically sorted. A
    ValueError is raised if this is impossible, which occurs if and only if
    the original digraph contains a directed cycle.
    '''

    t = [(sorted(x), sorted(y)) for (x, y) in t]
    sorder = []
    used = set()

    for _ in range(len(t)):
        for (i, (p, c)) in enumerate(t):
            if i in used:
                continue
            q = [x for x in p if x not in used]
            if len(q) == 0:
                used.add(i)
                sorder.append(i)
        if len(sorder) == len(t):
            break
    else:
        raise ValueError("%s is not a directed acyclic graph" % t)

    return permute_dag(t, sorder)

def min_toposort(dag):
    '''
    Find a canonical topological ordering of a directed acyclic graph.
    We use Algorithm V from Knuth 7.2.1.2 to iterate over all topological
    sorts, and take the minimal one (under the Python ordering).
    '''

    n = len(dag)
    dag = one_toposort(dag)
    mindag = [dag]

    def recurse_or_cmpxchg(porder):

        m = len(porder)
        if m == n:
            newdag = permute_dag(dag, porder)
            if newdag < mindag[0]:
                mindag[0] = newdag
            return

        recurse_or_cmpxchg(porder + [m])

        for i in range(m):
            if porder[m-i-1] in dag[m][0]:
                break
            recurse_or_cmpxchg(porder[:m-i-1] + [m] + porder[m-i-1:])

    recurse_or_cmpxchg([0])
    return mindag[0]

def trifree_dags(n):
    '''
    Compute all isomorphism classes of triangle-free DAGs on n nodes,
    subject to the condition that each node has indegree <= 2 and
    there is a unique maximal (outdegree 0) node. In particular, there
    are a total of:

    1 DAG with 1 node (o);
    1 DAG with 2 nodes (o --> o);
    2 DAGs with 3 nodes (o --> o <-- o and o --> o --> o);
    5 DAGs with 4 nodes (3 of which are trees);
    18 DAGs with 5 nodes (6 of which are trees);
    81 DAGs with 6 nodes (11 of which are trees);
    475 DAGs with 7 nodes (23 of which are trees);
    3394 DAGs with 8 nodes (46 of which are trees);
    29140 DAGs with 9 nodes (98 of which are trees);
    293198 DAGs with 10 nodes (207 of which are trees);

    These are enumerated by constructing all trees and sequentially
    adding edges, removing duplicate (isomorphic) DAGs in the process.

    Observe that any Boolean circuit of 2-input gates can be optimised
    to a triangle-free DAG without increasing the gate count or depth;
    for this reason, it suffices to restrict attention to these graphs.
    '''
    # TODO submit this as sequence A308634 to the OEIS
    dags = [tree2dag(t) for t in ubtrees(n)]
    all_dags = []
    while len(dags) > 0:
        all_dags += list(dags)
        dags = dag_successors(dags)
    return all_dags

def main(n):

    dags = trifree_dags(n)
    perm = list(range(n))[::-1]
    for d in dags:

        for x in d:
            if len(x[1]) == 2:
                i, j = x[1]
                if (len(d[i][1]) == 2) and (d[j][1] == d[i][1]):
                    # Subgraph of the shape K2,3:
                    break
        else:
            # No triangles or K2,3 subgraphs:
            d = permute_dag(d, perm)
            s = ''.join([str(x) for t in d for x in ((' ',) + t[1] + ('*', '*'))[:3]])
            print(s)

if __name__ == '__main__':

    from sys import argv
    seq = [int(x) for x in argv[1:]]
    for s in seq:
        main(s)
