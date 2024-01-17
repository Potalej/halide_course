#include "Halide.h"
#include "halide_image_io.h"
#include <cstdlib>

using namespace Halide::Tools;

int main(int argc, char **argv) {
  float threshold = 0.5f;
  
  Halide::Buffer<uint8_t> input = Halide::Tools::load_image("bird.jpg");

  Halide::Func proc;
  Halide::Var x, y, c;
  Halide::Func float_px;
  
  // Converte para float em [0,1]
  float_px(x, y, c) = Halide::cast<float>(input(x, y, c) / 255.0f);

  // Faz a alteracao
  Halide::Expr gray = 0.299f * float_px(x, y, 0) + // Altera o canal vermelho 
                      0.587f * float_px(x, y, 1) + // Altera o canal verde
                      0.114f * float_px(x, y, 2);  // Altera o canal azul
  
  // Converte para inteiro
  Halide::Expr thresholded = Halide::cast<uint8_t>(gray > threshold) * 255;
  
  proc(x, y, c) = thresholded;

  try {
    Halide::Buffer<uint8_t> output = proc.realize({input.width(), input.height(), input.channels()});
    Halide::Tools::save_image(output, "bird_threshold.png");
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
