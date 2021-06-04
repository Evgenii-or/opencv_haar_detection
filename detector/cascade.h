/*!
	\file
	\author Евгений Оржех
	\date 2020 год

	Данный заголовочный файл содержит в себе определение классов и функций
	для реализации детектирования методом каскадных классификаторов. (Cascade Classifiers)
*/

#pragma once
#ifndef DT_CASCADE_CASCADE_H
#define DT_CASCADE_CASCADE_H

#include "result.h"

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>

/// пространство имен Detector Lib
namespace dt {
	/*! Структура, упрощающая работу с именованными классификаторами */
	struct NamedCascade { 
		std::string name_; ///< Название элемента
		cv::CascadeClassifier cascade_; ///< Объект каскада
	public:
		NamedCascade(const std::string& cascade_path, const std::string& name); ///< Конструктор, загружающий каскад и его название
		NamedCascade() = default;
		~NamedCascade() = default;
		NamedCascade(const NamedCascade&) = default;
		NamedCascade& operator=(const NamedCascade & rhs) = default;

		void match(const cv::Mat& img, dt::Result& output); ///< Метод, реализующий детектирование с помощью обученного каскадного классификатора
	};

	/*! Реализует парсинг конфигурационного файла обученных каскадов
		\param main_dir Директория, где находится конфигурационный файл
		\param config_name Имя конфигурационного файла, в котором указаны имена каскадов и системные пути к ним
		\param output Выходной вектор объектов именованных каскадов
	*/
	void config(const std::string& main_dir, const std::string& config_name, std::vector<dt::NamedCascade>& output);

	/*! Реализует детектирование с помощью сразу нескольких обученных каскадных классификаторов
		\param img Исходное изображение, на котором нужно детектировать элементы
		\param cass Вектор объектов обученных именованных каскадов 
		\param output Выходной вектор результатов для каждого каскада
	*/
	void match(const cv::Mat& img, std::vector<dt::NamedCascade>& cass, std::vector<dt::Result>& output);
};
#endif DT_CASCADE_CASCADE_H
