int printf(const char* format, ...);

int global_init_var = 84;
__attribute__((section("MyData"))) int global_init_var2 = 84;
int global_uninit_var;

__attribute__((section("Foo"))) void func1(int i) {
	printf("%d\n", i);
}

extern char edata[];
extern char etext[];

int main(void) {
	static int static_var = 85;
	static int static_var2;
	static int static_var3 = 0;
	static int static_var4 = 1;
	int a = 1;
	int b;
	func1(static_var + static_var2 + a + b);

	printf("edata is %X.\n", edata);
	printf("edata is %d.\n", *((int *)edata - 1));
	printf("edata is %d.\n", *((int *)edata - 2));
	printf("edata is %d.\n", *((int *)edata - 3));
	printf("etext is %X.\n", etext);

	return a;
}
