    #include <bits/stdc++.h>
    using namespace std;
    #define fori(i, a, b) for(int i = (int) a; i <= (int) b; ++i)
    #define rofi(i, b, a) for(int i = (int) b; i >= (int) a; --i)
    #define foreach(tt, a) for(auto &tt: a)
    #define all(a) begin(a), end(a)
    #define csz(a) (int) a.size()
    #define pb push_back
    #define epb emplace_back
    #define mp make_pair
    #define load(a, v) fill(begin(a), end(a), v)
    #define load_mem(a, v) memset(a, v, sizeof(a));
    #define iostream_optimize() ios::sync_with_stdio(false); cin.tie(0);
    using ll = long long;
    const ll MOD = 1e9+7, LINF = 1e16+1;
    const int INF = 1e9+1;
    const double EPS = 1e-10;
    const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
    const ll P1 = 31, P2 = 37, M1 = 1e9+7, M2 = 1e9+9;
     
    // #include <ext/pb_ds/tree_policy.hpp>
    // #include <ext/pb_ds/assoc_container.hpp>
    // using namespace __gnu_pbds;
    // template <typename T>
    // using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<< END TEMPLATE >>>>>>>>>>>>>>>>>>>>>>>>>>> */
    const int N = 1e5+1;
     
    int n, q, bsz, a[N], blk[N], cnt[N];
    ll res[N];
    vector<tuple<int,int,int,int> > qq;
    struct upd {
    	int pos, val, pre;
    	upd() {}
    	upd(int pos, int val, int pre): pos(pos), val(val), pre(pre) {}
    };
    vector<upd> updates;
    bool mark[N];
     
    vector<int> vals;
    void compress() {
    	sort(all(vals));
    	vals.resize(unique(all(vals)) - vals.begin());
    	unordered_map<int,int> pos;
    	fori(i, 0, csz(vals)-1) pos[vals[i]] = i;
    	fori(i, 0, n-1) a[i] = pos[a[i]];
    	foreach(u, updates) {
    		u.val = pos[u.val];
    		u.pre = pos[u.pre];
    	}
    }
     
    ll glob_sum;
    void dec(int i) {
    	mark[i] = false;
    	int v = a[i];
    	--cnt[v];
    	if(cnt[v] == 0) glob_sum -= vals[v];
    	// cout << "dec " << i << ' ' << vals[v] << ' ' << glob_sum << endl;
    }
    void inc(int i) {
    	mark[i] = true;
    	int v = a[i];
    	if(cnt[v] == 0) glob_sum += vals[v];
    	++cnt[v];
    }
     
    int main() {
    	iostream_optimize();
    	cin >> n;
    	bsz = max((int) pow(n, 0.6), 1);
    	fori(i, 0, n-1) {
    		cin >> a[i];
    		vals.pb(a[i]);
    		blk[i] = i / bsz;
    	}
    	cin >> q;
    	load(res, INF);
    	fori(i, 0, q-1) {
    		char inst; int l, r;
    		cin >> inst >> l >> r;
    		if(inst == 'Q') {
    			--l, --r;
    			qq.epb(l, r, csz(updates)-1, i);
    		} else { // l, r -> pos, val
    			--l;
    			vals.pb(r);
    			updates.epb(l, r, a[l]);
    			a[l] = r;
    		}
    	}
    	rofi(i, csz(updates)-1, 0) a[updates[i].pos] = updates[i].pre;
    	compress();
    	sort(all(qq), [] (const auto& x, const auto &y) {
    		int lx, rx, tx, ly, ry, ty;
    		tie(lx, rx, tx, ignore) = x;
    		tie(ly, ry, ty, ignore) = y;
    		return make_tuple(blk[lx], blk[rx], tx) < make_tuple(blk[ly], blk[ry], ty);
    	});
    	int l = 0, r = 0, t = -1;
    	inc(0);
    	foreach(qqq, qq) {
    		int tl, tr, tt, i;
    		tie(tl, tr, tt, i) = qqq;
    		while(t < tt) {
    			++t;
    			if(mark[updates[t].pos]) {
    				dec(updates[t].pos);
    				a[updates[t].pos] = updates[t].val;
    				inc(updates[t].pos);
    			} else {
    				a[updates[t].pos] = updates[t].val;
    			}
    		}
    		while(t > tt) {
    			if(mark[updates[t].pos]) {
    				dec(updates[t].pos);
    				a[updates[t].pos] = updates[t].pre;
    				inc(updates[t].pos);
    			} else {
    				a[updates[t].pos] = updates[t].pre;
    			}
    			--t;
    		}
    		while(l > tl) inc(--l);
    		while(r < tr) inc(++r);
    		while(l < tl) dec(l++);
    		while(r > tr) dec(r--);
    		res[i] = glob_sum;
    	}
    	fori(i, 0, q-1) if(res[i] != INF) cout << res[i] << '\n';
    }
