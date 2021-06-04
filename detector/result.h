/*!  \file
	 \author Евгений Оржех
	 \date 2020 год

	 Данный заголовочный файл содержит в себе определение класса результата детектирования
	 и некоторые методы, общие для данной библиотеки.
*/


#pragma once
#ifndef DT_RESULT_RESULT_H
#define DT_RESULT_RESULT_H

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>

/// пространство имен Detector Lib
namespace dt {
	/*!  \brief Класс, реализущий хранение результата детектирования
		 
		 Объект данного класса Result предоставляет доступ к областям,
		 где были детектированы элементы одного типа, их названию и размеру исходной картинки.
		 А также методы визуализации результата. 
	*/
	class Result {
	public:
		Result() = default;
		Result(const std::string& name, const cv::Rect& image_rect); ///< Конструктор 
		Result(const Result&) = default;
		Result& operator=(const Result& rhs) = default;
		~Result() = default;

		std::string name() const { return name_; } ///< Cвойство доступа к названию найденного элемента
		void name(const std::string& name) { name_ = name; } ///< Cвойство добавления названия элемента

		cv::Rect image_rect() const { return image_rect_; } ///< Свойство доступа к размеру исходного изображения
		void image_rect(const cv::Rect& rect); ///< Свойство добавления размера исходного изображения

		bool empty() const { return regions_.empty(); } ///< Проверка на наличие детектированных элементов
		size_t size() const { return regions_.size(); } ///< Количество детектированных элементов

		cv::Rect get_region(size_t index) const; ///< Cвойство доступа к областям по индексу
		void add_region(const cv::Rect& region); ///< Cвойство добавления новой области

		void intersec_koef(double k); ///< Коэффициент допустимого пересечения детектируемых областей
		double intersec_koef() const { return intersec_koef_; } ///< Свойство получения коэф. пересечения
		void inside_koef(double k); ///< Коэффициент выхода за границы картинки для детектируемых областей
		double inside_koef() const { return inside_koef_; } ///< Свойство получения коэф. выхода за границы

		cv::Mat create(const cv::Mat& img); ///< Изображение детектированных областей с метками классов на картинке
		void set_regions(const std::vector<cv::Rect>& regions); ///< Добавление найденных областей
	private:
		double intersec_koef_{ 0.2 };
		double inside_koef_{ 0.8 };
		bool intersection(const cv::Rect& new_region);
		bool out_of_image(const cv::Rect& new_region);
	private:
		std::string name_;
		cv::Rect image_rect_;
		std::vector<cv::Rect> regions_;
	};

	/// сервисная функция получения изображения по его системному пути
	void get_Mat(std::string& image_path, cv::Mat& output); 
    
	/// функция, позволяющая совместить результаты детектирования нескольких видов элементов на одном изображении
	cv::Mat create(const cv::Mat& img, const std::vector<dt::Result>& res); 

	/// функция показа итогового результата на экране
    void show(const cv::Mat& image, std::string text, cv::WindowFlags size);
};
#endif DT_RESULT_RESULT_H