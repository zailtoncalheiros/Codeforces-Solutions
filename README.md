# Solving CF #427 (Div. 2) - Problem F

## Problem Description 

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

Let v be the chosen vertex and sp, the simple path that represents the tree diameter. Now, suppose that h is the shortest path from v to some vertex w in sp. Then, we can split sp in two parts, divided by the vertex w, sp1 and sp2, where sp1 length is less than sp2 length. If we apply that algorithm then we will find a path with length l at least (sp2 + h). If l is greater than that, we can easily construct a greater diameter which has a length at least (l - h + sp1), what it is impossible. Otherwise, we can construct another diameter with the same length, what it is not a problem. 

![tree example](images/graph-first-case.png?raw=true "Title")

Once we have one of the diameter endpoints, we run this algorithm again and get the other endpoint.

So, it's necessary to ignore all the edges that belongs to the cycle and apply the above algorithm to each formed tree.

#### Second Case 
