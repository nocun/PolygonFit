#pragma once

#include <opencv2/core/core.hpp>


struct line {
    cv::Point2f pt;
    cv::Vec2f vec;
};

float distance(const line& line, const cv::Point2f& point);

line fit_line(const std::vector<cv::Point2f>& points);

float fitness_error(const line& line, const std::vector<cv::Point2f>& points);
