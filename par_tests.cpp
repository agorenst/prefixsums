bool test_pointer_jumping(const int P[], const int size) {
  int presult[size];
  int sresult[size];
  pointer_jumping(P, presult, size);
  serial_pointer_jumping(P, sresult, size);
  return (equal_arrays(presult, sresult, size));
};


void ipss(T A[], const int len) {
  for (int i = 1; i < len; ++i) {
    A[i] += A[i-1];
  }
}

void prefix_test_suite(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "<array_size> <number_iterations>" << std::endl;
  }
  else {
    srand(time(NULL));
    const int size = atoi(argv[1]);
    const int iter = atoi(argv[2]);
    int test[size];
    for (int i = 0; i < iter; ++i) {
      rand_array(test, size);
      if (!test_parallel_prefix(test, size)) {
	std::cout << "Error!" << std::endl;
      }
    }
    std::cout << "Done" << std::endl;
  }
}

void init_worst_pointer_jump(const int to_init[], const int size) {
  for (int i = 0; i < size; ++i) {
    to_init[i] = i+1;
  }
  to_init[size-1] = size-1;
}

void test_par_prefix() {
  srand(time(NULL));
  const int arsize = 400000;
  int S[arsize];
  int W[arsize];
  int S2[arsize];
  int W2[arsize];
  for (int i = 0; i < 10; ++i) {
    make_bad_par_prefix(S, W, arsize);
    copy_array(S, S2, arsize);
    copy_array(W, W2, arsize);
    //    std::cout << equal_arrays(S, S2, arsize) << " " << equal_arrays(W, W2, arsize) << std::endl;
    parallel_prefix(S, S, W, arsize);
    serial_prefix(S2, S2, W2, arsize);
    std::cout << equal_arrays(S, S2, arsize) << " " << equal_arrays(W, W2, arsize) << std::endl;
  }
}
