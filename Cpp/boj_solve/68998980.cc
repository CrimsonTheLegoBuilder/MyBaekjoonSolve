#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define fi first
#define se second
typedef long double dd;
typedef pair<dd,dd> pt;
#define x first
#define y second
dd myrt(dd x){
	return sqrt(max((dd)0,x));
}
dd dist(pt x){return myrt(x.fi*x.fi+x.se*x.se);}
dd sqdist(pt x){return x.fi*x.fi+x.se*x.se;}
pt operator+(pt x,pt y){return {x.fi+y.fi,x.se+y.se};}
pt operator-(pt x,pt y){return {x.fi-y.fi,x.se-y.se};}
pt operator*(dd k,pt x){return {k*x.fi,k*x.se};};
pt operator*(pt x,dd k){return {k*x.fi,k*x.se};};
dd operator&(pt x,pt y){return x.fi*y.fi+x.se*y.se;}//dot product
dd operator^(pt x,pt y){return x.fi*y.se-x.se*y.fi;}//cross product
 

const int N=5e5+55;
int n;
pair<dd,pt>a[N];//{radius,{center_x,center_y}}
dd get_y(int h,dd x,pt bl,pt br){//given circle id, x-coordinate => returns upper y-coordinate
	dd y;
	if(h==0) y=(bl.y*(br.x-x)+br.y*(x-bl.x))/(br.x-bl.x);//line case
	else{
		y=a[h].se.y+myrt(a[h].fi*a[h].fi-(a[h].se.x-x)*(a[h].se.x-x));
	}
	return y;
}
 
 
 
 
 
 
 
 
//--geo stuff above
 
int ts=0;
int sz[N],lc[N],rc[N];
pt bl[N],br[N];
int cen[N];
mt19937 rng(226);
void init(int id){
	sz[id]=1;lc[id]=0,rc[id]=0;
}
int r(){
	return uniform_int_distribution<int>(0,1e9)(rng);
}
void pull(int id){
	sz[id]=sz[lc[id]]+sz[rc[id]]+1;
}
int join(int x,int y){
	if(x==0 || y==0) return x+y;
	if(r()%(sz[x]+sz[y])<sz[x]){
		rc[x]=join(rc[x],y);
		pull(x);return x;
	}
	else{
		lc[y]=join(x,lc[y]);
		pull(y);return y;
	}
}
pair<int,int>split(int id,int g){
	if(id==0) return {0,0};
	dd optx;
	if(cen[id]!=0){
		int h=cen[id];
		if(a[h].se.x+a[h].fi<=a[g].se.x+a[g].fi) optx=1e12;
		else if(a[h].se.x-a[h].fi>=a[g].se.x-a[g].fi) optx=-1e12;
		else{
			dd dx=(a[g].se.x-a[h].se.x);
			dx=dx*a[h].fi/(a[h].fi-a[g].fi);
			optx=a[h].se.x+dx;
		}
	}
	else{
		dd x=a[g].se.x;
		pt frog=br[id]-bl[id];
		dd dx=a[g].fi*frog.y/myrt(frog.x*frog.x+frog.y*frog.y);
		optx=a[g].se.x-dx;
	}
	if(optx<=bl[id].x){
		auto tmp=split(lc[id],g);
		lc[id]=tmp.se;pull(id);
		return {tmp.fi,id};
	}
	else if(optx>=br[id].x){
		auto tmp=split(rc[id],g);
		rc[id]=tmp.fi;pull(id);
		return {id,tmp.se};
	}
	else{
		dd opty=get_y(cen[id],optx,bl[id],br[id]);
		init(++ts);
		br[ts]=br[id];
		br[id]=bl[ts]={optx,opty};
		cen[ts]=cen[id];
		int sad=rc[id];rc[id]=0;
		sad=join(ts,sad);
		return {id,sad};
	}
}
pt gl,gr;
bool die(int h,int g,pt b){
	//cout << "die " << h << ' ' << g << ' ' << b.fi << ' ' << b.se << endl;
	dd frog=(b-a[h].se)&(b-a[g].se);
	frog/=a[h].fi;
	//cout << frog << endl;
	if(frog<=a[g].fi) return true;
	else return false;
}
bool save(int h,int g,pt b){
	//cout << "die " << h << ' ' << g << ' ' << b.fi << ' ' << b.se << endl;
	dd frog=(b-a[h].se)&(b-a[g].se);
	frog/=a[h].fi;
	//cout << frog << endl;
	if(frog>=a[g].fi) return true;
	else return false;
}
pt tang_l(pt u,pt v,dd r){//tangent from v to circle center u radius r, u "left of" v
	pt mv1=(v-u);
	mv1=mv1*((r*r)/sqdist(v-u));
	pt mv2={-mv1.se,mv1.fi};
	mv2=mv2*(myrt(r*r-sqdist(mv1))/dist(mv2));
	return u+mv1+mv2;
}
pt tang_r(pt u,pt v,dd r){
	//cout << "tang_r " << u.x << ' ' << u.y << ' ' << v.x << ' ' << v.y << ' ' << r << endl;
	if(r==0) return u;
	pt mv1=(v-u);
	mv1=mv1*((r*r)/sqdist(v-u));
	pt mv2={-mv1.se,mv1.fi};mv2=-1*mv2;
	mv2=mv2*(myrt(r*r-sqdist(mv1))/dist(mv2));
	auto res=u+mv1+mv2;
	//cout << "res " << res.x << ' ' << res.y << endl;
	return u+mv1+mv2;
}
int kill_l(int id,int g){
	if(id==0) return 0;
	if(cen[id]!=0){
		int h=cen[id];
		if(die(h,g,bl[id])){//whole arc removed
			gl=bl[id];
			return kill_l(lc[id],g);
		}
		else if(save(h,g,br[id])){//whole arc saved
			gl=br[id];
			rc[id]=kill_l(rc[id],g);
			pull(id);return id;
		}
		//compute common tangent uwuwuwuwu
		pt p0=tang_l(a[h].se,a[g].se,a[h].fi-a[g].fi);
		pt p1=a[g].se-p0;
		pt mv3={-p1.se,p1.fi};
		mv3=mv3*((a[g].fi)/dist(mv3));
		gl=p0+mv3;
		br[id]=gl;rc[id]=0;pull(id);
		return id;
	}
	else{//check if halfplane completely covers circle g
		dd frog=(a[g].se-bl[id])^(br[id]-bl[id]);
		frog/=dist(br[id]-bl[id]);
		if(frog<=a[g].fi){
			gl=bl[id];
			return kill_l(lc[id],g);
		}
		else{
			gl=br[id];
			rc[id]=kill_l(rc[id],g);
			pull(id);return id;
		}
	}
}
int kill_r(int id,int g){
	if(id==0) return 0;
	if(cen[id]!=0){
		int h=cen[id];
		if(die(h,g,br[id])){//whole arc removed
			gr=br[id];
			return kill_r(rc[id],g);
		}
		else if(save(h,g,bl[id])){//whole arc saved
			gr=bl[id];
			lc[id]=kill_r(lc[id],g);
			pull(id);return id;
		}
		//compute common tangent uwuwuwuwu
		pt p0=tang_r(a[h].se,a[g].se,a[h].fi-a[g].fi);
		pt p1=a[g].se-p0;
		pt mv3={-p1.se,p1.fi};mv3=mv3*-1;
		mv3=mv3*((a[g].fi)/dist(mv3));
		gr=p0+mv3;
		bl[id]=gr;lc[id]=0;pull(id);
		return id;
	}
	else{//check if halfplane completely covers circle g
		dd frog=(a[g].se-bl[id])^(br[id]-bl[id]);
		frog/=dist(br[id]-bl[id]);
		if(frog<=a[g].fi){
			gr=br[id];
			return kill_r(rc[id],g);
		}
		else{
			gr=bl[id];
			lc[id]=kill_r(lc[id],g);
			pull(id);return id;
		}
	}
}
void flow(int id){
	if(id==0) return;
	flow(lc[id]);
	cout << "Info " << id << ' ' << cen[id] << endl;
	cout << bl[id].x << ' ' << bl[id].y << endl;
	cout << br[id].x << ' ' << br[id].y << endl;
	cout << endl;
	flow(rc[id]);
}
dd pi=3.1415926535897932384626433832795;
dd perm(int id){
	if(id==0) return 0;
	dd frog=perm(lc[id])+perm(rc[id]);
	if(bl[id].x==br[id].x) return frog;
	dd cur;
	if(cen[id]!=0){
		int h=cen[id];
		dd r=a[h].fi;
		pt md=(bl[id]+br[id])*0.5;
		dd theta=2*acos(min((dd)1,dist(md-a[h].se)/r));
		cur=r*theta;
	}
	else cur=dist(br[id]-bl[id]);
	return frog+cur;
}
dd solve(){
	ts=1;init(1);
	bl[ts]={a[1].se.x-a[1].fi,a[1].se.y};
	br[ts]={a[1].se.x+a[1].fi,a[1].se.y};
	cen[ts]=1;
	int rt=1;
	for(int i=2; i<=n ;i++){
		int g=i;
		if(a[i].fi==a[i-1].fi && a[i].se.x==a[i-1].se.x) continue;//fucking cringe case
		auto tmp=split(rt,i);
		/*cout << "LEFT " << i << endl;
		flow(tmp.fi);
		cout << "RIGHT " << i << endl;
		flow(tmp.se);
		cout << endl;*/
		gl={a[g].se.x-a[g].fi,a[g].se.y};
		gr={a[g].se.x+a[g].fi,a[g].se.y};
		tmp.fi=kill_l(tmp.fi,i);
		tmp.se=kill_r(tmp.se,i);
		/*cout << gl.x << ' ' << gl.y << endl;
		cout << gr.x << ' ' << gr.y << endl;*/
		if(gl.x>=gr.x){
			rt=join(tmp.fi,tmp.se);
			continue;
		}
		pt cl=tang_r(a[i].se,gl,a[i].fi);
		pt cr=tang_l(a[i].se,gr,a[i].fi);
		if(gl!=cl){
			init(++ts);
			cen[ts]=0;bl[ts]=gl;br[ts]=cl;
			tmp.fi=join(tmp.fi,ts);
		}
		if(cr!=gr){
			init(++ts);
			cen[ts]=0;bl[ts]=cr;br[ts]=gr;
			tmp.se=join(ts,tmp.se);
		}
		{
			init(++ts);
			cen[ts]=i;bl[ts]=cl;br[ts]=cr;
			rt=join(tmp.fi,ts);
			rt=join(rt,tmp.se);
		}
		/*cout << "FINISH " << i << ' ' << a[g].se.x << ' ' << a[g].se.y << endl;
		cout << "START " << endl;
		flow(rt);
		cout << "END " << endl;*/
	}
	return perm(rt);
}
int main(){
	ios::sync_with_stdio(false);cin.tie(0);
	dd theta=0.68982;
	cin >> n;
	ll tl=1e18,tr=-1e18;
	ll lu=-1e18,ld=1e18,ru=-1e18,rd=1e18;
	for(int i=1; i<=n ;i++){
		dd x,y,r;cin >> x >> y >> r;
		x*=14;y*=14;r*=14;
		y+=5e9;
        x+=5e9;
        
		a[i]={r,{x,y}};
		a[i]={r,{x*cos(theta)+y*sin(theta),x*sin(theta)-y*cos(theta)}};
		tl=min(tl,(ll)(x-r));
		tr=max(tr,(ll)(x+r));
	}
	for(int i=1; i<=n ;i++){
		if(a[i].se.x-a[i].fi==tl){
			lu=max(lu,(ll)a[i].se.y);
			ld=min(ld,(ll)a[i].se.y);
		}
		if(a[i].se.x+a[i].fi==tr){
			ru=max(ru,(ll)a[i].se.y);
			rd=min(rd,(ll)a[i].se.y);
		}
	}
	dd ans=(lu-ld)+(ru-rd);
	ans=0;
	sort(a+1,a+n+1);reverse(a+1,a+n+1);
	ans+=solve();
	for(int i=1; i<=n ;i++) a[i].se.y=-a[i].se.y;
	sort(a+1,a+n+1);reverse(a+1,a+n+1);
	ans+=solve();
	cout << fixed << setprecision(12) << ans/14 << '\n';
}