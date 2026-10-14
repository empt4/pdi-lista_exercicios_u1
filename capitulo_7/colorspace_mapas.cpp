#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

int main(int argc, char** argv) {
    // Carrega a imagem colorida
    cv::Mat image = cv::imread("../noturno_icon.jpg", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "erro" << std::endl;
        return -1;
    }

    cv::Mat hsv;
    std::vector<cv::Mat> planes;
    
    //converte para HSV e separa os canais (planes[0] = Hue, planes[1] = Saturation, planes[2] = Value)
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV_FULL);
    cv::split(hsv, planes);

    std::vector<int> mapas = {
        cv::COLORMAP_JET, cv::COLORMAP_HOT, cv::COLORMAP_COOL,
        cv::COLORMAP_SPRING, cv::COLORMAP_VIRIDIS
    };
    std::vector<std::string> nomes = {
        "Jet", "Hot", "Cool", "Spring", "Viridis"
    };

    cv::Mat imagecolor;
    for (size_t i = 0; i < mapas.size(); i++) {
        //aplica o mapa de cores no canal Hue (planes[0])
        cv::applyColorMap(planes[0], imagecolor, mapas[i]);
        
        std::string nome_arquivo = "resultado_Hue_" + nomes[i] + ".png";
        cv::imwrite(nome_arquivo, imagecolor); //salva automaticamente
        
        cv::imshow("Hue com Colormap: " + nomes[i], imagecolor);
        std::cout << "Salvo: " << nome_arquivo << " (Pressione uma tecla)" << std::endl;
        
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    return 0;
}