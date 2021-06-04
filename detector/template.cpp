// Copyright 2020 by Orzhekh Evgenii

#include "template.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

dt::Template::Template(const cv::Mat& src, const std::string& name)
    : image_(src)
	, name_(name) {
	if (image_.empty()) {
		throw std::runtime_error("Can't load template image.");
	}
}

dt::Template::Template(const std::string& src_path, const std::string& name)
	: name_(name) {
	cv::imread(src_path).copyTo(image_);
	if (image_.empty()) {
		throw std::runtime_error("Can't load template image.");
	}
}

cv::Mat dt::Template::image() const{
	if (image_.empty()) {
		throw std::logic_error("Template is empty.");
	}
	return image_;
}

void dt::Template::image(const cv::Mat img) {
	if (img.empty()) {
		throw std::logic_error("Template is empty.");
	}
	image_ = img;
}


void points_to_rect(int cols, int rows, std::vector<cv::Point>& input, std::vector<cv::Rect>& output){
	for (auto point : input) {
		output.push_back(cv::Rect(point, cv::Size(cols, rows)));
	}
}

void dt::config(const std::string& main_dir, const std::string& config_name, std::vector<dt::Template>& output){
	std::ifstream fin(main_dir + config_name);
	if (fin.is_open()) {
		std::string buff, path, name;
		while (0 == getline(fin, buff).fail()) {
			std::istringstream sin(buff);
			sin >> name >> path;
			if (sin) { 
				try {
					Template t(main_dir + path, name);
					output.push_back(t);
				} 
				catch (const std::exception& er) {
					std::cerr << name << " " << er.what() << "\n";
					continue;
				}
			}
		}
		fin.close();
	}
	else {
		throw std::runtime_error("Can't open file: " + config_name);
	}
}

void detect(const cv::Mat& img, const cv::Mat& temp, std::vector<cv::Rect>& output, int limit, double threshold) {
	cv::Mat img_gray, temp_gray, result, bin_result;
	std::vector<cv::Point> points;

	cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
	cv::cvtColor(temp, temp_gray, cv::COLOR_BGR2GRAY);
	cv::matchTemplate(img_gray, temp_gray, result, cv::TM_CCORR_NORMED);
	cv::normalize(result, result, 0, 1, cv::NORM_MINMAX);
	cv::threshold(result, result, threshold, 1, cv::THRESH_BINARY);
	result.convertTo(bin_result, CV_8UC1);
	cv::findNonZero(bin_result, points);

	if (points.empty()) {
		std::cerr << "Haven't found this template on the image." << std::endl;
		return;
	}
	if (points.size() > limit) {
		points.erase(points.begin(), points.end());
		std::cerr << "Haven't found this template on the image." << std::endl;
		return;
	}
	points_to_rect(temp.cols, temp.rows, points, output);
}

void dt::match(const cv::Mat& img, const dt::Template& temp, dt::Result& res, int limit, double threshold) {
	cv::Mat temp_t{ temp.image() };
	std::vector<cv::Rect> temp_p;

	detect(img, temp_t, temp_p, limit, threshold);

	res.image_rect(cv::Rect(0, 0, img.cols, img.rows));
	res.set_regions(temp_p);
	res.name(temp.name());
}

void dt::match(const cv::Mat& img, const std::vector<dt::Template>& input, std::vector<dt::Result>& output, int limit, double threshold) {
	if (input.empty()) {
		throw std::logic_error("There are no templates in input vector.");
	}
	for (auto temp: input) {
		dt::Result res;
		match(img, temp, res, limit, threshold);
		output.push_back(res);
	}
}
