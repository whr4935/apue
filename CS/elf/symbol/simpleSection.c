int printf(const char* format, ...);
int global_init_val = 84;
int global_uninit_val;

static __attribute__((visibility("hidden"))) void whr_func1(int i)
{
    printf("%d\n", i);
}

#if 0
int main(void)
{
    static int static_var = 85;
    static int static_var2;
    int a = 1;
    int b;

    func1(static_var + static_var2);

    return a;
}
#endif
