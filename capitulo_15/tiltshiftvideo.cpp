#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    cv::VideoCapture cap("vasos720p.mp4");
    if (!cap.isOpened()) {
        std::cout << "erro ao abrir o video" << std::endl;
        return -1;
    }

    //le as propriedades do vídeo original
    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(cv::CAP_PROP_FPS);

    //configura a gravação do novo vídeo (.avi com codec MJPG é seguro no WSL)
    cv::VideoWriter writer("miniature_video.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 
                           fps, cv::Size(width, height));

    cv::Mat frame, blurred, result;
    result.create(height, width, CV_8UC3);

    //configuração fixa do Tilt-Shift para o vídeo
    int center = 50; //50% (Centro da tela)
    int focus_height = 30; //30% da altura em foco
    int decay = 15; //suavidade

    int l1 = (center * height / 100) - (focus_height * height / 200);
    int l2 = (center * height / 100) + (focus_height * height / 200);

    //pré-calcula os valores de alpha para cada linha
    std::vector<double> alpha_weights(height);
    for (int i = 0; i < height; i++) {
        double a = 0.5 * (tanh((i - l1) / (double)decay) - tanh((i - l2) / (double)decay));
        alpha_weights[i] = std::max(0.0, std::min(1.0, a));
    }

    int frame_count = 0;
    int frames_escritos = 0;

    std::cout << "Processar o video com efeito Stop Motion e Tilt-Shift..." << std::endl;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;
        frame_count++;

        //pula 3 quadros e processa o 4 para simular "Stop Motion"
        if (frame_count % 4 != 0) continue; 

        //aplica o borramento forte
        cv::blur(frame, blurred, cv::Size(21, 21));

        //aplica as máscaras alpha calculadas anteriormente
        for (int i = 0; i < height; i++) {
            cv::addWeighted(frame.row(i), alpha_weights[i], 
                            blurred.row(i), 1.0 - alpha_weights[i], 0.0, result.row(i));
        }

        writer.write(result);
        frames_escritos++;
    }

    std::cout << "Processamento concluido!" << std::endl;
    std::cout << "O video final foi guardado como 'miniature_video.avi' (" << frames_escritos << " quadros)." << std::endl;

    return 0;
}