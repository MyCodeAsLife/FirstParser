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

class MainContainer	// Для парсинга
{
public:
	std::wstring				m_id;			// id заголовка
	std::vector<std::wstring>	m_need_field;	// необходимые поля
	std::vector<int>			m_position;		// итоговые позиции полей в файле

public:
	explicit MainContainer(std::wstring id, std::vector<std::wstring>& need_field, std::vector<int>& position) : m_id(id),
		m_need_field(need_field), m_position(position)
	{}
};

struct ObjectName	// Для формирования массива дирректорий
{
	std::wstring m_workshop;
	std::wstring m_object;
	std::wstring m_file_name;
	std::wstring m_file_path;

	ObjectName(const std::wstring& workshop, const std::wstring& object,
		const std::wstring& file_path, const std::wstring& file_name = L"") :
		m_workshop(workshop), m_object(object), m_file_name(file_name), m_file_path(file_path) {}
};

// Массивы для поиска определенных полей внутри разделов
std::vector<std::wstring> arr_1000{ L"Название ОС",L"Имя компьютера",/*L"Имя процессора",*/
							L"Физических ядер",L"Логических процессоров",
							L"Всего оперативной памяти",L"Загрузка памяти",
							L"Размер файла подкачки",L"Загрузка файла подкачки" };
std::vector<std::wstring> arr_2052{ L"Производитель",L"Продукт",L"Версия",L"Серийный номер" };
std::vector<std::wstring> arr_2053{ L"Производитель",L"Серийный номер" };
std::vector<std::wstring> arr_2055{ L"-999999" };	// неиспользуется
std::vector<std::wstring> arr_2056{ L"Размер",L"Тип",L"Частота" };
std::vector<std::wstring> arr_2081{ L"ID модели",L"Серийный номер",L"Версия",L"Интерфейс" };
std::vector<std::wstring> arr_2042{ L"Адаптер",L"Текущий режим" };
std::vector<std::wstring> arr_2041{ L"ID монитора",L"Модель",L"Макс. видимая область экрана",L"Макс. разрешение" };
std::vector<std::wstring> arr_2201{ L"-999999",L"MAC адрес",L"IP адрес",L"Маска подсети",L"Шлюз" };
std::vector<std::wstring> arr_2350{ L"-999999" };	// Первая колонка "Имя пользователя", четвертая колонка "Отключен"(нужно перечислить не отключенных)
std::vector<std::wstring> arr_2311{ L"-999999" };	// Первая колонка "Имя пользователя"

const int MAX_COL = 37;									// Кол-во полей
// Информация о последовательности записей в строке (какая запись в какой ячейке)
std::vector<int> id_1000{ 4,3,/*11,*/12,13,14,15,19,20 };	// Общая информация
std::vector<int> id_2052{ 5,6,7,8 };					// Система
std::vector<int> id_2053{ 9,10 };						// Системная плата
std::vector<int> id_2055{ 11 };							// Процессоры ?????? (Вычислить кол-во процессоров)
std::vector<int> id_2056{ 16,17,18 };					// Устройства памяти (оперативка)
std::vector<int> id_2081{ 21,22,23,24 };				// АТА (жесткие диски, кол-во через запятую)
std::vector<int> id_2042{ 30,31 };						// Видео Windows (видеокарты)
std::vector<int> id_2041{ 32,33,34,35 };				// Монитор (может вместо строки "Модель", взять после "<tr><td class='h'>" ?)
std::vector<int> id_2201{ 25,26,27,28,29 };				// Сетевые адаптеры
std::vector<int> id_2350{ 36 };							// Пользователи (нужно перечислить не отключенных)
std::vector<int> id_2311{ 37 };							// Активные сессии (перечислить активные сессии)

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
