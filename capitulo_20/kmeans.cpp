#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Uso: " << argv[0] << " <imagem> [nClusters]" << std::endl;
    return EXIT_FAILURE;
  }

  cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);
  if (img.empty()) {
    std::cout << "Erro abrindo imagem: " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  const int nClusters = (argc >= 3) ? std::stoi(argv[2]) : 8;
  const int nRodadas = 1;
  const int totalExecucoes = 10;

  // Inicializa o gerador interno do OpenCV com o relogio atual
  cv::theRNG().state = static_cast<uint64>(
      std::chrono::system_clock::now().time_since_epoch().count());

  // Monta a matriz de amostras (1 linha por pixel, 3 colunas para B, G, R)
  cv::Mat samples(img.rows * img.cols, 3, CV_32F);
  for (int y = 0; y < img.rows; y++) {
    for (int x = 0; x < img.cols; x++) {
      for (int z = 0; z < 3; z++) {
        samples.at<float>(y + x * img.rows, z) = img.at<cv::Vec3b>(y, x)[z];
      }
    }
  }

  std::vector<cv::Mat> miniaturas;
  const int tileW = 240;
  const int tileH = static_cast<int>(tileW * (static_cast<double>(img.rows) / img.cols));
  const int headerH = 28;

  int melhorRodada = 1, piorRodada = 1;
  double menorCompacidade = std::numeric_limits<double>::max();
  double maiorCompacidade = 0.0;

  std::cout << "Executando " << totalExecucoes << " rodadas de k-means (k="
            << nClusters << ", nRodadas=" << nRodadas
            << ", KMEANS_RANDOM_CENTERS)...\n\n";

  for (int r = 1; r <= totalExecucoes; r++) {
    cv::Mat rotulos, centros;

    double compacidade = cv::kmeans(
        samples, nClusters, rotulos,
        cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10000,
                         0.0001),
        nRodadas, cv::KMEANS_RANDOM_CENTERS, centros);

    cv::Mat rotulada(img.size(), img.type());
    for (int y = 0; y < img.rows; y++) {
      for (int x = 0; x < img.cols; x++) {
        int indice = rotulos.at<int>(y + x * img.rows, 0);
        rotulada.at<cv::Vec3b>(y, x)[0] =
            static_cast<uchar>(centros.at<float>(indice, 0));
        rotulada.at<cv::Vec3b>(y, x)[1] =
            static_cast<uchar>(centros.at<float>(indice, 1));
        rotulada.at<cv::Vec3b>(y, x)[2] =
            static_cast<uchar>(centros.at<float>(indice, 2));
      }
    }

    std::ostringstream ossNome;
    ossNome << "resultado_kmeans_" << std::setw(2) << std::setfill('0') << r
            << ".png";
    cv::imwrite(ossNome.str(), rotulada);

    if (compacidade < menorCompacidade) {
      menorCompacidade = compacidade;
      melhorRodada = r;
    }
    if (compacidade > maiorCompacidade) {
      maiorCompacidade = compacidade;
      piorRodada = r;
    }

    std::cout << "Rodada " << std::setw(2) << std::setfill('0') << r
              << " -> Compacidade: " << std::fixed << std::setprecision(2)
              << (compacidade / 1e6) << "e6  (" << ossNome.str() << ")"
              << std::endl;

    // Cria miniatura rotulada para o mosaico comparativo 2x5
    cv::Mat tile(tileH + headerH, tileW, CV_8UC3, cv::Scalar(30, 30, 30));
    cv::Mat resized;
    cv::resize(rotulada, resized, cv::Size(tileW, tileH), 0, 0, cv::INTER_AREA);
    resized.copyTo(tile(cv::Rect(0, headerH, tileW, tileH)));

    std::ostringstream ossLabel;
    ossLabel << "Rodada " << std::setw(2) << std::setfill('0') << r
             << " | C=" << std::fixed << std::setprecision(1)
             << (compacidade / 1e6) << "M";
    cv::putText(tile, ossLabel.str(), cv::Point(8, 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.52, cv::Scalar(240, 240, 240), 1,
                cv::LINE_AA);
    miniaturas.push_back(tile);
  }

  // Monta a grade 2x5 com todas as 10 rodadas
  cv::Mat linha1, linha2, grade;
  cv::hconcat(std::vector<cv::Mat>(miniaturas.begin(), miniaturas.begin() + 5),
              linha1);
  cv::hconcat(std::vector<cv::Mat>(miniaturas.begin() + 5, miniaturas.end()),
              linha2);
  cv::vconcat(linha1, linha2, grade);

  cv::imwrite("resultado_kmeans_grade.png", grade);

  std::cout << "\nResumo:\n"
            << "  Melhor rodada (menor compacidade): Rodada " << std::setw(2)
            << std::setfill('0') << melhorRodada << " (" << std::fixed
            << std::setprecision(2) << (menorCompacidade / 1e6) << "e6)\n"
            << "  Pior rodada   (maior compacidade): Rodada " << std::setw(2)
            << std::setfill('0') << piorRodada << " (" << std::fixed
            << std::setprecision(2) << (maiorCompacidade / 1e6) << "e6)\n"
            << "Grade comparativa salva em: resultado_kmeans_grade.png\n"
            << "Pressione qualquer tecla na janela para fechar." << std::endl;

  cv::imshow("Comparacao 10 Rodadas - K-Means (RANDOM_CENTERS)", grade);
  cv::waitKey();

  return EXIT_SUCCESS;
}