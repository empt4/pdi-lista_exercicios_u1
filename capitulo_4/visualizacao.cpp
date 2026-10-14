#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Uso: ./visualizacao <imagem.png>\n";
        return -1;
    }

    Mat image = imread(argv[1], cv::IMREAD_GRAYSCALE);
    if (!image.data) {
        std::cout << "erro\n";
        return -1;
    }

    //gera o arquivo da matriz completa (2D/3D)
    std::ofstream file("matriz.txt");
    for (int j = 0; j < image.rows; j++) {
        for (int i = 0; i < image.cols; i++) {
            file << (int)(image.at<uchar>(j, i)) << " ";
        }
        file << "\n";
    }
    file.close();

    //gera o arquivo da linha central (1D)
    file.open("line.txt");
    for (int i = 0; i < image.cols; i++) {
        file << (int)(image.at<uchar>(image.rows / 2, i)) << "\n";
    }
    file.close();

    return 0;
}