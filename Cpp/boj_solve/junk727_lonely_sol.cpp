#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <assert.h>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>
#define fastio cin.tie(0),cout.tie(0),ios::sync_with_stdio(0)
using namespace std;
 
typedef double ld;
typedef pair<ld,ld> pd;
#define all(x) (x).begin(),(x).end()
 
const ld PI = acos(-1);
const ld EPS = 1e-10;
inline bool is_zero(ld x){return -EPS<=x && x<=EPS;} 
 
struct circle
{
    circle() {}
    circle(int x, int y, int r) : x(x), y(y), r(r) {}
 
    int x,y,r;
 
    ld green(ld s, ld e)
    {
        ld a = r*r*(e-s);
        ld b = x * r * (sin(e)-sin(s));
        ld d = y * r * (cos(s)-cos(e));
        return (a+b+d)/2;
    }
 
    ld dist(const circle &m)
    {
        return (m.x-x)*(m.x-x) + (m.y-y)*(m.y-y);
    }
 
    ld angle(const circle &m)
    {
        return atan2((m.y-y),(m.x-x));
    }
 
    bool operator == (const circle &m) const {return x==m.x && y==m.y && r==m.r;}
};
 
int meetcc(circle c1, circle c2, ld mp[])
{
    ld d = c1.dist(c2);
    if(d > (c1.r+c2.r)*(c1.r+c2.r) + EPS) return 0; 
    if(d < (c1.r-c2.r)*(c1.r-c2.r) - EPS) return 0;
 
    ld ct = c1.angle(c2);
 
    ld temp = (c1.r*c1.r + d - c2.r*c2.r) / (2*c1.r*sqrt(d));
    if(temp<-1) temp=-1;
    if(temp>1) temp=1;
    
    auto mod = [&](ld x)
    {
        while(x<0) x+=2*PI;
        while(x>=2*PI) x-=2*PI;
        return x;
    };
 
    ld dt = acos(temp);
     
    int tp=0;
    if(is_zero(dt)) return tp;
    ld v1 = mod(ct-dt);
    mp[tp++] = v1;
    v1 = mod(ct+dt);
    mp[tp++] = v1;
    return tp;
}
 
 
int main()
{
    int n,ans=0; cin >> n;
 
    vector<circle> vc(n);
 
    for(int i=0;i<n;i++)
    {
        int x,y,r; cin >> x >> y >> r;
        vc[i] = {x,y,r};
    }
 
    vector<ld> area(n+1,0);
 
    sort(vc.begin(),vc.begin()+n,[](circle &p, circle &q){return p.r>q.r;});
 
    for(int x=0;x<=n;x++)
    {
        vector<bool> in(n,0); ld temp=0;
        for(int i=0;i<n;i++)
        {
            if(in[i] || i==x) continue;
 
            vector<pd> vx = {};
 
            for(int j=0;j<n;j++)
            {
                ld d = vc[i].dist(vc[j]);
                if(j==x || i==j || in[j] || d>=(vc[i].r+vc[j].r)*(vc[i].r+vc[j].r)) continue;
                if(d <= (vc[i].r-vc[j].r)*(vc[i].r-vc[j].r)) {in[j]=1; continue;}
 
                ld mp[2]; int tp = meetcc(vc[i],vc[j],mp);
                if(!tp) continue;
 
                if(mp[0]>mp[1]) vx.push_back({mp[0],2*PI}), vx.push_back({0,mp[1]});
                else vx.push_back({mp[0],mp[1]});
            }
 
            if(vx.empty()) {temp+=vc[i].r*vc[i].r*PI; continue;}
            sort(all(vx));
 
            vx.push_back({2*PI,2*PI});
 
            ld r=0;
            for(pd j : vx)
            {
                if(r<j.first) temp += vc[i].green(r,j.first), r=j.second; 
                else r=max(r,j.second);
            }
        }
        area[x]=temp;
    }
 
    for(int i=0;i<n;i++) if(is_zero(area[n]-area[i])) ans++;
    cout << ans;
}