#include "Halide.h"
#include "halide_image_io.h"
#include <cstdlib>

using namespace Halide::Tools;

int main(int argc, char **argv) {
  Halide::Buffer<uint8_t> input = Halide::Tools::load_image("bird.jpg");

  Halide::Func proc, float_px;
  Halide::Var x, y, c;

  // O quanto de brilho quer aumentar
  float brightness = 0.3f;
  
  // Transforma em float
  float_px(x,y,c) = Halide::cast<float>(input(x,y,c) / 255.0f);
  
  // Aumenta o brilho
  Halide::Expr convert_brightness = float_px(x,y,c) + brightness;
  convert_brightness = Halide::max(convert_brightness, 0);
  convert_brightness = Halide::min(convert_brightness, 1);

  // Converte para int
  Halide::Expr uint_px = Halide::cast<uint8_t>(convert_brightness * 255);

  //Implementa
  proc(x, y, c) = uint_px;

  try {
    Halide::Buffer<uint8_t> output = proc.realize({input.width(), input.height(), input.channels()});
    Halide::Tools::save_image(output, "bird_claro.png");
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
