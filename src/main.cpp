
#include "polygon_fit.h"
#include "misc.h"

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>


void test(const std::string& filename, int n_vertices);

int main(int argc, char* argv[]) {
    
    test("Triangle1.png", 3);
    
    test("Quadrangle1.png", 4);
    test("Quadrangle2.png", 4);
    
    test("Pentagon1.png", 5);
    test("Pentagon2.png", 5);
    
    test("Polygon8.png", 8);
    test("Polygon11.png", 11);
    test("Polygon17.png", 17);
    test("Polygon18.png", 18);
    
    return 0;
}


void test(const std::string& filename, int n_vertices) {
    auto img = cv::imread("./imgs/" + filename);
    
    // check if the file has been loaded properly
    if (!img.data) {
        std::cerr << "File " << filename << " could not be loaded\n";
        return;
    }
    
    auto binary = convert_to_binary(img);
    auto contours = find_contours(binary);
    
    for (auto& contour : contours) {
        if (contour.size() < 25) continue;
        
        auto polygon = fit_polygon(convert<cv::Point2f>(contour), n_vertices);
        
        for (size_t i = 0; i < polygon.size(); i++) {
            cv::line(img, polygon[i], polygon[(i + 1) % polygon.size()], CV_RGB(0, 127, 255), 3, CV_AA);
        }
        
        for (size_t i = 0; i < polygon.size(); i++) {
            cv::circle(img, polygon[i], 6, CV_RGB(255, 127, 0), -1, CV_AA);
        }
    }
    
    cv::imshow(filename, img);
    cv::waitKey(0);
}
