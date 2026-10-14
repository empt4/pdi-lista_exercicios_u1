#include <iostream>
#include <opencv2/opencv.hpp>

//variáveis globais para compartilhamento com as funções do mouse e trackbars
cv::Mat original_image, display_image, paint_mask, final_mask;
bool isDrawing = false;
bool isErasing = false;
bool updateStats = false;

cv::Scalar average(128, 128, 128);
cv::Scalar stdev(0, 0, 0);
int rangeR = 0, rangeG = 0, rangeB = 0;

//callbacks vazios apenas para o trackbar funcionar corretamente
void on_trackbar(int, void*) {}

//função que gerencia o "pincel" com o mouse
static void onMouse(int event, int x, int y, int, void*) {
    int radius = 10; // Tamanho do pincel

    if (event == cv::EVENT_LBUTTONDOWN) {
        isDrawing = true;
        cv::circle(paint_mask, cv::Point(x, y), radius, cv::Scalar(255), -1);
    } else if (event == cv::EVENT_RBUTTONDOWN) {
        isErasing = true;
        cv::circle(paint_mask, cv::Point(x, y), radius, cv::Scalar(0), -1);
    } else if (event == cv::EVENT_LBUTTONUP) {
        isDrawing = false;
        updateStats = true; //recalcula os status ao soltar o clique
    } else if (event == cv::EVENT_RBUTTONUP) {
        isErasing = false;
        updateStats = true; //recalcula os status ao soltar o clique
    } else if (event == cv::EVENT_MOUSEMOVE) {
        if (isDrawing) {
            cv::circle(paint_mask, cv::Point(x, y), radius, cv::Scalar(255), -1);
        } else if (isErasing) {
            cv::circle(paint_mask, cv::Point(x, y), radius, cv::Scalar(0), -1);
        }
    }
}

int main(int argc, const char** argv) {
    original_image = cv::imread("noturno_icon.jpg", cv::IMREAD_COLOR);
    if (original_image.empty()) {
        std::cout << "erro" << std::endl;
        return -1;
    }

    //inicia a máscara de pintura (toda preta)
    paint_mask = cv::Mat::zeros(original_image.size(), CV_8UC1);

    cv::namedWindow("Image", cv::WINDOW_NORMAL);
    cv::namedWindow("Selection", cv::WINDOW_NORMAL);
    
    cv::createTrackbar("R-range", "Image", &rangeR, 255, on_trackbar);
    cv::createTrackbar("G-range", "Image", &rangeG, 255, on_trackbar);
    cv::createTrackbar("B-range", "Image", &rangeB, 255, on_trackbar);
    
    cv::setMouseCallback("Image", onMouse, 0);

    std::cout << "INSTRUCOES" << std::endl;
    std::cout << "- Pressione o BOTAO ESQUERDO do mouse para PINTAR e selecionar as cores." << std::endl;
    std::cout << "- Pressione o BOTAO DIREITO do mouse para APAGAR." << std::endl;
    std::cout << "- Pressione a tecla ESC para SALVAR AUTOMATICAMENTE e sair." << std::endl;

    for (;;) {
        //cria um feedback visual verde transparente onde o usuário está pintando
        cv::Mat overlay = original_image.clone();
        overlay.setTo(cv::Scalar(70, 70, 50), paint_mask);
        cv::addWeighted(original_image, 0.7, overlay, 0.3, 0, display_image);

        //se o usuário soltar o clique do mouse, calcula a média e variância da área pintada
        if (updateStats) {
            if (cv::countNonZero(paint_mask) > 0) {
                //meanStdDev aceita uma máscara para calcular os dados apenas onde está pintado
                cv::meanStdDev(original_image, average, stdev, paint_mask);
                
                //atualiza as barras de tolerância (3 * desvio padrão)
                rangeB = cv::saturate_cast<int>(3 * stdev[0]);
                rangeG = cv::saturate_cast<int>(3 * stdev[1]);
                rangeR = cv::saturate_cast<int>(3 * stdev[2]);
                
                cv::setTrackbarPos("B-range", "Image", rangeB);
                cv::setTrackbarPos("G-range", "Image", rangeG);
                cv::setTrackbarPos("R-range", "Image", rangeR);
            }
            updateStats = false;
        }

        //seleciona os pixels na imagem final baseando-se na cor média e na tolerância
        cv::inRange(original_image, 
                    average - cv::Scalar(rangeB, rangeG, rangeR),
                    average + cv::Scalar(rangeB, rangeG, rangeR), 
                    final_mask);

        cv::imshow("Image", display_image);
        cv::imshow("Selection", final_mask);

        char c = cv::waitKey(10);
        if (c == 27) { // 27 = Tecla ESC
            break;
        }
    }

    //salva as imagens ao sair
    cv::imwrite("resultado_pintura.png", display_image);
    cv::imwrite("resultado_mascara.png", final_mask);
    std::cout << "Arquivos 'resultado_pintura.png' e 'resultado_mascara.png' salvos com sucesso!" << std::endl;

    return 0;
}