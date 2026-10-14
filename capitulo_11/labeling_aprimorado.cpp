#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

int main(int argc, char** argv) {
    cv::Mat image = cv::imread("bolhas.png", cv::IMREAD_GRAYSCALE);
    if (!image.data) {
        std::cout << "erro\n";
        return (-1);
    }

    int width = image.cols;
    int height = image.rows;

    //varre a primeira e última linha
    for (int i = 0; i < width; i++) {
        if (image.at<uchar>(0, i) == 255) 
            cv::floodFill(image, cv::Point(i, 0), 0);
        if (image.at<uchar>(height - 1, i) == 255) 
            cv::floodFill(image, cv::Point(i, height - 1), 0);
    }
    //varre a primeira e última coluna
    for (int i = 0; i < height; i++) {
        if (image.at<uchar>(i, 0) == 255) 
            cv::floodFill(image, cv::Point(0, i), 0);
        if (image.at<uchar>(i, width - 1) == 255) 
            cv::floodFill(image, cv::Point(width - 1, i), 0);
    }

    //pinta o fundo da imagem (comecando do ponto 0,0) com a cor 1
    //o fundo fica = 1, bolhas = 255, e os buracos internos ficam isolados mantendo o valor 0
    cv::floodFill(image, cv::Point(0, 0), 1);

    int label = 2; //inicia a rotulação no 2 (0 e 1 já estão em uso)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at<uchar>(i, j) == 255) {
                cv::floodFill(image, cv::Point(j, i), label);
                label++;
            }
        }
    }

    //array de booleanos para rastrear quais bolhas tem furos
    std::vector<bool> tem_furo(label, false);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Se encontrar um pixel 0, é um buraco!
            if (image.at<uchar>(i, j) == 0) {
                //pixel a esquerda (j-1) obrigatoriamente pertence a bolha que envolve o buraco
                int bolha_pai = image.at<uchar>(i, j - 1);
                tem_furo[bolha_pai] = true;
                
                //preenche o buraco com a cor de fundo (1) para não voltar a contar ele (trata bolhas com >1 furo)
                cv::floodFill(image, cv::Point(j, i), 1);
            }
        }
    }

    int com_furo = 0;
    int sem_furo = 0;
    
    //começa do rótulo 2 até ao último criado
    for (int i = 2; i < label; i++) {
        if (tem_furo[i]) com_furo++;
        else sem_furo++;
    }

    std::cout << "==== RESULTADOS DA CONTAGEM ====" << std::endl;
    std::cout << "Bolhas com furo: " << com_furo << std::endl;
    std::cout << "Bolhas sem furo: " << sem_furo << std::endl;
    std::cout << "Total de bolhas válidas: " << com_furo + sem_furo << std::endl;

    //ajusta o brilho visual para o resultado final não ficar totalmente escuro
    cv::Mat realce;
    image.convertTo(realce, CV_32F);
    cv::normalize(realce, realce, 0, 255, cv::NORM_MINMAX);
    realce.convertTo(realce, CV_8U);

    cv::imwrite("resultado_labeling.png", realce);
    cv::imshow("Bolhas Filtradas", realce);
    
    std::cout << "\nImagem guardada como 'resultado_labeling.png'. Pressione uma tecla para sair..." << std::endl;
    cv::waitKey(0);

    return 0;
}