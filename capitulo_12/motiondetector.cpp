#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    // Tenta abrir a webcam padrao do computador (indice 0)
    cv::VideoCapture cap(0);
    
    // Caso nao detecte webcam, usa o video gravado como fallback
    if (!cap.isOpened()) {
        std::cout << "Webcam nao detectada. Abrindo o arquivo de video 'light.mp4'..." << std::endl;
        cap.open("light.mp4");
        if (!cap.isOpened()) {
            cap.open("../capitulo_9/light.mp4");
        }
        if (!cap.isOpened()) {
            std::cout << "erro ao abrir a webcam ou video" << std::endl;
            return -1;
        }
    }

    cv::Mat frame, gray, hist_curr, hist_prev;
    int nbins = 64;
    float range[] = {0, 256};
    const float *histrange = { range };
    bool uniform = true; 
    bool accumulate = false;

    std::cout << "Pressione ESC na janela do video para salvar e sair." << std::endl;

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            continue;
        }

        //calcula e normaliza o histograma do quadro atual em tons de cinza
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::calcHist(&gray, 1, 0, cv::Mat(), hist_curr, 1, &nbins, &histrange, uniform, accumulate);
        cv::normalize(hist_curr, hist_curr, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

        if (!hist_prev.empty()) {
            //compara o histograma atual com o anterior usando a distancia de Bhattacharyya
            double diferenca = cv::compareHist(hist_prev, hist_curr, cv::HISTCMP_BHATTACHARYYA);
            
            //dispara o alarme visual se a variacao ultrapassar o limiar
            if (diferenca > 0.05) { 
                cv::putText(frame, "MOVIMENTO DETECTADO!", cv::Point(50, 50), 
                            cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 3);
                cv::circle(frame, cv::Point(30, 40), 10, cv::Scalar(0, 0, 255), -1);
            }
        }
        
        //guarda o histograma atual para comparar no proximo quadro
        hist_prev = hist_curr.clone();

        cv::imshow("Detector de Movimento (Pressione ESC para sair)", frame);

        if (cv::waitKey(30) == 27) break;
    }
    
    cv::imwrite("resultado_motion.png", frame);
    std::cout << "Imagem 'resultado_motion.png' salva com sucesso!" << std::endl;
    
    return 0;
}