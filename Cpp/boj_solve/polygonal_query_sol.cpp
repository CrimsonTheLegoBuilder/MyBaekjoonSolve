// Solution by William Gozali

/*
Assumtions:
- no 2 point with same oordinat value (x or y)
- ASK is called after at least 3 ADD
 */
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <deque>
#include <random>
#include <array>
#include <tuple>
#include <complex>
#include <set>

using namespace std;

typedef long long LL;

const int ADD = 0;
const int ASK = 1;

class Point {
public:
    LL x, y;

    Point() : x(0), y(0) {};
    Point(int _x, int _y) : x(_x), y(_y) {};

    bool operator<(const Point& o) const {
        return x < o.x;
    }
    bool operator<=(const Point& o) const {
        return x <= o.x;
    }
    bool operator==(const Point& o) const {
        return (x == o.x) && (y == o.y);
    }

    void print() {
        printf("%lld %lld\n", x, y);
    }
};

class HalfHull {
private:
    const int OFFSET = 1000000;
    bool lowerHull;
    set<Point> him;
    LL minX;
    LL maxX;
    int bitt[1000002];

    Point transform(Point p) {
        // Invert & also make non zero, 1..1000001
        if (lowerHull) {
            Point transformed(-p.x + OFFSET + 1, -p.y + OFFSET + 1);
            return transformed;
        }
        else {
            p.x++;
            p.y++;
            return p;
        }
    }

    Point transformBack(Point p) {
        if (lowerHull) {
            Point transformed(-p.x + OFFSET + 1, -p.y + OFFSET + 1);
            return transformed;
        }
        else {
            p.x--;
            p.y--;
            return p;
        }
    }

    int bitQuery(int x) {
        int ret = 0;
        for (int i = x; i; i -= i & -i) {
            ret += bitt[i];
        }
        return ret;
    }

    void bitUpdate(int x, int v) {
        for (int i = x; i <= OFFSET + 1; i += i & -i) {
            bitt[i] += v;
        }
    }

    LL det(const Point& a, const Point& b, const Point& c) {
        return a.x * b.y + b.x * c.y + c.x * a.y -
            a.x * c.y - b.x * a.y - c.x * b.y;
    }

    bool isCcw(const Point& a, const Point& b, const Point& c) {
        return det(a, b, c) > 0;
    }

    bool isCw(const Point& a, const Point& b, const Point& c) {
        return det(a, b, c) < 0;
    }

    bool hasTwoBefore(Point p) {
        set<Point>::iterator it = him.find(p);

        if (it == him.begin()) return false;
        it--;
        if (it == him.begin()) return false;
        return true;
    }

    bool hasTwoAfter(Point p) {
        set<Point>::iterator it = him.find(p);

        it++;
        if (it == him.end()) return false;
        it++;
        if (it == him.end()) return false;
        return true;
    }

    void correctToLeft(Point p) {
        // printf("HAS TWO BEFORE %d\n", hasTwoBefore(p));

        while (hasTwoBefore(p)) {
            set<Point>::iterator it = him.find(p);
            set<Point>::iterator before = --it;
            set<Point>::iterator twoBefore = --it;

            // printf("CORRECTLEFT\n");
            // transformBack(*twoBefore).print();
            // transformBack(*before).print();
            // transformBack(p).print();
            // printf("%d\n\n", isCcw(p, *before, *twoBefore));

            if (!isCcw(p, *before, *twoBefore)) {
                bitUpdate(before->x, -1);
                him.erase(before);
            }
            else {
                break;
            }
        }
    }

    void correctToRight(Point p) {
        // printf("HAS TWO AFTER %d\n", hasTwoAfter(p));
        while (hasTwoAfter(p)) {
            set<Point>::iterator it = him.find(p);
            set<Point>::iterator after = ++it;
            set<Point>::iterator twoAfter = ++it;

            // printf("CORRECTRIGHT\n");
            // transformBack(p).print();
            // transformBack(*after).print();
            // transformBack(*twoAfter).print();
            // printf("%d\n\n", isCw(p, *after, *twoAfter));

            if (!isCw(p, *after, *twoAfter)) {
                bitUpdate(after->x, -1);
                him.erase(after);
            }
            else {
                break;
            }
        }
    }

    bool isInside(Point p) {
        set<Point>::iterator it = him.upper_bound(p);

        // Is right most?
        if (it == him.end()) return false;

        // Is left most?
        if (it == him.begin()) return false;

        set<Point>::iterator after = it;
        it--;
        set<Point>::iterator prev = it;

        return !isCw(*prev, p, *after);
    }

public:
    HalfHull(bool _lowerHull) : lowerHull(_lowerHull) {
        memset(bitt, 0, sizeof(bitt));
    };

    void add(Point p) {
        p = transform(p);

        if (him.size() <= 1) {
            him.insert(p);
            bitUpdate(p.x, 1);

        }
        else if (!isInside(p)) {
            him.insert(p);
            bitUpdate(p.x, 1);

            correctToLeft(p);
            correctToRight(p);
        }
    }

    bool has(Point p) {
        p = transform(p);
        return him.count(p) > 0;
    }

    int size() {
        return him.size();
    }

    int toBigX(Point p) {
        p = transform(p);
        return size() - bitQuery(p.x);
    }

    void print() {
        for (set<Point>::iterator it = him.begin(); it != him.end(); it++) {
            Point p = transformBack(*it);
            printf("%lld %lld\n", p.x, p.y);
        }
    }
};

HalfHull upperHull(false);
HalfHull lowerHull(true);

int sizeAll() {
    int sz = upperHull.size() + lowerHull.size();
    if (upperHull.size() == 1) {
        sz--;
    }
    else if (upperHull.size() > 1) {
        sz -= 2;
    }
    return sz;
}

void add(Point p) {
    upperHull.add(p);
    lowerHull.add(p);
}

int getPositionFromOrigin(Point point) {
    if (upperHull.has(point)) {
        return upperHull.toBigX(point);
    }
    else {
        return upperHull.size() + lowerHull.toBigX(point) - 1;
    }
}

bool isCcwLarger(Point pointA, Point pointB) {
    // printf("A: %d, B: %d\n", getPositionFromOrigin(pointA), getPositionFromOrigin(pointB));

    int sz = upperHull.size() + lowerHull.size() - 2;
    int ccw = getPositionFromOrigin(pointB) - getPositionFromOrigin(pointA);

    // Normalize
    if (ccw < 0) {
        ccw += sz;
    }

    int cw = sz - ccw;
    // printf("%d %d\n", ccw, cw);
    return ccw > cw;
}

int main() {
    int Q;
    scanf("%d", &Q);
    for (int i = 0; i < Q; i++) {
        int type;
        scanf("%d", &type);

        if (type == ADD) {
            int x, y;
            scanf("%d %d", &x, &y);
            Point p(x, y);
            add(p);

            // upperHull.print();
            // printf("--\n");
            // lowerHull.print();
            // printf("\n");

        }
        else {
            int xa, xb, ya, yb;
            scanf("%d %d %d %d", &xa, &ya, &xb, &yb);
            Point p1(xa, ya);
            Point p2(xb, yb);
            printf("%s\n", isCcwLarger(p1, p2) ? "CCW" : "CW");
        }
    }
}