# CF #427 (Div. 2) - Problem F

## Problem Description [(Link)](http://codeforces.com/contest/835/problem/F)

time limit per test: 2 seconds

memory limit per test: 256 megabytes

input: standard input

output: standard output

In the Kingdom K., there are n towns numbered with integers from 1 to n. The towns are connected by n bi-directional roads numbered with integers from 1 to n. The i-th road connects the towns ui and vi and its length is li. There is no more than one road between two towns. Also, there are no roads that connect the towns with itself.

Let's call the inconvenience of the roads the maximum of the shortest distances between all pairs of towns.

Because of lack of money, it was decided to close down one of the roads so that after its removal it is still possible to reach any town from any other. You have to find the minimum possible inconvenience of the roads after closing down one of the roads.

### Input

The first line contains the integer n (3 ≤ n ≤ 2·10^5) — the number of towns and roads.

The next n lines contain the roads description. The i-th from these lines contains three integers ui, vi, li (1 ≤ ui, vi ≤ n, 1 ≤ li ≤ 10^9) — the numbers of towns connected by the i-th road and the length of the i-th road. No road connects a town to itself, no two roads connect the same towns.

It's guaranteed that it's always possible to close down one of the roads so that all the towns are still reachable from each other.

### Output
Print a single integer — the minimum possible inconvenience of the roads after the refusal from one of the roads.

### Examples
<br/>

| input | 
| --- |
| 3 <br/> 1 2 4 <br/> 2 3 5 <br/> 1 3 1 <br/> |
| **output** |
| 5 |

<br/>

| input | 
| --- |
| 5 <br/> 2 3 7 <br/> 3 1 9 <br/> 4 1 8 <br/> 3 5 4 <br/> 4 5 5 |
| **output** |
| 18 |

## Solution

### Prerequisites

It's important to know the following contents to completely understand the solution:

* Basic graph theory: the solution covers contents such as graph traversal (DFS or BFS), trees, cycle detection.
* Segment Tree: it's a data structure which allows, among other applications, to retrieve the greatest element in a continous interval and to update values in a sub-linear time, O(log n).

### Proposed Solution

The first part when you are solving a problem is to completely understand what it wants and then, to create/colect the maximum of useful abstractions related to that task.

The problem wants to know what the best choice to remove an edge that leads into a minimum simple path from the maximum among all possible simple paths.

The input graph contains n vertices and n edges, every graph which contains exactly n vertices and n-1 edges is a tree and it connects all its vertices and contains no cycles, then the input is a tree with one more edge. If we add an edge into a tree, we are creating a cycle with some subset of vertices from that tree.

We can claim that the erased edge belongs to the cycle, otherwise the resulting graph is not any more connected. 

The solution can be split in two cases. In the first case, the greatest path does not pass through any edge in the cycle. The second case is the opposite of first case. We can deal these two cases independentily because the erased edge does not affect the first case cost. So, the result is the maximum value between both cases.

#### First Case 

Find the longest simple path in a tree is a well known problem and this path is known as tree diameter.

The first step to find the tree diameter is to idenitfy one of the final vertices of this diameter, and the algorithm is pretty simple: choose some vertex in the graph and run some traversal algorithm such as DFS to find the most distant vertex from it.

We can show that the initial vertex choice is not important and we are going to discuss about that.

Let v be the chosen vertex and sp, the simple path that represents the tree diameter. Now, suppose that h is the shortest path from v to some vertex w in sp. Then, we can split sp in two parts, divided by the vertex w, sp1 and sp2, where sp1 length is less than sp2 length. If we apply that algorithm then we will find a path with length l at least (sp2 + h). If l is greater than that, we can easily construct a greater diameter which has a length at least (l - h + sp1), what it is impossible. Otherwise, we can construct another diameter with the same length, what is not a problem. 

![tree example](images/graph-first-case.png?raw=true "Tree Example")

Once we have one of the diameter endpoints, we run this algorithm again and get the other endpoint.

So, it's necessary to ignore all the edges that belongs to the cycle and apply the above algorithm to each formed tree.

#### Second Case 

Once we know the vertices which belong to the cycle, let's represent them by (c1, c2, ..., cm), the initial step is to know for each vertex which is the maximum depth of its own tree, it's possible to find it through the previous DFS.

Now, we use a segment tree (ST) where each position represents its own tree depth minus the vertex distance to c1. Because when substract ST[x] - ST[y] + 2 * depth(y), we get: (depth(x) - dist(x, c1)) - (depth(y)- dist(y, c1)) + 2 * depth(y) = depth(y) + depth (x) + dist (y, x), i.e., the maximum path which starts in some node of x and finishes in some node of y, it will be further referred as solution_cost (x, y). So, if we want to know the maximum path which finishes in y but starts in some point between x and y-1, it will be referred as maximum_path (x, y), our query should be something like ST(x, y-1) + ST(y,y) + 2 * depth(y) -- where ST(a, b) represents the maximum ST value for positions between a and b.

A important observation is when for some pair of indices (x, y), every vertex a less than x and y, the solution_cost (a, x) - solution_cost (a, y) is always constant. Then, if we suppose maximum_path (a, x) is better than maximum_path (a, y), this situation will only change when a is greater enough that maximum_path (x, y) will become greater than maximum_path (a, x). And it is important because supposing that a is always increasing, once maximum_path (a, y) become greater than maximum_path (a, x), the opposite won't happen anymore.

In this way, if we store a sequence of right endpoints (r[1], r[2], ... r[k]) where maximum_path (r[i-2], r[i-1]) > maximum_path (r[i-2], r[i]), for all i greater than 2 and less or equal than k, for i iquals to 1, maximum_path (a, r[i-1]) > maximum_path (a, r[i]), where a represents the leftmost vertex, i.e., the first vertex after the removed edge. And if we have a situation where maximum_path (r[i-2], r[i-1]) <= maximum_path (r[i-2], r[i]), we can just remove r[i-1], while the condititon discribed above is not fulfilled. In the end, we will have something like:

![Invariant example](images/chart.png?raw=true "Invariant example")

So, it's easy to see that if this condition above holds, the sequence solution only decreases, and if we shift the removed edge to the next one, the a increases one position and possibly we need to erase some front elements from our sequence r and possibly have to add a new endpoint in the edge of the sequence. Then, we just need to front elements to check the best answer to that setting, and if we do n shifts, we just to need no more than n add, erase and check operations, where which one those operations cost at maximum O (log n). Therefore, we can hava a overall O (n * log n) time complixity.

Initially, we ignore the last edge (cm, c1), so we just consider the left endpoints from 1 to m. Then, we add all right endpoints from 1 to m into our sequence and we calculate the result as discribed above. After that, we make a right shift, i.e, ignore the first left endpoint (c1) and add a new one (cm+1) right endpoint into the sequence and possibly erasing others. And we keep doing that until we erase all the possible edges.