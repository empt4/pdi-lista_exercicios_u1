#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

// Variaveis globais para atualizacao interativa pelas trackbars
cv::Mat origGray;
cv::Mat complexInput;
cv::Mat filteredResult;
cv::Mat filterVis;

int gammaL_slider = 40;   // gamma_L = slider / 100.0 (0.00 a 1.00)
int gammaH_slider = 200;  // gamma_H = slider / 100.0 (0.00 a 4.00)
int c_slider = 10;        // c       = slider / 10.0  (0.1 a 10.0)
int d0_slider = 30;       // D0      = frequencia de corte (pixels)

void swapQuadrants(cv::Mat& image) {
  cv::Mat tmp, A, B, C, D;

  image = image(cv::Rect(0, 0, image.cols & -2, image.rows & -2));

  int centerX = image.cols / 2;
  int centerY = image.rows / 2;

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

void makeHomomorphicFilter(const cv::Mat& image, cv::Mat& filter, float gammaL,
                           float gammaH, float c, float d0) {
  cv::Mat_<float> filter2D(image.rows, image.cols);
  int centerX = image.cols / 2;
  int centerY = image.rows / 2;
  float d0_sq = d0 * d0;

  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      float d2 = static_cast<float>((i - centerY) * (i - centerY) +
                                    (j - centerX) * (j - centerX));
      filter2D.at<float>(i, j) =
          (gammaH - gammaL) * (1.0f - std::exp(-c * (d2 / d0_sq))) + gammaL;
    }
  }

  cv::normalize(filter2D, filterVis, 0, 1, cv::NORM_MINMAX);

  cv::Mat planes[] = {filter2D, cv::Mat::zeros(filter2D.size(), CV_32F)};
  cv::merge(planes, 2, filter);
}

void applyHomomorphicFilter(int, void*) {
  float gammaL = gammaL_slider / 100.0f;
  float gammaH = gammaH_slider / 100.0f;
  float c = std::max(1, c_slider) / 10.0f;
  float d0 = static_cast<float>(std::max(1, d0_slider));

  cv::Mat filter, filtered;
  makeHomomorphicFilter(complexInput, filter, gammaL, gammaH, c, d0);

  // Multiplicacao no dominio da frequencia
  cv::mulSpectrums(complexInput, filter, filtered, 0);

  // DFT inversa com escalonamento (DFT_SCALE evita overflow no exp)
  swapQuadrants(filtered);
  cv::idft(filtered, filtered, cv::DFT_SCALE);

  std::vector<cv::Mat> planos;
  cv::split(filtered, planos);

  // Recorta para o tamanho original (remove o padding)
  cv::Rect roi(0, 0, origGray.cols, origGray.rows);
  cv::Mat result = planos[0](roi).clone();

  // Exponencial para retornar do dominio logaritmico
  cv::exp(result, result);
  result -= 1.0f;

  // Normaliza para [0, 1] para exibicao
  cv::normalize(result, filteredResult, 0, 1, cv::NORM_MINMAX);

  cv::imshow("Original (Cinza)", origGray);
  cv::imshow("Filtro Homomorfico", filteredResult);
  cv::imshow("Mascara H(u,v)", filterVis);
}

void saveResults() {
  float gammaL = gammaL_slider / 100.0f;
  float gammaH = gammaH_slider / 100.0f;
  float c = std::max(1, c_slider) / 10.0f;
  float d0 = static_cast<float>(std::max(1, d0_slider));

  cv::Mat outFiltered, outFilter;
  filteredResult.convertTo(outFiltered, CV_8U, 255.0);
  filterVis.convertTo(outFilter, CV_8U, 255.0);

  cv::imwrite("resultado_original_cinza.png", origGray);
  cv::imwrite("resultado_homomorfico.png", outFiltered);
  cv::imwrite("resultado_filtro_homomorfico.png", outFilter);

  std::cout << std::fixed << std::setprecision(2)
            << "\nImagens salvas em capitulo_17/:\n"
            << "  - resultado_original_cinza.png\n"
            << "  - resultado_homomorfico.png\n"
            << "  - resultado_filtro_homomorfico.png\n"
            << "Parametros utilizados: gamma_L = " << gammaL
            << ", gamma_H = " << gammaH << ", c = " << c << ", D0 = " << d0
            << std::endl;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Uso: " << argv[0] << " <imagem_mal_iluminada>" << std::endl;
    return EXIT_FAILURE;
  }

  origGray = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
  if (origGray.empty()) {
    std::cout << "Erro abrindo imagem: " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  // Redimensiona fotos muito grandes para manter as trackbars fluidas
  int maxDim = std::max(origGray.rows, origGray.cols);
  if (maxDim > 800) {
    double scale = 800.0 / maxDim;
    cv::resize(origGray, origGray, cv::Size(), scale, scale, cv::INTER_AREA);
  }

  // Tamanho otimo e par para a DFT (evita recorte no swapQuadrants)
  int dft_M = cv::getOptimalDFTSize(origGray.rows);
  int dft_N = cv::getOptimalDFTSize(origGray.cols);
  while (dft_M % 2 != 0) dft_M = cv::getOptimalDFTSize(dft_M + 1);
  while (dft_N % 2 != 0) dft_N = cv::getOptimalDFTSize(dft_N + 1);

  // Padding replicando a borda (reduz artefatos de ringing ao realcar altas freq.)
  cv::Mat padded;
  cv::copyMakeBorder(origGray, padded, 0, dft_M - origGray.rows, 0,
                     dft_N - origGray.cols, cv::BORDER_REPLICATE);

  // Conversao para float e transformacao logaritmica ln(1 + f(x,y))
  cv::Mat paddedFloat, logPadded;
  padded.convertTo(paddedFloat, CV_32F);
  cv::log(paddedFloat + 1.0f, logPadded);

  // Monta matriz complexa e pre-calcula a DFT uma unica vez
  std::vector<cv::Mat> planos;
  planos.push_back(logPadded);
  planos.push_back(cv::Mat::zeros(logPadded.size(), CV_32F));
  cv::merge(planos, complexInput);

  cv::dft(complexInput, complexInput);
  swapQuadrants(complexInput);

  cv::namedWindow("Filtro Homomorfico", cv::WINDOW_AUTOSIZE);
  int maxD0 = std::min(dft_M, dft_N) / 2;
  d0_slider = std::min(d0_slider, maxD0);

  cv::createTrackbar("gamma_L (x100)", "Filtro Homomorfico", &gammaL_slider,
                     100, applyHomomorphicFilter);
  cv::createTrackbar("gamma_H (x100)", "Filtro Homomorfico", &gammaH_slider,
                     400, applyHomomorphicFilter);
  cv::createTrackbar("c (x10)", "Filtro Homomorfico", &c_slider, 100,
                     applyHomomorphicFilter);
  cv::createTrackbar("D0", "Filtro Homomorfico", &d0_slider, maxD0,
                     applyHomomorphicFilter);

  applyHomomorphicFilter(0, nullptr);

  std::cout << "Ajuste as trackbars na janela 'Filtro Homomorfico'.\n"
            << "Pressione 's' para salvar ou 'ESC' para salvar e sair."
            << std::endl;

  while (true) {
    int key = cv::waitKey(30);
    if (key == 's' || key == 'S') {
      saveResults();
    } else if (key == 27) {  // ESC
      saveResults();
      break;
    }
  }

  return EXIT_SUCCESS;
}