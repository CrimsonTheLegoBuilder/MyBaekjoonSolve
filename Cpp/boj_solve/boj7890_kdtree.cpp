#include <iostream>
#include <algorithm>
#include <climits>

// #define ONLINE_JUDGE

#ifdef ONLINE_JUDGE
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

// # define SIGNED

namespace fastio {
    constexpr size_t    OUT_BUFFER_SIZE = 1048576;

    namespace input {
        inline bool is_digit(char c) {
            return '0' <= c && c <= '9';
        }
        inline bool is_space(char c) {
            // return c == ' ' || (9 <= c && c <= 13);
            return c == ' ' || c == '\n';
        }

        char* i_st = nullptr;

        int ReadBuffer() {
            struct stat st;
            fstat(STDIN_FILENO, &st);
            i_st = (char*)mmap(nullptr, st.st_size, PROT_READ, MAP_SHARED, STDIN_FILENO, 0);
            return 1;
        }

        template <typename IntType>
        IntType ParseUnsignedInt() {
            IntType num = 0;
            while (is_digit(*i_st))
                num = 10 * num + (*i_st++ - '0');
            while (is_space(*i_st))
                i_st++;
            return num;
        }

        template <typename IntType>
        IntType ParseSignedInt() {
            IntType num = 0;
            bool neg = false;
            if (*i_st == '-') {
                neg = true;
                i_st++;
            }
            while (is_digit(*i_st))
                num = 10 * num + (*i_st++ - '0');
            while (is_space(*i_st))
                i_st++;
            return (neg ? -num : num);
        }
    }

    namespace output {
        char    out_buffer[OUT_BUFFER_SIZE];
        char* o_st = out_buffer;
        const char* o_ed = out_buffer + OUT_BUFFER_SIZE;

        void FlushBuffer() {
            write(STDOUT_FILENO, out_buffer, o_st - out_buffer);
            o_st = out_buffer;
        }

        inline void WriteChar(char c) {
            if (o_st == o_ed)
                FlushBuffer();
            *o_st++ = c;
        }

        void WriteStr(const char* s) {
            while (*s) {
                if (o_st == o_ed)
                    FlushBuffer();
                *o_st++ = *s++;
            }
        }

        template <typename IntType>
        void WriteUnsignedInt(IntType num) {
            int size = 0;
            char temp_buffer[20];
            do {
                temp_buffer[size++] = num % 10 + '0';
                num /= 10;
            } while (num);
            while (size) {
                if (o_st == o_ed)
                    FlushBuffer();
                *o_st++ = temp_buffer[--size];
            }
        }

        template <typename IntType>
        void WriteSignedInt(IntType num) {
            int size = 0;
            char temp_buffer[20];
            if (num < 0) {
                if (o_st == o_ed)
                    FlushBuffer();
                *o_st++ = '-';
                num = -num;
            }
            do {
                temp_buffer[size++] = num % 10 + '0';
                num /= 10;
            } while (num);
            while (size) {
                if (o_st == o_ed)
                    FlushBuffer();
                *o_st++ = temp_buffer[--size];
            }
        }

    }

    template <typename IntType>
#ifdef SIGNED
    const auto ParseInt = input::ParseSignedInt<IntType>;
#else
    const auto ParseInt = input::ParseUnsignedInt<IntType>;
#endif

    template <typename IntType>
#ifdef SIGNED
    const auto WriteInt = output::WriteSignedInt<IntType>;
#else
    const auto WriteInt = output::WriteUnsignedInt<IntType>;
#endif

    class IstreamWrapper {
    public:
        IstreamWrapper() {
            input::ReadBuffer();
        }

        template <typename IntType>
        inline const IstreamWrapper& operator>>(IntType& num) const {
            num = ParseInt<IntType>();
            return (*this);
        }
    };

    class OstreamWrapper {
    public:
        ~OstreamWrapper() {
            output::FlushBuffer();
        }

        template <typename IntType>
        inline const OstreamWrapper& operator<<(IntType num) const {
            WriteInt<IntType>(num);
            return (*this);
        }

        inline const OstreamWrapper& operator<<(char c) const {
            output::WriteChar(c);
            return (*this);
        }
        inline const OstreamWrapper& operator<<(const char* s) const {
            output::WriteStr(s);
            return (*this);
        }

    };

    const IstreamWrapper in;
    const OstreamWrapper out;
}

#define cin fastio::in
#define cout fastio::out
#define endl '\n'

#endif

#ifdef ONLINE_JUDGE
#define endl '\n'
#endif

using namespace std;
using int64 = long long;

constexpr int MAX = 100100;


struct point {
    int64 x, y;

    point() {}
    point(int64 x_, int64 y_) : x(x_), y(y_) {}

    inline bool operator==(const point& other) const {
        return (x == other.x) && (y == other.y);
    }
    inline bool operator!=(const point& other) const {
        return (x != other.x) || (y != other.y);
    }
    inline point operator-(const point& other) const {
        return point(x - other.x, y - other.y);
    }
    inline int64 mag_sq() const {
        return x * x + y * y;
    }

    friend ostream& operator<<(ostream& out, const point& p) {
        out << "(" << p.x << ", " << p.y << ")";
        return out;
    }
};


namespace kdtree {
    // simple implementation using nth_element (wo update)

    constexpr int MIN_PARTITION = 32;
    constexpr int64 DIST_MAX = LLONG_MAX;

    int n;
    point pts[MAX];
    enum axis {
        X = 0,
        Y
    };

    static inline bool cmp_x(const point& p1, const point& p2) {
        return p1.x < p2.x;
    }
    static inline bool cmp_y(const point& p1, const point& p2) {
        return p1.y < p2.y;
    }

    static inline int64 dist(const point& p1, const point& p2) {
        return (p1 - p2).mag_sq();
    }

    static void _Build(axis ax, int st, int ed) {
        // cout << "build: ax " << ((ax == X) ? "X" : "Y") << ", ";
        // cout << "range [" << st << ", " << ed << ")" << endl; 
        if (ed - st <= MIN_PARTITION) {
            // cout << "n_elem " << (ed - st ) << " <= MIN_PARTITION" << endl;
            return;
        }

        int mid = (st + ed) / 2;
        nth_element(pts + st, pts + mid, pts + ed, ((ax == X) ? cmp_x : cmp_y));

        // cout << "mid " << mid << " elem: " << pts[mid] << endl;

        _Build((axis)(1 - ax), st, mid);
        _Build((axis)(1 - ax), mid + 1, ed);
    }

    void Build(int n_, const point* pts_) {
        n = n_;
        for (int i = 0; i < n; i++) {
            pts[i] = pts_[i];
        }
        _Build(X, 0, n);
    }

    inline void update_min(int64& cur, int64 nxt) {
        cur = min(cur, nxt);
    }

    static int64 _FindMinDist(const point& p, axis ax, int st, int ed, int64 mn) {
        // cout << "find min " << p << ": cur_mn " << mn << ", ";
        // cout << "range [" << st << ", " << ed << ")" << endl; 
        if (ed - st <= MIN_PARTITION) {
            // cout << "reached MIN_PARTITION" << endl;
            for (int i = st; i < ed; i++) {
                if (p != pts[i]) {
                    // cout << "comp with " << pts[i] << ", d " << dist(p, pts[i]) << endl;
                    update_min(mn, dist(p, pts[i]));
                }
            }
            // cout << "cur mn " << mn << endl;
            return mn;
        }

        int mid = (st + ed) / 2;
        // cout << "mid " << mid << " elem " << pts[mid] << endl;
        if (p != pts[mid]) {
            // cout << "comp with " << pts[mid] << ", d " << dist(p, pts[mid]) << endl;
            update_min(mn, dist(p, pts[mid]));
        }

        int64 gap = ((ax == X) ? (p.x - pts[mid].x) : (p.y - pts[mid].y));
        if (gap < 0) {
            update_min(mn, _FindMinDist(p, (axis)(1 - ax), st, mid, mn));
            if (mn > gap * gap)
                update_min(mn, _FindMinDist(p, (axis)(1 - ax), mid + 1, ed, mn));
        }
        else {
            update_min(mn, _FindMinDist(p, (axis)(1 - ax), mid + 1, ed, mn));
            if (mn > gap * gap)
                update_min(mn, _FindMinDist(p, (axis)(1 - ax), st, mid, mn));
        }

        // cout << "cur mn " << mn << endl;
        return mn;
    }

    inline int64 FindMinDist(const point& p) {
        return _FindMinDist(p, X, 0, n, DIST_MAX);
    }

}

point pts[MAX];

void testcase() {
    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        cin >> pts[i].x >> pts[i].y;
    }
    kdtree::Build(n, pts);

    // cout << "\n=== BUILD DONE ===\n" << endl;

    for (int i = 0; i < n; i++) {
        // cout << "=== QUERY " << pts[i] << " ===" << endl;
        cout << kdtree::FindMinDist(pts[i]) << endl;
        // cout << endl;
    }
}

int main() {

#ifdef ONLINE_JUDGE
    // ios_base::sync_with_stdio(false);
    // cin.tie(nullptr);
#endif

    int t;
    cin >> t;
    while (t--) {
        testcase();
    }
}

