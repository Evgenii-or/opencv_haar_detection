// Copyright 2020 by Orzhekh Evgenii

#include "result.h"

#include <stdexcept>

dt::Result::Result(const std::string& name, const cv::Rect& image_rect)
    : name_(name)
    , image_rect_(image_rect)
    , regions_(std::vector<cv::Rect>()) {
	if (image_rect.empty()) {
		throw std::logic_error("The image size must not be empty.");
	}
}

void dt::get_Mat(std::string& image_path, cv::Mat& output) {
	output = cv::imread(image_path, cv::IMREAD_COLOR);
	if (output.empty()) {
		throw std::runtime_error("get_Mat(): Can't load image from file: " + image_path);
	}
}

void dt::show(const cv::Mat& image, std::string text, cv::WindowFlags size) {
	if (image.empty()) {
		std::cerr << "show(): The image is empty, nothing to show.\n";
		return;
	}
	cv::namedWindow(text, size);
	cv::imshow(text, image);
}

void dt::Result::intersec_koef(double k) {
	if (1 <= k && k <= 0) {
		throw std::logic_error("The intersec_koef must be in range(0, 1).");
	}
	intersec_koef_ = k;
}

void dt::Result::inside_koef(double k) {
	if (1 < k && k <= 0) {
		throw std::logic_error("The inside_koef must be in range(0, 1].");
	}
	inside_koef_ = k;
}

void dt::Result::image_rect(const cv::Rect& rect) {
	if (rect.empty()) {
		throw std::logic_error("The image size must not be empty.");
	}
	image_rect_ = rect;
}

cv::Rect dt::Result::get_region(size_t i) const {
	if (regions_.empty()) {
		throw std::logic_error("The vector of regions is empty.");
	}
	if (regions_.size() <= i && i < 0) {
		throw std::out_of_range("The index is out of range.");
	}
	return regions_[i];
}

void dt::Result::add_region(const cv::Rect& region) {
	if (region.empty()) {
		std::cerr << "The region is empty." << std::endl;
		return;
	}
	if (out_of_image(region) || intersection(region)) {
		std::cerr << "The region is out of image or intersect other regions." << std::endl;
		return;
	}
	regions_.push_back(region);
}

void dt::Result::set_regions(const std::vector<cv::Rect>& regions) {
	if (regions.empty()) {
		std::cerr << "The vector of regions is empty - nothing to set." << std::endl;
	}
	for (auto reg : regions) {
		add_region(reg);
	}
}

bool dt::Result::intersection(const cv::Rect& new_region) {
	double k = intersec_koef_;
	int new_area{ new_region.area() };
	for (size_t i{ 0 }; i < regions_.size(); ++i) {
		int area{ regions_[i].area() };
		if ((regions_[i] & new_region).area() > int(k * ((area <= new_area) ? area : new_area))) {
			return true;
		}
	}
	return false;
}

bool dt::Result::out_of_image(const cv::Rect& new_region) {
	double k = inside_koef_;
	int new_area{ new_region.area() };
	int area{ image_rect_.area() };
	if (new_area >= area || (image_rect_ & new_region).area() < int(k * new_area)) {
		return true;
	}
	return false;
}

cv::Mat dt::Result::create(const cv::Mat& img) {
	if (regions_.empty()) {
		std::cerr << name_ + ": Nothing to visualize, there are no results.\n";
		return img;
	}
	if (img.empty()) {
		throw std::logic_error("Nothing to visualize, the image is empty.");
	}
	cv::Mat img_result;
	img.copyTo(img_result);
	std::cout << name_ << ": ";
	for (auto region : regions_) {
		std::cout << region << " ";
		cv::rectangle(img_result, region, cv::Scalar(0, 230, 0), 2, 8, 0);
		cv::Point position{ region.tl() - cv::Point(0, 3) };
		cv::putText(img_result, name_, position, cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 230, 0), 2, 8, 0);
	}
	std::cout << "\n";
	return img_result;
}

cv::Mat dt::create(const cv::Mat& img, const std::vector<dt::Result>& res) {
	if (res.empty()) {
		std::cerr << "There are no results yet. Detect it and try again.\n";
		return img;
	}
	if (img.empty()) {
		throw std::runtime_error("Nothing to visualize, the image is empty.");
	}
	cv::Mat img_result;
	img.copyTo(img_result);
	std::cout << "Found regions:\n";
	for (auto result : res) {
		try {
			img_result = result.create(img_result);
		}
		catch (const std::exception & er) {
			std::cerr << er.what() << std::endl;
			continue;
		}
	}
	return img_result;
}
