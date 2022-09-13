#pragma once

#ifndef _HEADER_H_
#define _HEADER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <codecvt>
#include <filesystem>

class MainContainer	// ��� ��������
{
public:
	std::wstring				m_id;			// id ���������
	std::vector<std::wstring>	m_need_field;	// ����������� ����
	std::vector<int>			m_position;		// �������� ������� ����� � �����

public:
	explicit MainContainer(std::wstring id, std::vector<std::wstring>& need_field, std::vector<int>& position) : m_id(id),
		m_need_field(need_field), m_position(position)
	{}
};

struct ObjectName	// ��� ������������ ������� �����������
{
	std::wstring m_workshop;
	std::wstring m_object;
	std::wstring m_file_name;
	std::wstring m_file_path;

	ObjectName(const std::wstring& workshop, const std::wstring& object,
		const std::wstring& file_path, const std::wstring& file_name = L"") :
		m_workshop(workshop), m_object(object), m_file_name(file_name), m_file_path(file_path) {}
};

// ������� ��� ������ ������������ ����� ������ ��������
std::vector<std::wstring> arr_1000{ L"�������� ��",L"��� ����������",/*L"��� ����������",*/
							L"���������� ����",L"���������� �����������",
							L"����� ����������� ������",L"�������� ������",
							L"������ ����� ��������",L"�������� ����� ��������" };
std::vector<std::wstring> arr_2052{ L"�������������",L"�������",L"������",L"�������� �����" };
std::vector<std::wstring> arr_2053{ L"�������������",L"�������� �����" };
std::vector<std::wstring> arr_2055{ L"-999999" };	// ��������������
std::vector<std::wstring> arr_2056{ L"������",L"���",L"�������" };
std::vector<std::wstring> arr_2081{ L"ID ������",L"�������� �����",L"������",L"���������" };
std::vector<std::wstring> arr_2042{ L"�������",L"������� �����" };
std::vector<std::wstring> arr_2041{ L"ID ��������",L"������",L"����. ������� ������� ������",L"����. ����������" };
std::vector<std::wstring> arr_2201{ L"-999999",L"MAC �����",L"IP �����",L"����� �������",L"����" };
std::vector<std::wstring> arr_2350{ L"-999999" };	// ������ ������� "��� ������������", ��������� ������� "��������"(����� ����������� �� �����������)
std::vector<std::wstring> arr_2311{ L"-999999" };	// ������ ������� "��� ������������"

const int MAX_COL = 37;									// ���-�� �����
// ���������� � ������������������ ������� � ������ (����� ������ � ����� ������)
std::vector<int> id_1000{ 4,3,/*11,*/12,13,14,15,19,20 };	// ����� ����������
std::vector<int> id_2052{ 5,6,7,8 };					// �������
std::vector<int> id_2053{ 9,10 };						// ��������� �����
std::vector<int> id_2055{ 11 };							// ���������� ?????? (��������� ���-�� �����������)
std::vector<int> id_2056{ 16,17,18 };					// ���������� ������ (����������)
std::vector<int> id_2081{ 21,22,23,24 };				// ��� (������� �����, ���-�� ����� �������)
std::vector<int> id_2042{ 30,31 };						// ����� Windows (����������)
std::vector<int> id_2041{ 32,33,34,35 };				// ������� (����� ������ ������ "������", ����� ����� "<tr><td class='h'>" ?)
std::vector<int> id_2201{ 25,26,27,28,29 };				// ������� ��������
std::vector<int> id_2350{ 36 };							// ������������ (����� ����������� �� �����������)
std::vector<int> id_2311{ 37 };							// �������� ������ (����������� �������� ������)

std::vector<std::pair<std::wstring, std::pair<std::vector<std::wstring>, std::vector<int>>>> need_info{ std::pair<std::wstring,std::pair<std::vector<std::wstring>,std::vector<int>>>(L"1000",std::pair<std::vector<std::wstring>,std::vector<int>>(arr_1000,id_1000)),
																									std::pair<std::wstring,std::pair<std::vector<std::wstring>,std::vector<int>>>(L"2052",std::pair<std::vector<std::wstring>,std::vector<int>>(arr_2052,id_2052)),
																									std::pair<std::wstring,std::pair<std::vector<std::wstring>,std::vector<int>>>(L"2053",std::pair<std::vector<std::wstring>,std::vector<int>>(arr_2053,id_2053)),
																									std::pair<std::wstring,std::pair<std::vector<std::wstring>,std::vector<int>>>(L"2055",std::pair<std::vector<std::wstring>,std::vector<int>>(arr_2055,id_2055)),
																									std::pair<std::wstring,std::pair<std::vector<std::wstring>,std::vector<int>>>(L"2056",std::pair<std::vector<std::wstring>,std::vector<int>>(arr_2056,id_2056)),
																									std::pair<std::wstring,std::pair<std::vector<std::wstring>,std::vector<int>>>(L"2081",std::pair<std::vector<std::wstring>,std::vector<int>>(arr_2081,id_2081)),
																									std::pair<std::wstring,std::pair<std::vector<std::wstring>,std::vector<int>>>(L"2042",std::pair<std::vector<std::wstring>,std::vector<int>>(arr_2042,id_2042)),
																									std::pair<std::wstring,std::pair<std::vector<std::wstring>,std::vector<int>>>(L"2041",std::pair<std::vector<std::wstring>,std::vector<int>>(arr_2041,id_2041)),
																									std::pair<std::wstring,std::pair<std::vector<std::wstring>,std::vector<int>>>(L"2201",std::pair<std::vector<std::wstring>,std::vector<int>>(arr_2201,id_2201)),
																									std::pair<std::wstring,std::pair<std::vector<std::wstring>,std::vector<int>>>(L"2350",std::pair<std::vector<std::wstring>,std::vector<int>>(arr_2350,id_2350)),
																									std::pair<std::wstring,std::pair<std::vector<std::wstring>,std::vector<int>>>(L"2311",std::pair<std::vector<std::wstring>,std::vector<int>>(arr_2311,id_2311))
};


#endif // !_HEADER_H
