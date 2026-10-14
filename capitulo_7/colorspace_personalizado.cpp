#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

int main(int argc, char** argv) {
    cv::Mat image = cv::imread("../noturno_icon.jpg", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "erro" << std::endl;
        return -1;
    }

    cv::Mat hsv;
    std::vector<cv::Mat> planes;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV_FULL);
    cv::split(hsv, planes);

    //cria a tabela vertical (256 linhas, 1 coluna) exigida pelo OpenCV 4.10
    cv::Mat lut(256, 1, CV_8UC3);
    for (int i = 0; i < 256; i++) {
        lut.at<cv::Vec3b>(i, 0)[0] = 255 - i; // B: Inverso da intensidade
        lut.at<cv::Vec3b>(i, 0)[1] = i;       // G: Acompanha a intensidade
        lut.at<cv::Vec3b>(i, 0)[2] = 127;     // R: Fixo em 127
    }

    cv::Mat imagecolor;
    cv::applyColorMap(planes[0], imagecolor, lut);

    cv::imwrite("resultado_Hue_Personalizado.png", imagecolor);
    cv::imshow("Hue - Colormap Personalizado", imagecolor);
    cv::waitKey(0);

    return 0;
}