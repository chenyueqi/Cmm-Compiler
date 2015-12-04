int main() 
{
	int i = 0;
	int j = 0;
	int k = 0;
	int t;
	int a[100];
	int N = 100;

	while(i < N) {
		a[i] = N - i;
		i = i + 1;
	}

	while(j < N) {
		i = 0;
		while(i < N - 1) {
			if(a[i] > a[i + 1]) {
				t = a[i];
				a[i] = a[i + 1];
				a[i + 1] = t;
			}
			i = i + 1;
		}
		j = j + 1;
	}

	while(k < N) {
		write(a[k]);
		k = k + 1;
	}
	return 0;
}
