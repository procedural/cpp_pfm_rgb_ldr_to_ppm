#if 0
g++ -std=c++11 -O2 pfm_rgb_ldr_to_ppm.cpp -o pfm_rgb_ldr_to_ppm
strip --strip-all pfm_rgb_ldr_to_ppm
exit
#endif

#include <string>
#include <fstream>
#include <sstream>

int main(int ArgsCount, char ** Args) {
  // https://github.com/OpenImageDenoise/oidn/blob/v1.2.2/apps/utils/image_io.cpp#L52

  // Open the file
  std::ifstream file(Args[1], std::ios::binary);
  if (file.fail())
    throw std::runtime_error("cannot open image file: " + std::string(Args[1]));

  // Read the header
  std::string id;
  file >> id;
  int C;
  if (id == "PF")
    C = 3;
  else
    throw std::runtime_error("invalid PFM image");

  file.get(); // skip newline

  int H, W;
  file >> W >> H;

  file.get(); // skip newline

  float scale;
  file >> scale;

  file.get(); // skip newline

  if (file.fail())
    throw std::runtime_error("invalid PFM image");

  if (scale >= 0.f)
    throw std::runtime_error("big-endian PFM images are not supported");

  std::stringstream p6;
  p6 << "P6\n" << std::to_string(W) << " " << std::to_string(H) << " 255\n";

  for (int h = 0; h < H; ++h)
  {
    for (int w = 0; w < W; ++w)
    {
      for (int c = 0; c < C; ++c)
      {
        float x;
        file.read((char*)&x, sizeof(float));
        p6 << (unsigned char)(x * 255.f);
      }
    }
  }

  std::ofstream out("out.ppm");
  out << p6.str();
  out.close();
}
