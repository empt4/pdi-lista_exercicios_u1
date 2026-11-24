#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

std::string getBaseName(const std::string& path) {
  size_t slash = path.find_last_of("/\\");
  std::string filename =
      (slash == std::string::npos) ? path : path.substr(slash + 1);
  size_t dot = filename.find_last_of('.');
  return (dot == std::string::npos) ? filename : filename.substr(0, dot);
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Uso: " << argv[0] << " <imagem> [sigma]" << std::endl;
    return EXIT_FAILURE;
  }

  cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
  if (image.empty()) {
    std::cout << "Imagem nao carregou corretamente: " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  double sigma = (argc >= 3) ? std::stod(argv[2]) : 35.0;
  std::string base = getBaseName(argv[1]);

  // 1. Limiarizacao de Otsu direta (sem correcao de iluminacao)
  cv::Mat otsuDireto;
  double limiarDireto = cv::threshold(
      image, otsuDireto, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

  // 2. Estimativa da iluminacao de fundo via filtro Gaussiano de grande sigma
  cv::Mat imgFloat, fundoFloat;
  image.convertTo(imgFloat, CV_32F);
  cv::GaussianBlur(imgFloat, fundoFloat, cv::Size(0, 0), sigma, sigma);

  // 3. Correcao de iluminacao (modelo multiplicativo: refletancia ~ original / fundo)
  cv::Mat corrigidaFloat = (imgFloat + 1.0f) / (fundoFloat + 1.0f);

  cv::Mat fundoVis, corrigida;
  cv::normalize(fundoFloat, fundoVis, 0, 255, cv::NORM_MINMAX);
  fundoVis.convertTo(fundoVis, CV_8U);

  cv::normalize(corrigidaFloat, corrigida, 0, 255, cv::NORM_MINMAX);
  corrigida.convertTo(corrigida, CV_8U);

  // 4. Limiarizacao de Otsu sobre a imagem com iluminacao corrigida
  cv::Mat otsuCorrigido;
  double limiarCorrigido = cv::threshold(
      corrigida, otsuCorrigido, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

  // Salva todas as etapas automaticamente
  std::string outDireto = "resultado_otsu_" + base + ".png";
  std::string outFundo = "resultado_gaussiano_" + base + ".png";
  std::string outCorrigida = "resultado_corrigida_" + base + ".png";
  std::string outOtsuCorr = "resultado_otsu_corrigido_" + base + ".png";

  cv::imwrite(outDireto, otsuDireto);
  cv::imwrite(outFundo, fundoVis);
  cv::imwrite(outCorrigida, corrigida);
  cv::imwrite(outOtsuCorr, otsuCorrigido);

  std::cout << "Imagem: " << argv[1] << " (sigma = " << sigma << ")\n"
            << "  Limiar Otsu direto:    " << limiarDireto << " -> " << outDireto << "\n"
            << "  Fundo estimado:        " << outFundo << "\n"
            << "  Iluminacao corrigida:  " << outCorrigida << "\n"
            << "  Limiar Otsu corrigido: " << limiarCorrigido << " -> " << outOtsuCorr
            << std::endl;

  cv::imshow("Original", image);
  cv::imshow("Otsu Direto", otsuDireto);
  cv::imshow("Iluminacao Corrigida", corrigida);
  cv::imshow("Otsu Corrigido", otsuCorrigido);

  std::cout << "Pressione qualquer tecla na janela para fechar." << std::endl;
  cv::waitKey();

  return EXIT_SUCCESS;
}