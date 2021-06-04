// Copyright 2020 by Orzhekh Evgenii

#include "cascade.h"
#include "result.h"
#include "template.h"

#include <stdexcept>

static void help(const char** argv)
{
	std::cout << "\nThis program demonstrates the use of library" 
		"for detecting graphical user interface elements.\n"
		"Using this library, the user can detect the buttons"
		"of the internal interface of the Instagram app"
		"for mobile devices based on the input screenshot.\n"
		"The library provides two detection methods:\n"
		"Template Matching and Cascade Classifiers\n"
		"Usage:\n"
		<< argv[0]
		<< "   [--image=<image_path> This is the image where you want to detect buttons]\n" 
		   "   [--tm=<template_matching>]\n"
		   "   [--cc=<cascade_classifiers>]\n You can use both methods.\n"
		   "example:\n"
		<< argv[0]
		<< " --image=\"data/test_image.png\" --tm --cc\n"
		<< std::endl;
}

void use_templates(const cv::Mat& image) {
	std::vector<dt::Template> templates;
	std::vector<dt::Result> temp_results;
	cv::Mat img_result;

	dt::config("../prj.cwork/data/", "templates.txt", templates);
	dt::match(image, templates, temp_results);
	img_result = dt::create(image, temp_results);

	dt::show(img_result, "Templates detector result", cv::WINDOW_NORMAL);
}

void use_cascades(cv::Mat& image) {
	std::vector<dt::NamedCascade> cascades;
	std::vector<dt::Result> cas_results;
	cv::Mat img_result;

	dt::config("../prj.cwork/data/", "cascades.txt", cascades);
	dt::match(image, cascades, cas_results);
	img_result = dt::create(image, cas_results);

	dt::show(img_result, "Cascade Classifier result", cv::WINDOW_NORMAL);
}

int main(int argc, const char** argv) {
	cv::Mat img;
	bool tm, cc;
	std::string img_path;

	cv::CommandLineParser parser(argc, argv,
		"{help h||}"
		"{image||}" 
		"{tm||}"
		"{cc||}"
	);
	if (parser.has("help")) {
		help(argv);
		return 0;
	}
	img_path = parser.get<std::string>("image");
	tm = parser.has("tm");
	cc = parser.has("cc");
	if (!parser.check()) {
		parser.printErrors();
		return 1;
	}
	try {
		dt::get_Mat(img_path, img);
		if (cc) {
			use_cascades(img);
		}
		std::cout << "\n";
		if (tm) {
			use_templates(img);
		}
		cv::waitKey(0);
		return 0;
	}
	catch (const std::exception & er) {
		std::cerr << "Error: " << er.what() << std::endl;
		help(argv);
		return 1;
	}
}
