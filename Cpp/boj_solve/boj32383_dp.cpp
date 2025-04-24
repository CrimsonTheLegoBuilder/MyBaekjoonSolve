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
		C[i] = 1;//Ʈ���� �ڼ�(�� �ڽ���) ���� 1�� �ʱ�ȭ
		while (sp && h[S[sp - 1]] <= h[i]) {//������ ���Ұ� ���� ���� ���Һ��� �۰ų� ���� ������ pop
			j = S[--sp];//���� �ٷ� �� ����
			k = sp && h[S[sp - 1]] < h[i] ? S[sp - 1] : i;//������ ���Ұ� ���� ���Һ��� ������ ����, �ƴϸ� ���� ����
			//ret = (ret + ((ll)N - C[j]) * C[j] % MOD * (h[k] - h[j]) % MOD * (h[k] - h[j])) % MOD;
			ret = (ret + ((ll)N - C[j]) * C[j] % MOD * sq(h[k] - h[j]) % MOD) % MOD;//��� ���
			//����Ʈ���� ũ�⸦ �����ϸ鼭 �ش� ������ ��� �Ի꿡�� �� �� ȸ ���̴����� ����Ͽ� ����� ����
			C[k] += C[j];//Ʈ���� ����
		}
		S[sp++] = i;//���ÿ� �ױ�
	}
	while (sp > 1) {
		int i = S[sp - 1];//���� �� ��
		int j = S[sp - 2];//�ٷ� �Ʒ�
		//ret = (ret + ((ll)N - C[i]) * C[i] % MOD * (h[j] - h[i]) % MOD * (h[j] - h[i])) % MOD;
		ret = (ret + ((ll)N - C[i]) * C[i] % MOD * sq(h[j] - h[i]) % MOD) % MOD;//��� ���
		C[j] += C[i];//Ʈ���� ����
		sp--;
	}
	std::cout << ret;
}//boj32383 refer to jay0202