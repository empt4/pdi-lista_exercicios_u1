#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    //carrega a imagem em tons de cinza na pasta do capitulo
    cv::Mat image = cv::imread("noturno_icon.jpg", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        //fallback caso o executavel seja rodado de dentro da subpasta build
        image = cv::imread("../noturno_icon.jpg", cv::IMREAD_GRAYSCALE);
    }
    if (image.empty()) {
        std::cout << "erro" << std::endl;
        return -1;
    }

    //tabela LUT: 256 linhas, 1 coluna e 3 canais (BGR)
    cv::Mat lut(256, 1, CV_8UC3);

    //preenche a tabela com um gradiente personalizado (tons de roxo)
    for (int i = 0; i < 256; i++) {
        lut.at<cv::Vec3b>(i, 0)[0] = i; // Canal Azul (B)
        lut.at<cv::Vec3b>(i, 0)[1] = 0; // Canal Verde (G)
        lut.at<cv::Vec3b>(i, 0)[2] = i; // Canal Vermelho (R)
    }

    cv::Mat imagecolor;
    cv::applyColorMap(image, imagecolor, lut);

    //salva e exibe o resultado
    cv::imwrite("resultado_Personalizado.png", imagecolor);
    cv::imshow("Colormap Personalizado (Roxo)", imagecolor);
    cv::waitKey(0);

    return 0;
}