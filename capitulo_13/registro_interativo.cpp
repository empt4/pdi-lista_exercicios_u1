#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

//variáveis globais para compartilhar com o callback do mouse
cv::Mat original_image, display_image;
std::vector<cv::Point2f> srcPoints;

//função chamada sempre que o rato interage com a janela
void onMouse(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        //processa se ainda não tivermos os 4 pontos
        if (srcPoints.size() < 4) {
            srcPoints.push_back(cv::Point2f(x, y));
            
            //desenha um pequeno círculo vermelho onde o usuário clicou
            cv::circle(display_image, cv::Point(x, y), 4, cv::Scalar(0, 0, 255), -1);
            cv::imshow("Original - Clique nos 4 cantos", display_image);

            //quando o 4 ponto for clicado calcula a transformação
            if (srcPoints.size() == 4) {
                std::cout << "\n4 pontos registados, calcular perspetiva" << std::endl;

                //calcula a largura (maior distância horizontal)
                float largura = cv::max(cv::norm(srcPoints[0] - srcPoints[1]), 
                                        cv::norm(srcPoints[3] - srcPoints[2]));

                //calcula a altura (maior distância vertical)
                float altura = cv::max(cv::norm(srcPoints[1] - srcPoints[2]), 
                                       cv::norm(srcPoints[0] - srcPoints[3]));

                //define os pontos de destino (um retângulo perfeito)
                std::vector<cv::Point2f> dstPoints;
                dstPoints.push_back(cv::Point2f(0, 0));                   //superior esquerdo
                dstPoints.push_back(cv::Point2f(largura - 1, 0));         //superior direito
                dstPoints.push_back(cv::Point2f(largura - 1, altura - 1));//inferior direito
                dstPoints.push_back(cv::Point2f(0, altura - 1));          //inferior esquerdo

                //matriz de transformação
                cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(srcPoints, dstPoints);
                
                //distorção na imagem
                cv::Mat correctedImage;
                cv::warpPerspective(original_image, correctedImage, perspectiveMatrix, cv::Size(largura, altura));

                //ostra e salva o resultado final
                cv::imshow("Imagem Corrigida", correctedImage);
                cv::imwrite("resultado_voltimetro.png", correctedImage);
                std::cout << "Imagem corrigida salva como 'resultado_voltimetro.png' na pasta build." << std::endl;
                std::cout << "Pressione ESC para sair." << std::endl;
            }
        }
    }
}

int main(int argc, char** argv) {
    original_image = cv::imread("../voltimetro.png");
    if (original_image.empty()) {
        std::cout << "erro ao carregar a imagem" << std::endl;
        return -1;
    }

    display_image = original_image.clone();

    cv::namedWindow("Original - Clique nos 4 cantos", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Original - Clique nos 4 cantos", onMouse, NULL);

    std::cout << "MODO DE USO" << std::endl;
    std::cout << "Clique nos 4 cantos do visor nesta ordem:" << std::endl;
    std::cout << "1. Canto Superior Esquerdo" << std::endl;
    std::cout << "2. Canto Superior Direito" << std::endl;
    std::cout << "3. Canto Inferior Direito" << std::endl;
    std::cout << "4. Canto Inferior Esquerdo" << std::endl;

    cv::imshow("Original - Clique nos 4 cantos", display_image);

    while (true) {
        if (cv::waitKey(30) == 27) break; //sai ao pressionar ESC
    }

    return 0;
}