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
    write(res);
    res = res - 5;
    write(res);
    return res;
}

int g(int pa,int pb){
    return pa+pb;
}

int main(){
    struct A aa;
    struct B bb;
    aa.a = 1;
    bb.l = 10;
    bb.s.e = 2;
    bb.c=3;
    f(bb);

    write(g(f(bb),f(bb)));
    return aa.a;
}
