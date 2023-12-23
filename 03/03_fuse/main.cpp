#include "Halide.h"
#include "halide_image_io.h"

int main(int argc, char **argv) {
  Halide::Var x("x"), y("y");
  Halide::Func gradient("gradient");

  gradient(x, y) = x + y;

  Halide::Var fused;
  gradient.fuse(x, y, fused);

  // //equivalente em C++
  // for (int fused = 0; fused < 4 * 4; fused++) {
  //   int y = fused / 4;
  //   int x = fused % 4;
  //   out = x + y;
  // }

  gradient.trace_stores();
  gradient.print_loop_nest();

  try {
    Halide::Buffer<int32_t> output = gradient.realize({4, 4});
  }
  catch (Halide::CompileError& e){
    std::cout << "Halide::CompileError: " << e.what();
  }
  catch (Halide::RuntimeError& e){
    std::cout << "Halide::RuntimeError: " << e.what();
  }
  catch (std::exception& e){
    std::cout << "Generic Error: " << e.what();
  }

  return 0;
}
