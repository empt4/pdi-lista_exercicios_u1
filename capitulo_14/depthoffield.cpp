#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    cv::VideoCapture cap("vasos720p.mp4");
    if (!cap.isOpened()) {
        std::cout << "erro ao abrir o video" << std::endl;
        return -1;
    }

    cv::Mat frame, gray, lap, abs_lap;
    cv::Mat max_lap, output_image;
    
    //mascara do filtro laplaciano
    float laplacian_data[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
    cv::Mat mask(3, 3, CV_32F, laplacian_data);

    bool first_frame = true;
    int frame_count = 0;

    std::cout << "Processando video quadro a quadro" << std::endl;

    while (true) {
        cap >> frame;
        if (frame.empty()) break; //fim do vídeo

        if (first_frame) {
            //inicia matriz de máximos com zeros
            max_lap = cv::Mat::zeros(frame.rows, frame.cols, CV_32F);
            output_image = frame.clone();
            first_frame = false;
        }

        //converte para cinza e depois para float para aplicar o filtro
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        gray.convertTo(gray, CV_32F);

        //aplica o filtro laplaciano
        cv::filter2D(gray, lap, gray.depth(), mask, cv::Point(1, 1), 0, cv::BORDER_REPLICATE);
        
        //nitidez é medida pela magnitude do laplaciano (valor absoluto)
        abs_lap = cv::abs(lap);

        //compara pixel a pixel
        for (int i = 0; i < frame.rows; i++) {
            for (int j = 0; j < frame.cols; j++) {
                //se pixel atual for mais nítido do que o registado anteriormente
                if (abs_lap.at<float>(i, j) > max_lap.at<float>(i, j)) {
                    //atualiza o recorde de nitidez
                    max_lap.at<float>(i, j) = abs_lap.at<float>(i, j);
                    //copia o pixel colorido do quadro atual para a imagem final
                    output_image.at<cv::Vec3b>(i, j) = frame.at<cv::Vec3b>(i, j);
                }
            }
        }
        frame_count++;
    }

    std::cout << "Processamento concluido! Lidos " << frame_count << " frames." << std::endl;
    
    cv::imwrite("resultado_foco.png", output_image);
    cv::imshow("Imagem Final Totalmente Focada", output_image);
    
    std::cout << "Imagem salva como 'resultado_foco.png'. Pressione qualquer tecla para sair." << std::endl;
    cv::waitKey(0);

    return 0;
}