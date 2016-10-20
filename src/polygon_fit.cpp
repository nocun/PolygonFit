
#include "polygon_fit.h"
#include "line.h"
#include "misc.h"

#include <opencv2/opencv.hpp>
#include <vector>
#include <numeric>
#include <cassert>


static const int MIN_SIDE_SIZE      = 5;
static const int MIN_VERTICES       = 3;
static const int MIN_CONTOUR_SIZE   = 25;


std::vector<float> calculate_global_fitness_error(const std::vector<cv::Point2f>& contour, int sample_size, int filter_size) {
    auto transformation = [](const std::vector<cv::Point2f>& points) -> float {
        return fitness_error(fit_line(points), points);
    };
    
    auto error_value = filter(contour, sample_size, transformation);
    
    return box_filter(error_value, filter_size);
}


std::vector<int> get_peaks_indices(const std::vector<float>& fitness, int filter_size, float min_fitness) {
    std::vector<int> taken(fitness.size()), result, peaks;
    
    const int N = int(fitness.size());
    const int half_sample = filter_size / 2;
    
    for (int i = 0; i < N; i++) {
        int previous = (i - 1 + N) % N;
        int next = (i + 1) % N;
        
        if (fitness[i] > fitness[previous] &&
            fitness[i] > fitness[next] &&
            fitness[i] > min_fitness) {
            peaks.push_back(i);
        }
    }
    
    for (int peak : peaks) {
        if (taken[peak]) continue;
        
        result.push_back(peak);
        
        // mark the peak and its neighbourhood as used
        for (int j = -half_sample; j <= half_sample; j++) {
            int index = (peak + j + N) % N;
            taken[index] = 1;
        }
    }
    
    return result;
}


bool is_input_valid(const size_t contour_size, int n_vertices, int side_size) {
    return  contour_size >= MIN_CONTOUR_SIZE &&
            side_size >= MIN_SIDE_SIZE &&
            n_vertices >= 3;
}


std::vector<cv::Point2f> fit_polygon(const std::vector<cv::Point2f>& contour, int n_vertices, int side_size) {
    if (!is_input_valid(contour.size(), n_vertices, side_size)) {
        assert(false && "fit_polygon - invalid arguments");
        return{};
    }
    
    int sample_size = std::min(side_size, int(contour.size()));
    int filter_size = int(std::sqrt(sample_size));
    
    auto fitness_error = calculate_global_fitness_error(contour, sample_size, filter_size);
    auto peaks = get_peaks_indices(fitness_error, filter_size, 0.0f);
    
    // extract only those n vertices with highest fitness error
    n_vertices = std::min(n_vertices, int(peaks.size()));
    
    std::partial_sort(begin(peaks), begin(peaks) + n_vertices, end(peaks),
        [&](int a, int b) { return fitness_error[a] > fitness_error[b]; }
    );
    
    peaks.erase(begin(peaks) + n_vertices, end(peaks));
    
    // restore initial order - sort by index
    std::sort(begin(peaks), end(peaks));
    
    std::vector<cv::Point2f> result(n_vertices);
    
    std::transform(begin(peaks), end(peaks), begin(result),
        [&contour](int index) { return contour[index]; }
    );
    
    return result;
}


std::vector<cv::Point2f> fit_polygon(const std::vector<cv::Point2f>& contour, int n_vertices) {
    const float SIDE_SIZE_FACTOR = 0.7f;
    int side_size = int(SIDE_SIZE_FACTOR * int(contour.size()) / n_vertices);
    
    return fit_polygon(contour, n_vertices, side_size);
}


std::vector<cv::Point2f> fit_polygon_auto(const std::vector<cv::Point2f>& contour, int side_size) {
    if (!is_input_valid(contour.size(), MIN_VERTICES, side_size)) {
        assert(false && "fit_polygon_auto - invalid arguments");
        return{};
    }
    
    int sample_size = std::min(side_size, int(contour.size()));
    int filter_size = int(std::sqrt(sample_size));
    
    auto fitness_error = calculate_global_fitness_error(contour, sample_size, filter_size);
    auto peaks = get_peaks_indices(fitness_error, filter_size, mean(fitness_error));
    
    std::vector<cv::Point2f> result(peaks.size());
    
    std::transform(begin(peaks), end(peaks), begin(result),
        [&contour](int index) { return contour[index]; }
    );
    
    return result;
}
