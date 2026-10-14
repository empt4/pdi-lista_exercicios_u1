#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    //abre o vídeo
    cv::VideoCapture cap("../light.mp4");
    if (!cap.isOpened()) {
        std::cout << "erro ao abrir o vídeo" << std::endl;
        return -1;
    }

    cv::Mat frame;
    cap >> frame; // Lê o primeiro frame
    if (frame.empty()) {
        std::cout << "erro" << std::endl;
        return -1;
    }

    //inicia as imagens acumuladoras com o primeiro frame do vídeo
    cv::Mat result_max = frame.clone();
    cv::Mat result_add = frame.clone();

    int frame_count = 1;

    //loop para ler o vídeo inteiro
    while (true) {
        cap >> frame;
        if (frame.empty()) break; // Fim do vídeo

        //1 Combinação: cv::max 
        //mantém apenas o pixel mais claro entre o acumulado e o frame atual
        result_max = cv::max(result_max, frame);

        //2 combinação (Para Comparação): Operador de Soma (+)
        //soma as matrizes: vai saturar a imagem se o fundo não for preto
        result_add = result_add + frame;

        frame_count++;
    }

    std::cout << "Processamento concluido! Foram lidos " << frame_count << " frames." << std::endl;

    //salva as imagens na pasta build
    cv::imwrite("resultado_lightpainting_max.png", result_max);
    cv::imwrite("resultado_lightpainting_add.png", result_add);

    //mostra na tela
    cv::namedWindow("Light Painting - MAX", cv::WINDOW_NORMAL);
    cv::namedWindow("Light Painting - SOMA", cv::WINDOW_NORMAL);
    cv::imshow("Light Painting - MAX", result_max);
    cv::imshow("Light Painting - SOMA", result_add);

    std::cout << "Arquivos salvos com sucesso! Pressione qualquer tecla na janela da imagem para sair." << std::endl;
    cv::waitKey(0);

    return 0;
}