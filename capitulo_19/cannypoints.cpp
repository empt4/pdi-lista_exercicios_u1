#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <opencv2/opencv.hpp>
#include <random>
#include <vector>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Uso: " << argv[0] << " <imagem>" << std::endl;
    return EXIT_FAILURE;
  }

  cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);
  if (image.empty()) {
    std::cout << "Nao foi possivel abrir a imagem: " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  int width = image.cols;
  int height = image.rows;
  std::cout << "Imagem carregada: " << width << "x" << height << std::endl;

  // Versao em tons de cinza para o detector de bordas de Canny
  cv::Mat gray;
  cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

  // Gerador aleatorio moderno (para embaralhamento e jitter)
  unsigned seed = static_cast<unsigned>(
      std::chrono::system_clock::now().time_since_epoch().count());
  std::mt19937 rng(seed);

  // ETAPA 1: Pontilhismo basico com pontos maiores (camada de fundo)

  const int STEP = 5;
  const int JITTER = 3;
  const int BASE_RADIUS = 5;

  std::vector<int> rowRange(height / STEP);
  std::vector<int> colRange(width / STEP);
  std::iota(rowRange.begin(), rowRange.end(), 0);
  std::iota(colRange.begin(), colRange.end(), 0);

  for (int& r : rowRange) r = r * STEP + STEP / 2;
  for (int& c : colRange) c = c * STEP + STEP / 2;

  cv::Mat points(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
  std::uniform_int_distribution<int> jitterDist(-JITTER, JITTER);

  std::shuffle(rowRange.begin(), rowRange.end(), rng);
  for (int r : rowRange) {
    std::shuffle(colRange.begin(), colRange.end(), rng);
    for (int c : colRange) {
      int sr = std::clamp(r + jitterDist(rng), 0, height - 1);
      int sc = std::clamp(c + jitterDist(rng), 0, width - 1);

      cv::Vec3b bgr = image.at<cv::Vec3b>(sr, sc);
      cv::circle(points, cv::Point(sc, sr), BASE_RADIUS,
                 cv::Scalar(bgr[0], bgr[1], bgr[2]), cv::FILLED, cv::LINE_AA);
    }
  }

  // Salva o pontilhismo basico para comparacao antes/depois no relatorio
  cv::Mat basePoints = points.clone();
  cv::imwrite("resultado_pontilhismo_base.png", basePoints);

  // ETAPA 2: Refinamento iterativo usando bordas de Canny
  // Limiares crescentes -> circulos progressivamente menores
  
  struct CannyPass {
    int lowThreshold;
    int radius;
  };

  std::vector<CannyPass> passes = {
      {20, 4},
      {50, 3},
      {90, 2},
      {140, 1},
  };

  cv::Mat border, borderExemplo;

  for (const auto& pass : passes) {
    cv::Canny(gray, border, pass.lowThreshold, 3 * pass.lowThreshold);

    // Guarda um mapa de bordas representativo (limiar 50 / 150) para o relatorio
    if (pass.lowThreshold == 50) {
      borderExemplo = border.clone();
    }

    // Coleta todas as coordenadas dos pixels de borda
    std::vector<cv::Point> edgePoints;
    for (int r = 0; r < height; r++) {
      const uchar* rowPtr = border.ptr<uchar>(r);
      for (int c = 0; c < width; c++) {
        if (rowPtr[c] > 0) {
          edgePoints.emplace_back(c, r);  // Point(x=col, y=row)
        }
      }
    }

    // Embaralha a ordem de desenho para evitar sobreposicao direcional
    std::shuffle(edgePoints.begin(), edgePoints.end(), rng);

    for (const auto& pt : edgePoints) {
      cv::Vec3b bgr = image.at<cv::Vec3b>(pt.y, pt.x);
      cv::circle(points, pt, pass.radius, cv::Scalar(bgr[0], bgr[1], bgr[2]),
                 cv::FILLED, cv::LINE_AA);
    }

    std::cout << "Canny (" << pass.lowThreshold << ", " << 3 * pass.lowThreshold
              << ") -> " << edgePoints.size()
              << " pontos de borda desenhados com raio " << pass.radius
              << std::endl;
  }

  cv::imwrite("resultado_canny_bordas.png", borderExemplo);
  cv::imwrite("resultado_cannypoints.png", points);

  std::cout << "\nImagens salvas em capitulo_19/:\n"
            << "  - resultado_pontilhismo_base.png\n"
            << "  - resultado_canny_bordas.png\n"
            << "  - resultado_cannypoints.png\n"
            << "Pressione qualquer tecla na janela para fechar." << std::endl;

  cv::imshow("Original", image);
  cv::imshow("Pontilhismo Base", basePoints);
  cv::imshow("Bordas Canny (50/150)", borderExemplo);
  cv::imshow("CannyPoints Final", points);
  cv::waitKey();

  return EXIT_SUCCESS;
}