#include <iostream>
#include <vector>
#include <queue>

typedef long long ll;
const int LEN = 2e3 + 1;
const ll INF = 1e9;

struct Pos {
    ll x, y;
    bool operator==(const Pos& o) const { return x == o.x && y == o.y; }
    bool operator<(const Pos& o) const { return x < o.x; }
    Pos operator+(const Pos& o) const { return { x + o.x, y + o.y }; }
};
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
    ll c = cross(d1, d2, d3);
    return c > 0 ? 1 : c < 0 ? -1 : 0;
}

class SplayTree {
    struct Node {
        Node* l;
        Node* r;
        Node* p;
        Pos val;
        int size;
        Node(const Pos& p) : l(0), r(0), p(0), val(p), size(1) {}
        ~Node() { if (l) delete l; if (r) delete r; }
        void update() {
            size = 1;
            if (l) size += l->size;
            if (r) size += r->size;
        }
    } *root;
    void rotate(Node* x) {
        Node* p = x->p;
        if (!p) return;
        Node* b = 0;
        if (x == p->l) {
            p->l = b = x->r;
            x->r = p;
        }
        else {
            p->r = b = x->l;
            x->l = p;
        }
        x->p = p->p;
        p->p = x;
        if (b) b->p = p;
        (x->p ? p == x->p->l ? x->p->l : x->p->r : root) = x;
        p->update();
        x->update();
    }
    void splay(Node* x) {
        while (x->p) {
            Node* p = x->p;
            Node* g = p->p;
            if (g) {
                if ((x == p->l) == (p == g->l)) rotate(p);
                else rotate(x);
            }
            rotate(x);
        }
    }
    bool update_convex(Node* x) {
        splay(x);

        // check convex (optimal)
        Node* l = x->l; while (l->r) l = l->r; splay(l);
        Node* r = x->r; while (r->l) r = r->l; splay(r);
        if (ccw(r->val, l->val, x->val) <= 0) {
            pop(x->val);
            return 0;
        }

        // pop left
        splay(l);
        while (l->l) {
            Node* ll = l->l; while (ll->r) ll = ll->r; splay(ll);
            if (ccw(x->val, ll->val, l->val) <= 0) pop(l->val);
            else break;
            l = ll; splay(l);
        }

        // pop right
        splay(r);
        while (r->r) {
            Node* rr = r->r; while (rr->l) rr = rr->l; splay(rr);
            if (ccw(rr->val, x->val, r->val) <= 0) pop(r->val);
            else break;
            r = rr; splay(r);
        }
        return 1;
    }
public:
    SplayTree() {
        root = new Node({ -1, INF });
        root->r = new Node({ INF, -1 });
        root->r->p = root; splay(root->r);
    }
    ~SplayTree() { if (root) delete root; root = 0; }
    int size() const { return root ? root->size : 0; }
    bool insert(const Pos& val) {
        if (!root) {
            root = new Node(val);
            return 1;
        }
        Node* p = root;
        Node** pp;
        while (1) {
            if (p->val.x == val.x) {
                if (val.x >= p->val.x) return 0;
                p->val.x = val.x;
                return update_convex(p);
            }
            if (val < p->val) {
                if (!p->l) {
                    pp = &p->l;
                    break;
                }
                p = p->l;
            }
            else {
                if (!p->r) {
                    pp = &p->r;
                    break;
                }
                p = p->r;
            }
        }
        Node* x = new Node(val);
        x->p = p;
        *pp = x;
        return update_convex(x);
    }
    bool find(const Pos& val) {
        if (!root) return false;
        Node* p = root;
        while (1) {
            if (p->val.x == val.x) break;
            if (val < p->val) {
                if (!p->l) break;
                p = p->l;
            }
            else {
                if (!p->r) break;
                p = p->r;
            }
        }
        splay(p);
        return p->val == val;
    }
    void pop(const Pos& val) {
        if (!find(val)) return;
        Node* p = root;

        if (p->l && p->r) {
            root = p->l; root->p = 0;
            Node* l = root;
            while (l->r) l = l->r;
            l->r = p->r;
            p->r->p = l;
            splay(p->r);
        }
        else if (p->l) root = p->l, root->p = 0;
        else if (p->r) root = p->r, root->p = 0;
        else root = 0;
        p->l = p->r = 0;

        delete p;
    }
    ll operator[](size_t i) {
        Node* p = root;
        if (!p) return INF;
        while (1) {
            while (p->l && p->l->size > i) p = p->l;
            if (p->l) i -= p->l->size;
            if (!i--) break;
            p = p->r;
        }
        splay(p);
        return p->val.x * p->val.y;
    }
} dp[LEN];

struct Info {
    int u;
    Pos p;
    bool operator<(const Info& o) const { return p.x * p.y > o.p.x * o.p.y; }
};

std::priority_queue<Info> pq;
std::vector<Info> graph[LEN];

int N, M;

int main() {
    std::cin >> N >> M;
    for (int i = 0, u, v, t, c; i < M; ++i) {
        std::cin >> u >> v >> t >> c;
        graph[u].push_back({ v, { t, c } });
        graph[v].push_back({ u, { t, c } });
    }
    pq.push({ 1, { 0, 0 } });
    dp[1].insert({ 0, 0 });
    while (pq.size()) {
        int u = pq.top().u;
        Pos cur = pq.top().p;
        // std::cout << "top: " << u << ' ' << cur.x << ' ' << cur.y << '\n';
        pq.pop();
        if (!dp[u].find(cur)) continue;
        for (const Info& i : graph[u]) {
            int v = i.u;
            Pos nxt = cur + i.p;
            // std::cout << "push: " << u << "->" << v << ", (" << nxt.x << ", " << nxt.y << ")\n";
            if (dp[v].insert(nxt)) pq.push({ v, nxt });
        }
    }
    for (int i = 2; i <= N; ++i) {
        ll ret = INF * INF;
        // std::cout << dp[i].size() << '\n';
        for (int j = 1; j < dp[i].size() - 1; ++j) {
            ll cost = dp[i][j];
            ret = std::min(ret, cost);
        }
        std::cout << (ret < INF * INF ? ret : -1) << '\n';
    }
}