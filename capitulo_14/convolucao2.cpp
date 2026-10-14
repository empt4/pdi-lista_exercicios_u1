#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    cv::Mat image = cv::imread("noturno_icon.jpg", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cout << "erro ao abrir a imagem" << std::endl;
        return -1;
    }

    cv::Mat img32f, res3, res11, res21;
    image.convertTo(img32f, CV_32F); //converte para float para cálculos matemáticos precisos

    //cria as máscaras de media (matriz preenchida por 1's, dividida pela área total)
    cv::Mat mask3 = cv::Mat::ones(3, 3, CV_32F) / 9.0f;
    cv::Mat mask11 = cv::Mat::ones(11, 11, CV_32F) / 121.0f;
    cv::Mat mask21 = cv::Mat::ones(21, 21, CV_32F) / 441.0f;

    //aplica os filtros
    cv::filter2D(img32f, res3, img32f.depth(), mask3, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
    cv::filter2D(img32f, res11, img32f.depth(), mask11, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
    cv::filter2D(img32f, res21, img32f.depth(), mask21, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);

    //retorna ao formato original (8 bits)
    res3.convertTo(res3, CV_8U);
    res11.convertTo(res11, CV_8U);
    res21.convertTo(res21, CV_8U);

    //guarda os resultados
    cv::imwrite("media_03x03.png", res3);
    cv::imwrite("media_11x11.png", res11);
    cv::imwrite("media_21x21.png", res21);

    std::cout << "Imagens guardadas! Verifique os ficheiros media_...png" << std::endl;

    //mostra as janelas
    cv::imshow("Filtro 3x3", res3);
    cv::imshow("Filtro 11x11", res11);
    cv::imshow("Filtro 21x21", res21);
    cv::waitKey(0);

    return 0;
}