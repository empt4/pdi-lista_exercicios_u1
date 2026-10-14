#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Uso: ./video_negativo <caminho_do_video>" << std::endl;
        return -1;
    }

    cv::VideoCapture cap(argv[1]);
    if (!cap.isOpened()) return -1;

    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int type = cap.get(cv::CAP_PROP_FOURCC);
    double fps = cap.get(cv::CAP_PROP_FPS);
    cv::Size frameSize(width, height);

    //'true' mantém a gravação colorida (3 canais)
    cv::VideoWriter out("saida_negativo.avi", type, fps, frameSize, true);

    cv::Mat frame, frame_negativo;
    while (cap.read(frame)) {
        //inverte as cores do frame (efeito negativo)
        cv::bitwise_not(frame, frame_negativo);
        
        out << frame_negativo;
        cv::imshow("Video em Negativo", frame_negativo);
        
        if (cv::waitKey(30) >= 0) break;
    }
    return 0;
}