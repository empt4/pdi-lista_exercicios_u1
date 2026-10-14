#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "uso: ./decodificador <imagem_codificada.png>" << std::endl;
        return -1;
    }

    //carrega a imagem que tem o segredo
    cv::Mat imagemCodificada = cv::imread(argv[1], cv::IMREAD_COLOR);
    if (imagemCodificada.empty()) {
        std::cout << "Erro ao abrir a imagem!" << std::endl;
        return -1;
    }

    //cria uma matriz vazia para guardar a imagem revelada
    cv::Mat imagemRecuperada = cv::Mat::zeros(imagemCodificada.size(), imagemCodificada.type());
    
    int nbits = 3; 

    //percorre todos os píxeis da imagem
    for (int i = 0; i < imagemCodificada.rows; i++) {
        for (int j = 0; j < imagemCodificada.cols; j++) {
            cv::Vec3b valCodificada = imagemCodificada.at<cv::Vec3b>(i, j);
            cv::Vec3b valRecuperada;

            //pego o byte original e uso um "Shift Left" (<<) de 5 posições (8 - 3).
            //isso leva os 3 bits escondidos do fundo para o topo do byte, 
            //e descarta automaticamente a imagem portadora que estava no topo.
            valRecuperada[0] = valCodificada[0] << (8 - nbits);
            valRecuperada[1] = valCodificada[1] << (8 - nbits);
            valRecuperada[2] = valCodificada[2] << (8 - nbits);

            imagemRecuperada.at<cv::Vec3b>(i, j) = valRecuperada;
        }
    }

    //salva o resultado na pasta
    cv::imwrite("imagem_revelada.png", imagemRecuperada);

    //mostra o resultado
    cv::imshow("Segredo Revelado", imagemRecuperada);
    std::cout << "Imagem revelada com sucesso e salva como 'imagem_revelada.png'!" << std::endl;
    cv::waitKey(0);

    return 0;
}