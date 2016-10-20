#pragma once

#include <opencv2/core/core.hpp>
#include <vector>
#include <numeric>


// Convert 24-bit BGR image into 8-bit black-and-white one
cv::Mat convert_to_binary(const cv::Mat& imgBGR, double threshold = 64.0);


// Extract all contours
std::vector<std::vector<cv::Point>> find_contours(cv::Mat& binary, int min_size = 0);


template <typename T2, typename T1>
auto convert(const std::vector<T1>& vec) -> std::vector<T2> {
    std::vector<T2> result(vec.size());
    
    std::transform(begin(vec), end(vec), begin(result),
        [](const T1& x) { return T2(x); }
    );
    
    return result;
}


template <typename T>
auto mean(const std::vector<T>& vec) -> T {
    assert(vec.size() > 0);
    return std::accumulate(begin(vec), end(vec), T(0)) / vec.size();
}


template <typename T>
auto median(std::vector<T> vec) -> float {
    assert(vec.size() > 0);
    
    const size_t N = vec.size();
    std::nth_element(begin(vec), begin(vec) + N / 2, end(vec));
    float m = vec[N / 2];
    
    if (N % 2 == 0) {
        m = 0.5f * (m + *std::min_element(begin(vec) + N / 2 + 1, end(vec)));
    }
    
    return m;
}


template <typename T, typename Fun, typename R = float>
auto filter(const std::vector<T>& vec, size_t sample_size, Fun&& f) -> std::vector<R> {
    assert(sample_size <= int(vec.size()));
    assert(sample_size > 1);
    
    sample_size = sample_size & ~1;
    const int half_sample = sample_size / 2;
    
    std::vector<R> result(vec.size());
    std::vector<T> sample(sample_size);
    
    // populate initial sample
    std::copy_n(end(vec) - half_sample, half_sample, begin(sample));
    std::copy_n(begin(vec), half_sample, begin(sample) + half_sample);
    
    for (size_t i = 0; i < vec.size(); i++) {
        // update the oldest sample element
        sample[i % sample_size] = vec[(i + half_sample) % vec.size()];
        result[i] = f(sample);
    }
    
    return result;
}


template <typename T>
auto box_filter(const std::vector<T>& vec, int sample_size) -> std::vector<T> {
    return filter(vec, sample_size, mean<T>);
}
