#pragma once

#include <cstdint>
#include <vector>


class FrequencyTable {
public: virtual ~FrequencyTable() = 0;
		//Возвращает количество символов в этой таблице частот, которое является положительным числом.
public: virtual std::uint32_t getSymbolLimit() const = 0;
		//Возвращает частоту данного символа.
public: virtual std::uint32_t get(std::uint32_t symbol) const = 0;
		// Устанавливает частоту данного символа для данного значения.
public: virtual void set(std::uint32_t symbol, std::uint32_t freq) = 0;
		// Увеличивает частоту данного символа.
public: virtual void increment(std::uint32_t symbol) = 0;
		// Возвращает сумму всех частот символов. 
		//Возвращаемое значение всегда равно getHigh (getSymbolLimit () - 1).
public: virtual std::uint32_t getTotal() const = 0;
		//Возвращает сумму частот всех символов строго ниже заданного значения символа.
public: virtual std::uint32_t getLow(std::uint32_t symbol) const = 0;
		//Возвращает сумму частот данного символа и всех символов ниже.
public: virtual std::uint32_t getHigh(std::uint32_t symbol) const = 0;
};



class FlatFrequencyTable final : public FrequencyTable {
private: std::uint32_t numSymbols;	//Общее количество символов, которое не менее 1.

public: explicit FlatFrequencyTable(std::uint32_t numSyms);

public: std::uint32_t getSymbolLimit() const override;
public: std::uint32_t get(std::uint32_t symbol) const override;
public: std::uint32_t getTotal() const override;
public: std::uint32_t getLow(std::uint32_t symbol) const override;
public: std::uint32_t getHigh(std::uint32_t symbol) const override;
public: void set(std::uint32_t symbol, std::uint32_t freq) override;
public: void increment(std::uint32_t symbol) override;
private: void checkSymbol(std::uint32_t symbol) const;
};



class SimpleFrequencyTable final : public FrequencyTable {

		//Частота для каждого символа. Его длина не менее 1.
private: std::vector<std::uint32_t> frequencies;

		 //cumulative[i] - это сумма «частот» от 0 (включительно) до i (исключая).
private: mutable std::vector<std::uint32_t> cumulative;

private: std::uint32_t total;	//Сумма «частот».



public: explicit SimpleFrequencyTable(const std::vector<std::uint32_t>& freqs);
		// Создает таблицу частот, копируя данную таблицу частот.
public: explicit SimpleFrequencyTable(const FrequencyTable& freqs);

public: std::uint32_t getSymbolLimit() const override;
public: std::uint32_t get(std::uint32_t symbol) const override;
public: void set(std::uint32_t symbol, std::uint32_t freq) override;
public: void increment(std::uint32_t symbol) override;
public: std::uint32_t getTotal() const override;
public: std::uint32_t getLow(std::uint32_t symbol) const override;
public: std::uint32_t getHigh(std::uint32_t symbol) const override;

		// Пересчитывает массив кумулятивных частот символов.
private: void initCumulative(bool checkTotal = true) const;
		// Добавляет заданные целые числа
private: static std::uint32_t checkedAdd(std::uint32_t x, std::uint32_t y);
};