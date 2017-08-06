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