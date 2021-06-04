/*!
	\file
	\author Евгений Оржех
	\date 2020 год

	Данный заголовочный файл содержит в себе определение классов и функций 
	для реализации детектирования методом сопоставления шаблонов. (Template Mathing)
*/

#pragma once
#ifndef DT_TEMPLATE_TEMPLATE_H
#define DT_TEMPLATE_TEMPLATE_H

#include "result.h"

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>

/// пространство имен Detector Lib
namespace dt {
	/*! Класс, упрощающий работу с именованными шаблонами */
	class Template {
	public:
		Template() = default; 
		Template(const Template&) = default;
		~Template() = default;
		Template& operator=(const Template&) = default;

		Template(const cv::Mat& src, const std::string& name); ///< конструктор именованного шаблона
		Template(const std::string& src_path, const std::string& name); ///< конструктор именованного шаблона

	    cv::Mat image() const; ///< Доступ к изображению шаблона
		void image(const cv::Mat img); ///< Добавление изображения шаблона
		std::string name() const { return name_; } ///< Доступ к имени шаблона
		void name(const std::string name) { name_ = name; } ///< Добавление имени шаблона
	private:
		cv::Mat image_;
		std::string name_;
	};

    /*! Реализует парсинг конфигурационного файла шаблонов
	    \param main_dir Директория, где находится конфигурационный файл
		\param config_name Имя конфигурационного файла, в котором указаны имена шаблонов и системные пути к ним
		\param output Выходной вектор объектов шаблонов
	*/
	void config(const std::string& main_dir, const std::string& config_name, std::vector<dt::Template>& output);

	/*! Реализует детектирование методом сопоставления шаблонов для одного элемента
	    \param img Исходное изображение, на котором нужно детектировать элементы
		\param temp Объект шаблона, который нужно детектировать
		\param output Выходной объект результата детектирования
		\param limit Лимит количества возможных шаблонов на изображении, если найдено больше - значит детектор ошибся и это выбросы.
		\param threshold Порог выраженности шаблона, рекомендуется 0.97-0.99
	*/
	void match(const cv::Mat& img, const dt::Template& temp, dt::Result& output, int limit = 100, double threshold = 0.98);

	/// Реализует детектирование методом сопоставления шаблонов для нескольких элементов сразу
	void match(const cv::Mat& img, const std::vector<dt::Template>& input, std::vector<dt::Result>& output, int limit = 100, double threshold = 0.98);
};

#endif DT_TEMPLATE_TEMPLATE_H