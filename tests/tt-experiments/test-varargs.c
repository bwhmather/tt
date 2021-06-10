

#define TEST_FN(...) test_fn((int *[]){__VA_ARGS__, NULL})

static void test_fn(int *children[]) {

}

int main() {
    test_fn((int *[]){malloc(sizeof(int))});

    TEST_FN(malloc(sizeof(int)), malloc(sizeof(int)));
}
