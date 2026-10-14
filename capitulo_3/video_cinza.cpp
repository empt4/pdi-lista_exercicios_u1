#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Uso: ./video_cinza <caminho_do_video>" << std::endl;
        return -1;
    }

    cv::VideoCapture cap(argv[1]);
    if (!cap.isOpened()) return -1;

    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int type = cap.get(cv::CAP_PROP_FOURCC);
    double fps = cap.get(cv::CAP_PROP_FPS);
    cv::Size frameSize(width, height);

    //'false' indica que a gravação será em tons de cinza (1 canal)
    cv::VideoWriter out("saida_cinza.avi", type, fps, frameSize, false);

    cv::Mat frame, frame_cinza;
    while (cap.read(frame)) {
        //converte o frame colorido para tons de cinza
        cv::cvtColor(frame, frame_cinza, cv::COLOR_BGR2GRAY);
        
        out << frame_cinza; //grava o frame cinza no arquivo
        cv::imshow("Video em Tons de Cinza", frame_cinza);
        
        if (cv::waitKey(30) >= 0) break;
    }
    return 0;
}