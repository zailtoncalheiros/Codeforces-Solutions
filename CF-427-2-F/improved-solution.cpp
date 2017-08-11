#include <bits/stdc++.h>

using namespace std;

#define FOR(i,a,b) for(int i = a; i < b; i++)
#define FORR(i,b) FOR(i, 0, b)
#define sz(e) (int)e.size()
#define CLR(x,v) memset (x, v, sizeof x)
#define pb push_back
#define all(e) e.begin(), e.end()

typedef long long ll;
typedef pair<ll, ll> ii;
typedef vector<int> vi;

#define PI acos(-1)

const int INF = 1000000000;
const int MAXN = 200005;
const ll LINF = 1LL * INF * INF;

vector< pair<int, int> > adj[MAXN];

long long vertex_depth[MAXN];
bool is_cycle[MAXN];
vector<int> vertices_in_cycle;

map<int, int> madj[MAXN];
// Returns the cost of the edge (i, j).
int get_cost (int i, int j) {    
    return madj[i][j];
}

ll prefix[2*MAXN], suffix[2*MAXN];

// Get the path length from branch x to the branch y.
ll get_resp (int x, int y, int n) {
    return prefix[y] - prefix[x] + 2 * vertex_depth[vertices_in_cycle[x % n]];
}

// Compute the answer when it passes through the cycle.
ll solve_cycle() {
    int n = sz(vertices_in_cycle);
    ll path_len = 0;

    FORR (i, 2*n) {
        int a = vertices_in_cycle[(i)%n], b = vertices_in_cycle[(i+1)%n];

        prefix[i] = vertex_depth[a] + path_len;
        suffix[i] = vertex_depth[a] - path_len;

        path_len += get_cost (a, b);
    }

    deque<int> d_1, d_2;
    d_1.push_back (0);
    d_2.push_back (1);
    FOR (i, 2, n) {
        int d2f = d_2.front();
        while (!d_2.empty() && prefix[d_2.back()] < prefix[i]) {
            d_2.pop_back();
        }

        d_2.push_back(i);
        if (sz(d_2) > 1 && get_resp (d_1.front(), d_2.front(), n) < get_resp (d_2.front(), d_2[1], n)) {
            d_2.pop_front();
        }
        FOR (j, d2f, d_2.front()) {
            while (!d_1.empty() && suffix[d_1.back()] < suffix[j]) {
                d_1.pop_back();
            }
            d_1.push_back (j);
        }
    }

    ll resp = get_resp(d_1.front(), d_2.front(), n);

    FOR (i, n, 2*n) {
        int k = i-n;
        if (k >= d_1.front()) {
            d_1.pop_front();
        } 

        int d2f = d_2.front();
        if (d_1.empty()) {
            d_1.push_back (d_2.front());
            d_2.pop_front();
            d2f++;
        }
        
        while (!d_2.empty() && prefix[d_2.back()] < prefix[i]) {
            d_2.pop_back();
        }
        d_2.push_back(i);
        if (sz(d_2) > 1 && get_resp (d_1.front(), d_2.front(), n) < get_resp (d_2.front(), d_2[1], n)) {
            d_2.pop_front();
        }
        FOR (j, d2f, d_2.front()) {
            while (!d_1.empty() && suffix[d_1.back()] < suffix[j]) {
                d_1.pop_back();
            }
            d_1.push_back (j);
        }
        resp = min (resp, get_resp(d_1.front(), d_2.front(), n));
    }

    return resp;
}

int flag[MAXN];
// Find the vertices which belongs to the cycle.
bool find_cycle (int v, int p) {
    flag[v] = 1;
    bool resp = false;
    FORR (i, sz(adj[v])) {
        int k = adj[v][i].first;
        if (flag[k]==2 || k==p) continue;
        if (flag[k]==1){
            flag[k] = 3;
            madj[v][k] = madj[k][v] = adj[v][i].second;
            resp = true;
        } else if (flag[k] == 0) {
            bool aux = find_cycle(k, v);
            if (aux) {
                madj[v][k] = madj[k][v] = adj[v][i].second;
                resp = true;
            }
        }
    }
    if (resp) {
        is_cycle[v] = 1;
        vertices_in_cycle.pb(v);
    }
    if (flag[v]==3) return false;
    return resp;
}

// Returns (farthest vertex, distance) from the vertex of the initial call,
// the function receives as paramenters (vertex, its parent).
pair<int, long long> find_deepest (int v, int p) {
    pair<int, long long> ans = make_pair (v, 0);
    FORR (i, sz(adj[v])) {
        int k = adj[v][i].first;
        if ((is_cycle[v] && is_cycle[k]) || k==p) continue;
        pair<int, long long> tmp = find_deepest (k, v);
        tmp.second +=  adj[v][i].second;
        if (tmp.second > ans.second) ans = tmp;
    }
    return ans;
}

// Compute the final result.
long long solve() {
    find_cycle(0, 0);
    int T = sz(vertices_in_cycle);

    long long ans = 0;
    FORR (i, T) {
        int w = vertices_in_cycle[i];
        // Find the depth to each vertex which belongs to the cycle.
        pair<int, long long> tmp = find_deepest(w, w);
        vertex_depth[w] = tmp.second;

        // Find the greatest paths which path does not pass through any edge in cycle.
        pair<int, long long> aux = find_deepest(tmp.first, tmp.first);
        ans = max (aux.second, ans);
    }

    ans = max (ans, solve_cycle());
    return ans;
}

int main () {
    ios_base::sync_with_stdio(false);
	cin.tie(NULL);

    int n;
    cin>>n;
    FORR (i, n) {
        int a, b, c;
        cin>>a>>b>>c; a--, b--;
        adj[a].pb (make_pair(b, c));
        adj[b].pb (make_pair(a, c));
    }

    cout << solve() << endl;
}