
#include "misc.h"

#include <opencv2/opencv.hpp>


cv::Mat convert_to_binary(const cv::Mat& imgBGR, double threshold) {
    assert(imgBGR.type() == CV_8UC3);
    assert(threshold >= 0.0 && threshold <= 255.0);
    
    cv::Mat binary, grey;
    
    cv::cvtColor(imgBGR, grey, CV_BGR2GRAY);
    cv::threshold(grey, binary, threshold, 255.0, cv::THRESH_BINARY_INV);
    
    return binary;
}


std::vector<std::vector<cv::Point>> find_contours(cv::Mat& binary, int min_size) {
    assert(binary.type() == CV_8UC1);
    
    std::vector<std::vector<cv::Point>> contours, result;
    std::vector<cv::Vec4i> hierarchy;
    
    cv::findContours(binary, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    
    for (size_t i = 0; i < hierarchy.size(); i++) {
        if (hierarchy[i][3] == -1 &&                // use only 'outer' contours (with no parents)
            int(contours[i].size()) >= min_size) {  // with proper size
            result.push_back(contours[i]);
        }
    }
    
    return result;
}
