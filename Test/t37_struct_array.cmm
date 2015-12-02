struct A{
    int a[10];
    int b;
    float k;
};

struct B{
    struct A sa1;
    int c[15];
    struct A sa2;
};

struct C{
    struct B sb1;
    struct A sa3;
    struct B b_Array[3];
    int d[10];
};

int main(){
    int i = 0;
    struct C ss;
    while(i<10){
        ss.sb1.sa1.a[i] = i;
        i = i + 1;
    }
    i=0;
    while(i<10){
        write(ss.sb1.sa1.a[i]);
        i = i + 1;
    }
    i=0;
    while(i<15){
        ss.b_Array[1].c[i] = i;
        i = i + 1;
    }
    i=0;
    while(i<15){
        write(ss.b_Array[1].c[i]);
        i = i + 1;
    }
    return 0;
}

