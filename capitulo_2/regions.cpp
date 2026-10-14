#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm> //necessário para std::min e std::max

int main(int argc, char** argv) {
    // Carrega a imagem em tons de cinza
    cv::Mat image = cv::imread("noturno_icon.jpg", cv::IMREAD_GRAYSCALE);
    if (!image.data) {
        std::cout << "erro" << std::endl;
        return -1;
    }

    int x1, y1, x2, y2;
    std::cout << "Dimensões da imagem: " << image.cols << "x" << image.rows << "\n";
    std::cout << "Digite a coordenada X do Ponto 1: "; std::cin >> x1;
    std::cout << "Digite a coordenada Y do Ponto 1: "; std::cin >> y1;
    std::cout << "Digite a coordenada X do Ponto 2: "; std::cin >> x2;
    std::cout << "Digite a coordenada Y do Ponto 2: "; std::cin >> y2;

    //tratamento de segurança: garante que o retângulo fique dentro dos limites da imagem e organiza qual é o ponto inicial e final
    int startX = std::max(0, std::min(x1, x2));
    int endX = std::min(image.cols, std::max(x1, x2));
    int startY = std::max(0, std::min(y1, y2));
    int endY = std::min(image.rows, std::max(y1, y2));

    //aplica o efeito negativo (255 - pixel atual) na região demarcada
    for (int i = startY; i < endY; i++) {
        for (int j = startX; j < endX; j++) {
            image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
        }
    }

    cv::imshow("Efeito Regioes - Negativo", image);
    cv::waitKey();
    return 0;
}