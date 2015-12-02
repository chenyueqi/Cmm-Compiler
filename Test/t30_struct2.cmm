struct A{
    int a;
    float b;
    int e;
};
struct B{
    int c;
    struct A s;
    int k,l;
};

int f(struct B x){
    int res = x.l - x.s.e - x.c;
    write(x.l);
    write(x.s.e);
    write(x.c);
    write(x.l-x.s.e);
    write(res);
    res = res - 5;
    write(res);
    return res;
}

int main(){
    struct A aa;
    struct B bb;
    aa.a = 1;
    bb.l = 10;
    bb.s.e = 2;
    bb.c=3;
    f(bb);
    return aa.a;
}
