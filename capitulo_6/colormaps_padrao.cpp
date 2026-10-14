#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

int main(int argc, char** argv) {
    cv::Mat image = cv::imread("noturno_icon.jpg", cv::IMREAD_GRAYSCALE);
    if (!image.data) {
        std::cout << "erro" << std::endl;
        return -1;
    }

    std::vector<int> mapas = {
        cv::COLORMAP_AUTUMN,
        cv::COLORMAP_BONE,
        cv::COLORMAP_WINTER,
        cv::COLORMAP_RAINBOW,
        cv::COLORMAP_OCEAN
    };
    std::vector<std::string> nomes = {
        "Autumn", "Bone", "Winter", "Rainbow", "Ocean"
    };

    cv::Mat imagecolor;
    for (size_t i = 0; i < mapas.size(); i++) {
        cv::applyColorMap(image, imagecolor, mapas[i]);
        cv::imshow("Colormap: " + nomes[i], imagecolor);
        
        //salva a imagem automaticamente na pasta
        std::string nome_arquivo = "resultado_" + nomes[i] + ".png";
        cv::imwrite(nome_arquivo, imagecolor);
        
        std::cout << "A exibir e salvar: " << nome_arquivo << " (Pressione uma tecla para avancar...)" << std::endl;
        
        cv::waitKey(0);
        cv::destroyAllWindows();
    }

    return 0;
}