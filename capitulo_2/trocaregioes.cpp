#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    cv::Mat image = cv::imread("noturno_icon.jpg", cv::IMREAD_GRAYSCALE);
    if (!image.data) {
        std::cout << "erro" << std::endl;
        return -1;
    }

    //calcula os pontos médios
    int midX = image.cols / 2;
    int midY = image.rows / 2;

    //define as 4 Regiões de Interesse
    cv::Rect rect_TL(0, 0, midX, midY);          //superior esquerdo
    cv::Rect rect_TR(midX, 0, midX, midY);       //superior direito
    cv::Rect rect_BL(0, midY, midX, midY);       //inferior esquerdo
    cv::Rect rect_BR(midX, midY, midX, midY);    //inferior direito

    //cria cabeçalhos que apontam para as regiões da imagem original
    cv::Mat TL = image(rect_TL);
    cv::Mat TR = image(rect_TR);
    cv::Mat BL = image(rect_BL);
    cv::Mat BR = image(rect_BR);

    cv::Mat temp; //matriz para auxiliar na troca

    //troca os quadrantes diagonais: superior esquerdo com inferior direito
    TL.copyTo(temp);
    BR.copyTo(TL);
    temp.copyTo(BR);

    //troca os quadrantes diagonais: superior direito com inferior esquerdo
    TR.copyTo(temp);
    BL.copyTo(TR);
    temp.copyTo(BL);

    cv::imshow("Troca de Regioes", image);
    cv::waitKey();
    return 0;
}