#include <iostream>
typedef long long ll;
const int LEN = 500'001;
const ll MOD = 1e9 + 7;
int N, h[LEN], C[LEN], sp, S[LEN];
//inline ll sq(ll x) { return (x % MOD) * x % MOD; }
#define sq(x) ((((ll)x) % MOD) * ((ll)x) % MOD)

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cin >> N;

	ll ret = 0;
	for (int i = 0, j, k; i < N; ++i) {
		std::cin >> h[i];
		C[i] = 1;//트리의 자손(과 자신의) 수를 1로 초기화
		while (sp && h[S[sp - 1]] <= h[i]) {//스택의 원소가 지금 받은 원소보다 작거나 같을 때까지 pop
			j = S[--sp];//스택 바로 위 원소
			k = sp && h[S[sp - 1]] < h[i] ? S[sp - 1] : i;//스택의 원소가 지금 원소보다 작으면 스택, 아니면 지금 원소
			//ret = (ret + ((ll)N - C[j]) * C[j] % MOD * (h[k] - h[j]) % MOD * (h[k] - h[j])) % MOD;
			ret = (ret + ((ll)N - C[j]) * C[j] % MOD * sq(h[k] - h[j]) % MOD) % MOD;//비용 계산
			//서브트리의 크기를 관리하면서 해당 간선이 비용 게산에서 총 몇 회 쓰이는지를 계산하여 비용을 더함
			C[k] += C[j];//트리를 연결
		}
		S[sp++] = i;//스택에 쌓기
	}
	while (sp > 1) {
		int i = S[sp - 1];//스택 맨 위
		int j = S[sp - 2];//바로 아래
		//ret = (ret + ((ll)N - C[i]) * C[i] % MOD * (h[j] - h[i]) % MOD * (h[j] - h[i])) % MOD;
		ret = (ret + ((ll)N - C[i]) * C[i] % MOD * sq(h[j] - h[i]) % MOD) % MOD;//비용 계산
		C[j] += C[i];//트리를 연결
		sp--;
	}
	std::cout << ret;
}//boj32383 refer to jay0202