
#include "line.h"
#include "misc.h"

#include <opencv2/opencv.hpp>


float distance(const line& line, const cv::Point2f& point) {
    assert(cv::norm(line.vec) > 0.0f);
    
    auto A = line.pt;
    auto B = A + cv::Point2f{ line.vec };
    auto C = point;
    
    auto AB = B - A;
    auto AC = C - A;
    
    return float(std::abs(AB.cross(AC) / cv::norm(AB)));
}


line fit_line(const std::vector<cv::Point2f>& points) {
    cv::Vec4f params;
    line result;
    
    cv::fitLine(points, params, CV_DIST_L2, 0.0, 0.01, 0.01);
    
    result.pt = { params[2], params[3] };
    result.vec = { params[0], params[1] };
    
    return result;
}


float fitness_error(const line& line, const std::vector<cv::Point2f>& points) {
    std::vector<float> distances(points.size());
    
    std::transform(begin(points), end(points), begin(distances),
        [&line](const cv::Point2f& point) {
            return distance(line, point);
        }
    );
    
    return median(distances);
}
