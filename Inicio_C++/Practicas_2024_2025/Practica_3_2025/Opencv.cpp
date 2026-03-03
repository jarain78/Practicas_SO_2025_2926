#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::Mat img = cv::imread("image.jpg");
    if (img.empty()) {
        std::cout << "❌ No se pudo cargar la imagen.\n";
        return -1;
    }

    cv::imshow("Imagen", img);
    cv::waitKey(0);
    return 0;
}


// g++ Practica_3_2025/Opencv.cpp -o opencv -L /opt/homebrew/lib

