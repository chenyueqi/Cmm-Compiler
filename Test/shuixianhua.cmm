int cube(int nt) {
    return nt * nt * nt;
}

int mod(int a, int b) {
    return (a - (a / b) * b);
}

int main() {
    int ans[4];
    int n, n2, n1, n0;
    int k = 0;

    ans[0] = 153;
    ans[1] = 370;
    ans[2] = 371;
    ans[3] = 407;

    n = 100;
    while (n < 1000) {
        n2 = n / 100;
        n1 = mod(n / 10, 10);
        n0 = mod(n, 10);

        if(n == cube(n2) + cube(n1) + cube(n0)) {
            write(n == ans[k]);
            k = k + 1;
        }
        n = n + 1;
    }

    if (k == 4) {
        write(44);
    }

    write(666);

    return 0;
}
