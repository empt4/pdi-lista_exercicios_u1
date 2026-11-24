#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

void swapQuadrants(cv::Mat& image) {
  cv::Mat tmp, A, B, C, D;

  // Se a imagem tiver tamanho impar, recorta para o maior tamanho par possivel
  image = image(cv::Rect(0, 0, image.cols & -2, image.rows & -2));

  int centerX = image.cols / 2;
  int centerY = image.rows / 2;

  // Rearranja os quadrantes para trazer a origem para o centro
  // A B   ->  D C
  // C D       B A
  A = image(cv::Rect(0, 0, centerX, centerY));
  B = image(cv::Rect(centerX, 0, centerX, centerY));
  C = image(cv::Rect(0, centerY, centerX, centerY));
  D = image(cv::Rect(centerX, centerY, centerX, centerY));

  A.copyTo(tmp);
  D.copyTo(A);
  tmp.copyTo(D);

  C.copyTo(tmp);
  B.copyTo(C);
  tmp.copyTo(B);
}

bool isYamlFile(const std::string& filename) {
  auto endsWith = [](const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
  };
  return endsWith(filename, ".yml") || endsWith(filename, ".yaml") ||
         endsWith(filename, ".YML") || endsWith(filename, ".YAML");
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Uso: " << argv[0]
              << " <imagem.png | arquivo.yml> [saida_espectro.png]" << std::endl;
    return EXIT_FAILURE;
  }

  std::string inputPath = argv[1];
  cv::Mat image, padded, complexImage;
  std::vector<cv::Mat> planos;

  if (isYamlFile(inputPath)) {
    cv::FileStorage fs(inputPath, cv::FileStorage::READ);
    if (!fs.isOpened()) {
      std::cout << "Erro abrindo arquivo YAML: " << inputPath << std::endl;
      return EXIT_FAILURE;
    }
    fs["mat"] >> image;
    if (image.empty()) {
      // Fallback caso o no raiz do YAML tenha outro nome
      fs.getFirstTopLevelNode() >> image;
    }
    fs.release();
  } else {
    image = cv::imread(inputPath, cv::IMREAD_GRAYSCALE);
  }

  if (image.empty()) {
    std::cout << "Erro abrindo imagem: " << inputPath << std::endl;
    return EXIT_FAILURE;
  }

  // Converte para float (CV_32F) preservando os valores exatos do YAML ou do PNG
  cv::Mat imageFloat;
  image.convertTo(imageFloat, CV_32F);

  // Expande para o tamanho otimo da DFT com zero-padding
  int dft_M = cv::getOptimalDFTSize(imageFloat.rows);
  int dft_N = cv::getOptimalDFTSize(imageFloat.cols);
  cv::copyMakeBorder(imageFloat, padded, 0, dft_M - imageFloat.rows, 0,
                     dft_N - imageFloat.cols, cv::BORDER_CONSTANT,
                     cv::Scalar::all(0));

  // Prepara matriz complexa (parte real = imagem, parte imaginaria = zeros)
  planos.push_back(padded);
  planos.push_back(cv::Mat::zeros(padded.size(), CV_32F));
  cv::merge(planos, complexImage);

  // Calcula a DFT e desloca a origem para o centro
  cv::dft(complexImage, complexImage);
  swapQuadrants(complexImage);

  // Separa partes real (planos[0]) e imaginaria (planos[1])
  cv::split(complexImage, planos);

  // Calcula o espectro de magnitude
  cv::Mat magn;
  cv::magnitude(planos[0], planos[1], magn);

  // Compressao logaritmica de faixa dinamica e normalizacao para [0, 1]
  magn += cv::Scalar::all(1);
  cv::log(magn, magn);
  cv::normalize(magn, magn, 0, 1, cv::NORM_MINMAX);

  // Salva o espectro convertido para 8 bits (para o relatorio em AsciiDoc)
  std::string outputPath =
      (argc >= 3) ? argv[2]
                  : (isYamlFile(inputPath) ? "espectro_yml.png"
                                           : "espectro_png.png");
  cv::Mat magnOut;
  magn.convertTo(magnOut, CV_8U, 255.0);
  cv::imwrite(outputPath, magnOut);
  std::cout << "Espectro salvo em: " << outputPath << std::endl;

  // Normaliza apenas para visualizacao correta na janela (CV_32F espera [0,1])
  cv::Mat displayImg;
  cv::normalize(imageFloat, displayImg, 0, 1, cv::NORM_MINMAX);

  cv::imshow("Imagem", displayImg);
  cv::imshow("Espectro de magnitude", magn);
  cv::waitKey();

  return EXIT_SUCCESS;
}