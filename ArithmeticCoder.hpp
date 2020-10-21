#pragma once

#include <algorithm>
#include <cstdint>
#include "BitIoStream.hpp"
#include "FrequencyTable.hpp"

class ArithmeticCoderBase {

protected: int numStateBits;

		   // Максимальный диапазон (high+1-low) во время кодирования, явл 2^numStateBits = 1000...000.
protected: std::uint64_t fullRange;
		   // Верхний бит на ширине numStateBits, который составляет 0100 ... 000.
protected: std::uint64_t halfRange;
		   // он второй по величине бит при ширине numStateBits, которая составляет 0010 ... 000. 
		   //Это ноль, когда numStateBits = 1.
protected: std::uint64_t quarterRange;
		   //Миним диапазон (high+1-low) во время кодирования, явл. 0010...010.
protected: std::uint64_t minimumRange;
		   //Максимально допустимая сумма из таблицы частот за все время во время кодирования.
protected: std::uint64_t maximumTotal;
		   // Битовая маска из numStateBits единиц, которая составляет 0111 ... 111.
protected: std::uint64_t stateMask;

		   // Нижний предел текущего диапазона этого арифметического кодера.
protected: std::uint64_t low;
		   //Верхний предел текущего диапазона этого арифметического кодера.
protected: std::uint64_t high;


		   // Создает арифметический кодер, который инициализирует диапазон кода.
public: explicit ArithmeticCoderBase(int numBits);
public: virtual ~ArithmeticCoderBase() = 0;

		// Обновляет диапазон кода (низкий и высокий) этого арифметического кодера в результате
		// обработки данного символа с данной таблицей частот.
protected: virtual void update(const FrequencyTable& freqs, std::uint32_t symbol);

		   //Вызывается для обработки ситуации, когда верхний бит «низкий» и «высокий» равны.
protected: virtual void shift() = 0;

		   //Вызывается для обработки ситуации, когда низкий = 01 (...) и высокий = 10 (...).
protected: virtual void underflow() = 0;
};



class ArithmeticDecoder final : private ArithmeticCoderBase {
private: BitInputStream& input;
		 // Буферизуется текущ бит исходного кода, кот. всегда нах в диапазоне [низкий, высокий].
private: std::uint64_t code;

public: explicit ArithmeticDecoder(int numBits, BitInputStream& in);
		// Декодирует следующий символ на основе заданной таблицы частот и возвращает его.
		// Также обновляет состояние этого арифм кодера и может считывать некоторые биты.
public: std::uint32_t read(const FrequencyTable& freqs);

protected: void shift() override;

protected: void underflow() override;
		// Возвращает следующий бит (0 или 1) из входного потока. Конец
		// потока обрабатывается как бесконечное число конечных нулей.
private: int readCodeBit();

};



class ArithmeticEncoder final : private ArithmeticCoderBase {
private: BitOutputStream& output;
		 // Число сохраненных временных битов
private: unsigned long numUnderflow;

public: explicit ArithmeticEncoder(int numBits, BitOutputStream& out);

		// Кодирует данный символ на основе заданной таблицы частот.
		// Также обновляет состояние этого арифм кодера.
public: void write(const FrequencyTable& freqs, std::uint32_t symbol);


		// Завершает арифметическое кодирование, сбрасывая любые буферизованные биты.
		//Этот метод записывает данные в основной поток вывода, но не закрывает его.
public: void finish();
protected: void shift() override;
protected: void underflow() override;
};