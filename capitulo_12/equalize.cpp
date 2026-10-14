#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    //tenta abrir a webcam padrao do computador (indice 0)
    cv::VideoCapture cap(0); 
    
    //caso nao detecte webcam usa o video gravado como fallback
    if (!cap.isOpened()) {
        std::cout << "Webcam nao detectada. Abrindo o arquivo de video 'light.mp4'..." << std::endl;
        cap.open("light.mp4"); 
        if (!cap.isOpened()) {
            cap.open("../capitulo_9/light.mp4");
        }
        if (!cap.isOpened()) {
            std::cout << "erro" << std::endl;
            return -1;
        }
    }

    cv::Mat frame, gray, equalized, combined;
    std::cout << "Pressione ESC na janela do video para salvar e sair." << std::endl;

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            //reinicia o video caso chegue ao fim
            cap.set(cv::CAP_PROP_POS_FRAMES, 0); 
            continue;
        }

        //converte para tons de cinza e equaliza o histograma
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, equalized);

        //junta a imagem original e a equalizada lado a lado
        cv::hconcat(gray, equalized, combined);

        cv::imshow("Original (Esquerda) vs Equalizada (Direita)", combined);

        if (cv::waitKey(30) == 27) break; // Sai ao pressionar ESC
    }
    
    cv::imwrite("resultado_equalize.png", combined);
    std::cout << "Imagem 'resultado_equalize.png' salva com sucesso!" << std::endl;
    
    return 0;
}