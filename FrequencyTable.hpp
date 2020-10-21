#pragma once

#include <cstdint>
#include <vector>


class FrequencyTable {
public: virtual ~FrequencyTable() = 0;
		//���������� ���������� �������� � ���� ������� ������, ������� �������� ������������� ������.
public: virtual std::uint32_t getSymbolLimit() const = 0;
		//���������� ������� ������� �������.
public: virtual std::uint32_t get(std::uint32_t symbol) const = 0;
		// ������������� ������� ������� ������� ��� ������� ��������.
public: virtual void set(std::uint32_t symbol, std::uint32_t freq) = 0;
		// ����������� ������� ������� �������.
public: virtual void increment(std::uint32_t symbol) = 0;
		// ���������� ����� ���� ������ ��������. 
		//������������ �������� ������ ����� getHigh (getSymbolLimit () - 1).
public: virtual std::uint32_t getTotal() const = 0;
		//���������� ����� ������ ���� �������� ������ ���� ��������� �������� �������.
public: virtual std::uint32_t getLow(std::uint32_t symbol) const = 0;
		//���������� ����� ������ ������� ������� � ���� �������� ����.
public: virtual std::uint32_t getHigh(std::uint32_t symbol) const = 0;
};



class FlatFrequencyTable final : public FrequencyTable {
private: std::uint32_t numSymbols;	//����� ���������� ��������, ������� �� ����� 1.

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

		//������� ��� ������� �������. ��� ����� �� ����� 1.
private: std::vector<std::uint32_t> frequencies;

		 //cumulative[i] - ��� ����� ������� �� 0 (������������) �� i (��������).
private: mutable std::vector<std::uint32_t> cumulative;

private: std::uint32_t total;	//����� �������.



public: explicit SimpleFrequencyTable(const std::vector<std::uint32_t>& freqs);
		// ������� ������� ������, ������� ������ ������� ������.
public: explicit SimpleFrequencyTable(const FrequencyTable& freqs);

public: std::uint32_t getSymbolLimit() const override;
public: std::uint32_t get(std::uint32_t symbol) const override;
public: void set(std::uint32_t symbol, std::uint32_t freq) override;
public: void increment(std::uint32_t symbol) override;
public: std::uint32_t getTotal() const override;
public: std::uint32_t getLow(std::uint32_t symbol) const override;
public: std::uint32_t getHigh(std::uint32_t symbol) const override;

		// ������������� ������ ������������ ������ ��������.
private: void initCumulative(bool checkTotal = true) const;
		// ��������� �������� ����� �����
private: static std::uint32_t checkedAdd(std::uint32_t x, std::uint32_t y);
};