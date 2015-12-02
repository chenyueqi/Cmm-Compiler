int mod(int aaa, int bbb) {
    return (aaa - (aaa / bbb) * bbb);
}


int main() {
    int n, sum, i, k = 0;
    int ans[3];

    ans[0] = 6;
    ans[1] = 28;
    ans[2] = 496;

    n = 1;
    while (n < 500) {
        sum = 0;

        i = 1;
        while (i < n) {
            if(mod(n, i) == 0) {
                sum = sum + i;
            }
            i = i + 1;
        }
        
        if(sum == n) {
            write(n == ans[k]);
            k = k + 1;
        }
        n = n + 1;
    }

    write(k == 3);

    return 0;
}
