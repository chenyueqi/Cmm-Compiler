/*
int ans[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};
*/

int fact(int n) {
	if(n == 0 || n == 1) return 1;
	else return fact(n - 1) * n;
}

int main() {
	int i = 0;
	int f[15];
	while(i < 13) {
		f[i] = fact(i);
		write(f[i]);
		i = i + 1;
	}

	return 0;
}

