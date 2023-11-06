#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std ;
using pt = pair<double, double> ;
// is the middle point below the a--b segment?
int interplow(const pt &a, const pt &b, const pt &c) {
   double delta { c.first - a.first } ;
   if (delta == 0)
      return b.second < a.second || b.second < c.second ;
   double y { (a.second * (c.first - b.first) +
               c.second * (b.first - a.first)) / delta } ;
   return b.second < y ;
}
int main(int argc, char *[]) {
   cout << setprecision(2) << fixed ;
   int N { 0 } ;
   double budget { 0 } ;
   cin >> N >> budget ;
   vector<pt> pts ;
   pts.push_back(make_pair(0, 0)) ;
   for (int i=0; i<N; i++) {
      double cost{0}, health{0}, potency{0} ;
      cin >> cost >> health >> potency ;
      pts.push_back(make_pair(budget*health/cost, budget*potency/cost)) ;
   }
   sort(pts.begin(), pts.end()) ;
   double r {0} ;
   for (int i=1; i<pts.size(); i++)
      r = max(pts[i].first*pts[i].second, r) ;
   if (argc > 1)
      cout << "Stupid solution: " << r << endl ;
   int wr {1} ;
   for (int i=1; i<(int)pts.size(); i++) {
      while (wr > 1 && interplow(pts[wr-2], pts[wr-1], pts[i]))
         wr-- ;
      pts[wr++] = pts[i] ;
   }
   for (int i=1; i+1<wr; i++) {
      double a { pts[i+1].second - pts[i].second } ;
      double b { pts[i].first - pts[i+1].first } ;
      double c { a * pts[i].first + b * pts[i].second } ;
      if (a != 0) {
         double bx { c / (2*a) } ;
         if (bx > pts[i].first && bx < pts[i+1].first) {
            double by = (c - a * bx) / b ;
            r = max(r, bx*by) ;
         }
      }
   }
   cout << r << endl ;
}
