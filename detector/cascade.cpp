// Copyright 2020 by Orzhekh Evgenii

#include "cascade.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

dt::NamedCascade::NamedCascade(const std::string& filename, const std::string& name)
	: name_(name) {
	if (!cascade_.load(filename)) {
		throw std::runtime_error("Сan't load cascade from file.");
	}
}

void dt::NamedCascade::match(const cv::Mat& img, dt::Result& output) {
	if (!cascade_.empty()) {
		cv::Mat img_gray;
		const int limit{ 100 };
		std::vector<cv::Rect> regions;

		cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
		cv::equalizeHist(img_gray, img_gray);
		cascade_.detectMultiScale(img_gray, regions);
		if (regions.empty()) {
			std::cerr << "Haven't found this template on the image.\n";
			return;
		}
		if (regions.size() > limit) {
			regions.erase(regions.begin(), regions.end());
			std::cerr << "Haven't found this template on the image.\n";
			return;
		}

		output.image_rect(cv::Rect(0, 0, img.cols, img.rows));
		output.name(name_);
		output.set_regions(regions);
	}
	else {
		throw std::logic_error("The cascade is empty, load it and try again.");
	}
}

void dt::match(const cv::Mat& img, std::vector<dt::NamedCascade>& cass, std::vector<dt::Result>& output) {
	if (cass.empty()) {
		throw std::logic_error("There are no cascades in input vector.");
	}
	for (auto cascade : cass) {
		dt::Result r;
		cascade.match(img, r);
		output.push_back(r);
	}
}

void dt::config(const std::string& main_dir, const std::string& config_name, std::vector<dt::NamedCascade>& output) {
	std::ifstream fin(main_dir + config_name);
	if (fin.is_open()) {
		std::string buff, path, name;
		while (0 == getline(fin, buff).fail()) {
			std::istringstream sin(buff);
			sin >> name >> path;
			if (sin) {
				try {
					NamedCascade cas(main_dir + path, name);
					output.push_back(cas);
				}
				catch (const std::exception & er) {
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