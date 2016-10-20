#pragma once

#include <opencv2/core/core.hpp>
#include <vector>


// Fit polygon given maximum number of vertices
std::vector<cv::Point2f> fit_polygon(const std::vector<cv::Point2f>& contour, int n_vertices);


// Fit polygon given maximum number of vertices and size of the shortest side
std::vector<cv::Point2f> fit_polygon(const std::vector<cv::Point2f>& contour, int n_vertices, int side_size);


// Fit polygon, number of vertices calculated automatically
std::vector<cv::Point2f> fit_polygon_auto(const std::vector<cv::Point2f>& contour, int side_size);
