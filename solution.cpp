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

/**************************************** Segment Tree Code ****************************************************/

template <typename nodetype>
nodetype merge_nodes (const nodetype &X, const nodetype &Y);
 
template <typename lazytype>
lazytype accumulate_updates (int a, int b, int l, int r, const lazytype &lt1, const lazytype &lt2);
 
template <typename nodetype, typename lazytype>
nodetype update_node_info (int a, int b, int l, int r, const nodetype &nt, const lazytype &lt);
 
// LazyValue de um no x carrega a informacao que deve ser atualizada na sub-arvore de (x), nao incluindo o no (x).
template <typename nodetype, typename lazytype>
class LazyValue {
private:
    lazytype value;
    bool empty;
 
public:
    // Inicializa o LazyValue com vazio.
    LazyValue () {  empty = true; }
     
    // Inicializa o LazyValue com o valor (x).
    LazyValue (lazytype x) { empty = false, value = x;  }
     
    // Verifica se existe alguma atualizacao a ser feita.
    bool is_empty() { return empty; }
     
    // Acumula a informacao de uma atualizacao com uma nova.
    LazyValue accumulate (int a, int b, int l, int r, const LazyValue &lv) {
        if (empty) return lv; //lv nunca estara vazio.
        return LazyValue( accumulate_updates (a, b, l, r, this->value, lv.value) );
    }
     
    // Atualiza o valor do no (nd) com base na atualizacao propagada pelos antecessores.
    nodetype update_node (int a, int b, int l, int r, const nodetype &nd) {
        if (empty) return nd;
        return update_node_info (a, b, l, r, nd, this->value);
    }
};
 
template <typename nodetype, typename lazytype>
class SegTree {
private:
    int N;
    nodetype *segTree;
    LazyValue<nodetype, lazytype> *lazy;
      
    // Atualiza o valor do no (pos) e depois atualiza o lazy de (pos)
    void change (int pos, int a, int b, int l, int r, LazyValue<nodetype, lazytype> lv) { 
        segTree[pos] = lv.update_node (a, b, l, r, segTree[pos]);
        lazy[pos] = lazy[pos].accumulate (a, b, l, r, lv);
    }
  
    void to_propagate (int pos, int a, int b, int l, int r) {
        if (!lazy[pos].is_empty()) {
            int mid = (a+b)>>1;
            change (2*pos, a, mid, l, r, lazy[pos]);
            change (2*pos+1, mid+1, b, l, r, lazy[pos]);
            lazy[pos] = LazyValue<nodetype, lazytype>();
        }
    }
  
    nodetype st_search (int pos, int a, int b, int l, int r) {
        if (l<=a && b<=r) {
            return segTree[pos];
        }
        int med = (a+b)>>1; 
        to_propagate (pos, a, b, l, r);
        if (l<=med && r>med) return merge_nodes (st_search (2*pos, a, med, l, r), st_search (2*pos+1, med+1, b, l, r));
        if (r<=med) return st_search (2*pos, a, med, l, r);
        if (l >med) return st_search (2*pos+1, med+1, b, l, r);
    }
  
    void st_update (int pos, int a, int b, int l, int r, const LazyValue<nodetype, lazytype> &lv) {
        if (l<=a && b<=r) {
            return change (pos, a, b, l, r, lv);
        }
        int med = (a+b)>>1;
        to_propagate (pos, a, b, l, r);
        if (l<=med) st_update (2*pos, a, med, l, r, lv);
        if (r>med)  st_update (2*pos+1, med+1, b, l, r, lv);
        segTree[pos] = merge_nodes (segTree[2*pos], segTree[2*pos+1]);
    }
  
    void init (nodetype *v, int pos, int a, int b) {
        if (a==b) {
            segTree[pos] = v[a];
        } else {
            int med = (a+b)>>1;
            init (v, 2*pos, a, med);
            init (v, 2*pos+1, med+1, b);
            segTree[pos] = merge_nodes(segTree[2*pos], segTree[2*pos+1]);
        }
    }
  
public:
  
    SegTree (int _n, nodetype *v) {
        N = _n;
        segTree = new nodetype[4*N];
        lazy    = new LazyValue<nodetype, lazytype>[4*N];
        init (v, 1, 0, N-1);
    }
  
    SegTree (int _n, const nodetype &v) {
        N = _n;
        segTree = new nodetype[4*N];
        lazy    = new LazyValue<nodetype, lazytype>[4*N];
         
        nodetype *tmp = new nodetype[N];
        for (int i = 0; i < N; i++) tmp[i] = v;
        init (tmp, 1, 0, N-1);
        delete []tmp;
    }
  
    ~SegTree () {
        delete []segTree;
        delete []lazy;
    }
  
    nodetype search (int l, int r) { return st_search (1, 0, N-1, l, r);  }
  
    void update (int l, int r, const lazytype &lv) { st_update (1, 0, N-1, l, r, LazyValue<nodetype, lazytype>(lv)); }
};


/****************************************** Problem Specific ***********************************************************/

template <typename nodetype>
nodetype merge_nodes (const nodetype &x, const nodetype &y) {
	return max(x, y);
}

template <typename lazytype>
lazytype accumulate_updates (int a, int b, int l, int r, const lazytype &lt1, const lazytype &lt2){
	return lt1 + lt2;
}

template <typename nodetype, typename lazytype>
nodetype update_node_info (int a, int b, int l, int r, const nodetype &nt, const lazytype &lt) {
	return nt + lt;
}

/******************************************* End of ST Code *************************************************************/

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

// Compute the maximum path which finishes in some vertex of (x+1)th vertex in cycle and
// starts at any vertex of a vertex not before the (L+1)th vertex in the cycle.
long long find_best (SegTree<long long, long long> &st, int L, int x, int n) {
    if (L==x) return vertex_depth[vertices_in_cycle[x%n]];
    long long bst = st.search (L, x-1);
    bst -= st.search (x, x);
    bst += 2*vertex_depth[vertices_in_cycle[x%n]];
    return bst;
}

// Compute the answer when it passes through the cycle.
long long solve_cycle () {
    int n = sz(vertices_in_cycle);
    long long resp = 0, path_len = 0;

    SegTree<long long, long long> st (2*n, 0LL);
    deque<int> candidates;

    FORR (i, n) {
        int a = vertices_in_cycle[i], b = vertices_in_cycle[(i+1)%n];
        st.update (i, i, vertex_depth[a] - path_len);

        //PUSH BACK
        while ((sz(candidates) > 0) && 
            (find_best(st, 0, i, n) > find_best(st, 0, candidates.back(), n))) {
            candidates.pop_back();
        }
        candidates.push_back (i);

        path_len += get_cost(a, b);
    }
    //Compute the answer after ignoring/removing the first edge.
    resp = find_best(st, 0, candidates.front(), n);

    FOR (i, 1, n) {
        int j = i+n-1;
        int a = vertices_in_cycle[(j-1)%n], b = vertices_in_cycle[(j)%n], c = vertices_in_cycle[(j+1)%n];

        st.update (j, j, vertex_depth[b] - path_len);

        //POP FRONT
        while ( (sz(candidates) > 0 && candidates.front() <= i) ||
                (sz(candidates) > 1 && 
                    find_best(st, i, candidates[0], n) < find_best(st, i, candidates[1], n))) {
            candidates.pop_front();
        }

        //PUSH BACK
        while ((sz(candidates) == 1) && (find_best(st, i, j, n) > find_best(st, i, candidates.back(), n))
        || ((sz(candidates) > 1) && (find_best(st, candidates[sz(candidates)-2], j, n) > find_best(st, candidates[sz(candidates)-2], candidates.back(), n)))) {
            candidates.pop_back();
        }
        candidates.push_back (j);

        path_len += get_cost(b, c);

        //Compute the answer after ignoring/removing the (i+1)th edge.
        resp = min (resp, find_best(st, i, candidates.front(), n));
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