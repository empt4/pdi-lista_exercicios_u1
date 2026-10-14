#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

cv::Mat image, blurred, result;

//valores iniciais (em porcentagem)
int center_slider = 50;
int height_slider = 30;
int decay_slider = 10;

void applyTiltShift(int, void*) {
    //evita divisão por zero no decaimento
    int d = std::max(decay_slider, 1); 
    
    //converte as porcentagens dos trackbars para as posições reais em pixels
    int l1 = (center_slider * image.rows / 100) - (height_slider * image.rows / 200);
    int l2 = (center_slider * image.rows / 100) + (height_slider * image.rows / 200);

    //processa linha a linha combinando a imagem original e a borrada
    for (int i = 0; i < image.rows; i++) {
        //função de ponderação baseada na tangente hiperbólica
        double alpha = 0.5 * (tanh((i - l1) / (double)d) - tanh((i - l2) / (double)d));
        
        //garante que o alpha fique entre 0 e 1
        alpha = std::max(0.0, std::min(1.0, alpha));
        
        //mistura a linha original com a linha borrada de acordo com o peso alpha
        cv::addWeighted(image.row(i), alpha, blurred.row(i), 1.0 - alpha, 0.0, result.row(i));
    }

    cv::imshow("Efeito Tilt-Shift", result);
}

int main(int argc, char** argv) {
    image = cv::imread("noturno_icon.jpg");
    if (image.empty()) {
        std::cout << "Erro ao abrir a imagem 'noturno_icon.jpg'." << std::endl;
        return -1;
    }

    result = image.clone();
    
    //cria a versão fortemente borrada da imagem (filtro da média grande)
    cv::blur(image, blurred, cv::Size(21, 21));

    cv::namedWindow("Efeito Tilt-Shift", cv::WINDOW_AUTOSIZE);

    //cria as 3 barras de controle exigidas pelo exercício
    cv::createTrackbar("Centro Vertical %", "Efeito Tilt-Shift", &center_slider, 100, applyTiltShift);
    cv::createTrackbar("Altura do Foco %", "Efeito Tilt-Shift", &height_slider, 100, applyTiltShift);
    cv::createTrackbar("Decaimento (Suavidade)", "Efeito Tilt-Shift", &decay_slider, 50, applyTiltShift);

    //primeira chamada para gerar a imagem inicial
    applyTiltShift(0, 0);

    std::cout << "Ajuste os controles deslizantes." << std::endl;
    std::cout << "Pressione ESC para salvar o resultado e sair." << std::endl;

    while (true) {
        if (cv::waitKey(30) == 27) break;
    }

    cv::imwrite("resultado_tiltshift.png", result);
    std::cout << "Arquivo 'resultado_tiltshift.png' salvo com sucesso!" << std::endl;

    return 0;
}