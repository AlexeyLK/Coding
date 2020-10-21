#pragma once

#include <algorithm>
#include <cstdint>
#include "BitIoStream.hpp"
#include "FrequencyTable.hpp"

class ArithmeticCoderBase {

protected: int numStateBits;

		   // ������������ �������� (high+1-low) �� ����� �����������, ��� 2^numStateBits = 1000...000.
protected: std::uint64_t fullRange;
		   // ������� ��� �� ������ numStateBits, ������� ���������� 0100 ... 000.
protected: std::uint64_t halfRange;
		   // �� ������ �� �������� ��� ��� ������ numStateBits, ������� ���������� 0010 ... 000. 
		   //��� ����, ����� numStateBits = 1.
protected: std::uint64_t quarterRange;
		   //����� �������� (high+1-low) �� ����� �����������, ���. 0010...010.
protected: std::uint64_t minimumRange;
		   //����������� ���������� ����� �� ������� ������ �� ��� ����� �� ����� �����������.
protected: std::uint64_t maximumTotal;
		   // ������� ����� �� numStateBits ������, ������� ���������� 0111 ... 111.
protected: std::uint64_t stateMask;

		   // ������ ������ �������� ��������� ����� ��������������� ������.
protected: std::uint64_t low;
		   //������� ������ �������� ��������� ����� ��������������� ������.
protected: std::uint64_t high;


		   // ������� �������������� �����, ������� �������������� �������� ����.
public: explicit ArithmeticCoderBase(int numBits);
public: virtual ~ArithmeticCoderBase() = 0;

		// ��������� �������� ���� (������ � �������) ����� ��������������� ������ � ����������
		// ��������� ������� ������� � ������ �������� ������.
protected: virtual void update(const FrequencyTable& freqs, std::uint32_t symbol);

		   //���������� ��� ��������� ��������, ����� ������� ��� ������� � �������� �����.
protected: virtual void shift() = 0;

		   //���������� ��� ��������� ��������, ����� ������ = 01 (...) � ������� = 10 (...).
protected: virtual void underflow() = 0;
};



class ArithmeticDecoder final : private ArithmeticCoderBase {
private: BitInputStream& input;
		 // ������������ ����� ��� ��������� ����, ���. ������ ��� � ��������� [������, �������].
private: std::uint64_t code;

public: explicit ArithmeticDecoder(int numBits, BitInputStream& in);
		// ���������� ��������� ������ �� ������ �������� ������� ������ � ���������� ���.
		// ����� ��������� ��������� ����� ����� ������ � ����� ��������� ��������� ����.
public: std::uint32_t read(const FrequencyTable& freqs);

protected: void shift() override;

protected: void underflow() override;
		// ���������� ��������� ��� (0 ��� 1) �� �������� ������. �����
		// ������ �������������� ��� ����������� ����� �������� �����.
private: int readCodeBit();

};



class ArithmeticEncoder final : private ArithmeticCoderBase {
private: BitOutputStream& output;
		 // ����� ����������� ��������� �����
private: unsigned long numUnderflow;

public: explicit ArithmeticEncoder(int numBits, BitOutputStream& out);

		// �������� ������ ������ �� ������ �������� ������� ������.
		// ����� ��������� ��������� ����� ����� ������.
public: void write(const FrequencyTable& freqs, std::uint32_t symbol);


		// ��������� �������������� �����������, ��������� ����� �������������� ����.
		//���� ����� ���������� ������ � �������� ����� ������, �� �� ��������� ���.
public: void finish();
protected: void shift() override;
protected: void underflow() override;
};