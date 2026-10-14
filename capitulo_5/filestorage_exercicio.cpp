#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <string>

int SIDE = 256;
int PERIODOS = 4; //alterado para 4 conforme o exercício

int main(int argc, char** argv) {
    cv::Mat image = cv::Mat::zeros(SIDE, SIDE, CV_32FC1);

    //gera a senóide em float
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            //amplitude 127, deslocamento 128 para evitar valores negativos
            image.at<float>(i, j) = 127 * sin(2 * M_PI * PERIODOS * j / SIDE) + 128;
        }
    }

    //grava a imagem float intocada no YAML
    cv::FileStorage fs("senoide-256.yml", cv::FileStorage::WRITE);
    fs << "mat" << image;
    fs.release();

    //converte para 8 bits e grava no PNG
    cv::Mat image_png;
    cv::normalize(image, image_png, 0, 255, cv::NORM_MINMAX);
    image_png.convertTo(image_png, CV_8U);
    cv::imwrite("senoide-256.png", image_png);

    //leitura dos arquivos para comparação
    cv::Mat mat_yaml, mat_png;
    
    //le YAML
    fs.open("senoide-256.yml", cv::FileStorage::READ);
    fs["mat"] >> mat_yaml;
    fs.release();
    
    //le PNG em tons de cinza
    mat_png = cv::imread("senoide-256.png", cv::IMREAD_GRAYSCALE);

    //compara linha central e gravação das diferenças
    std::ofstream file("diferenca.txt");
    int linha_central = SIDE / 2;
    
    for (int j = 0; j < SIDE; j++) {
        float val_yaml = mat_yaml.at<float>(linha_central, j);
        float val_png = (float)mat_png.at<uchar>(linha_central, j);
        
        float diferenca = std::abs(val_yaml - val_png);
        file << diferenca << "\n";
    }
    file.close();
    
    std::cout << "Processamento concluido. Arquivo 'diferenca.txt' gerado na pasta build." << std::endl;

    //mostra a imagem na tela (opcional)
    cv::imshow("Senoide", image_png);
    cv::waitKey();

    return 0;
}