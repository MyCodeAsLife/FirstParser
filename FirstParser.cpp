#include "FirstParser.h"

namespace fs = std::filesystem;

static int num_field = 0;

int MyGetLine(std::wifstream& readfile, std::wstring& read_to_wstr)
{
	std::getline(readfile, read_to_wstr);
	++num_field;
	return num_field;
}

void Copy_WStr(std::wstring::iterator first, std::wstring::iterator last, std::wstring& dst)
{
	auto end = last;
	++end;
	while (first != end)
	{
		dst.push_back(*first);
		++first;
	}
}

void Search_And_Copy_WStr(const int pos_start, const int pos_end, std::wstring& source, std::wstring& dest)
{
	dest.erase();
	auto ptr_id_1 = source.begin();
	ptr_id_1 += pos_start;
	auto ptr_id_2 = source.begin();
	ptr_id_2 += pos_end;
	Copy_WStr(ptr_id_1, ptr_id_2, dest);
}
// Объеденить все 3 функции Get в одну
void Get_WText_Chain(std::wstring& source, std::wstring& dest)
{
	int pos_start_col = source.find(L"<tr><td>");
	int pos_start_text = source.find(L"</td><td>", pos_start_col);
	//int pos_end_text = source.find("</td></tr>");
	if (pos_start_text != (-1))
	{
		Search_And_Copy_WStr(pos_start_col + 8, pos_start_text - 1, source, dest);
	}
}

void Get_WText_Col(std::wstring& source, std::wstring& dest)
{
	//int pos_start_col = source.find(L"<tr><td>");
	int pos_start_text = source.find(L"</td><td>");
	int pos_end_text = source.find(L"</td></tr>", pos_start_text);
	if (pos_start_text != (-1))
	{
		Search_And_Copy_WStr(pos_start_text + 9, pos_end_text - 1, source, dest);
	}
}

void Get_WText_Head(std::wstring& source, std::wstring& dest)
{
	int pos_start_text = source.find(L"<tr><td class='h'>");
	int pos_end_text = source.find(L"</td><td>",pos_start_text);
	if (pos_start_text != (-1))
	{
		Search_And_Copy_WStr(pos_start_text + 18, pos_end_text - 1, source, dest);
	}
}

void Get_WText_T1(std::wstring& source, std::wstring& dest)
{
	int count = 0;
	int pos_start = source.find(L"</td><td>");
	while (true)
	{
		if (count > 1)
			break;
		pos_start = source.find(L"</td><td>", (pos_start + 9));		// 9 - длина строки "</td><td>"
		count++;
	}
	int pos_end = source.find(L"</td><td>", (pos_start + 9));
	if (pos_start != (-1) || pos_end != (-1))
	{
		Search_And_Copy_WStr(pos_start + 9, pos_end - 1, source, dest);
	}
}

//// Формирование конечного массива( из которого будет производится запись в файл
void WriteInFinalyArray(std::map<int, std::wstring>& temp_array, std::vector<std::pair<int, std::wstring>>& array_string,
	std::map<int, std::wstring>& final_array_text, std::vector<ObjectName>& object_file_name_dir)
{
	for (int i = 0; i < array_string.size(); ++i)
	{
		temp_array.insert(std::make_pair(array_string[i].first, L""));
	}
	for (int i = 0; i < array_string.size(); ++i)
	{
		for (auto ptr3 = temp_array.begin(); ptr3 != temp_array.end(); ++ptr3)
		{
			if (ptr3->first == array_string[i].first)
			{
				ptr3->second += array_string[i].second;
				ptr3->second += L", ";
			}
		}
	}
	///// Конец формирования информации для конечной записи
	for (auto ptr4 = temp_array.begin(); ptr4 != temp_array.end(); ++ptr4)	// Конечная запись сформированной информации
	{
		final_array_text.insert(std::make_pair(ptr4->first, ptr4->second));
	}
}

// Если раздел "Устройства памяти" - оперативка
void Find_For_2056(std::wifstream& rfile, std::wstring& read_wstr, std::map<int, std::wstring>& final_array_text, std::vector<MainContainer>::iterator& ptr, std::vector<ObjectName>& object_file_name_dir)
{
		std::vector<std::pair<int, std::wstring>> array_string;
		std::map<int, std::wstring> temp_array;
		std::wstring write_wstr;
		while (true)
		{
			int num_field = MyGetLine(rfile, read_wstr);
			if (read_wstr.find(L"</table>") != (-1))	// Если тело раздела кончилось, прерываем цикл
			{
				break;
			}
			if (read_wstr.find(L"Размер") != (-1))
			{
				while (true)
				{
					if (read_wstr.find(L"<tr><td class='h'>") != (-1))	// Если начинается новый подраздел
					{
						break;											// Выходим из цикла
					}
					int num_pos = 0;		// Для формирования временного массива с повторяющимися полями
					// Перебираем массив наименований полей привязанный к определенному id
					for (auto ptr1 = ptr->m_need_field.begin(); ptr1 != ptr->m_need_field.end(); ++ptr1)
					{
						if (read_wstr.find(ptr1->c_str()) != (-1))	// Ищем имя поля из массива в текущей строке
						{
							Get_WText_Col(read_wstr, write_wstr);	// Если нашли то вынимаем вторую часть поля("Значение")
							// и записываем ее во временный массив массив с привязкой к позиции(для повторяющихся полей, например если несколько планок ОЗУ)
							array_string.push_back(std::make_pair(ptr->m_position[num_pos], write_wstr));
						}
						++num_pos;
					}
					MyGetLine(rfile, read_wstr);
				}
			}
		}
		WriteInFinalyArray(temp_array, array_string, final_array_text, object_file_name_dir);
}

void Find_For_Test(std::wifstream& rfile, std::wstring& read_wstr, std::map<int, std::wstring>& final_array_text, std::vector<MainContainer>::iterator& ptr, std::vector<ObjectName>& object_file_name_dir)
{
	std::vector<std::pair<int, std::wstring>> array_string;
	std::map<int, std::wstring> temp_array;
	std::wstring write_wstr;
	while (true)
	{

		if (read_wstr.find(L"</table>") != (-1))	// Если тело раздела кончилось, прерываем цикл
		{
			break;
		}
		int num_field = MyGetLine(rfile, read_wstr);
		if (read_wstr.find(L"<tr><td class='h'>") != (-1))
		{
			while (true)
			{
				if ((read_wstr.find(L"</table>") != (-1)))	// Если тело раздела кончилось, прерываем цикл
				{
					break;
				}
				int num_pos = 0;		// Для формирования временного массива с повторяющимися полями
				// Перебираем массив наименований полей привязанный к определенному id
				for (auto ptr1 = ptr->m_need_field.begin(); ptr1 != ptr->m_need_field.end(); ++ptr1)
				{
					if (read_wstr.find(ptr1->c_str()) != (-1))	// Ищем имя поля из массива в текущей строке
					{
						Get_WText_Col(read_wstr, write_wstr);	// Если нашли то вынимаем вторую часть поля("Значение")
						// и записываем ее во временный массив массив с привязкой к позиции(для повторяющихся полей, например если несколько планок ОЗУ)
						array_string.push_back(std::make_pair(ptr->m_position[num_pos], write_wstr));
					}
					++num_pos;
				}
				MyGetLine(rfile, read_wstr);
			}
		}
	}
	WriteInFinalyArray(temp_array, array_string, final_array_text, object_file_name_dir);
}

void Find_For_2055(std::wifstream& rfile, std::wstring& read_wstr, std::map<int, std::wstring>& final_array_text, std::vector<MainContainer>::iterator& ptr, std::vector<ObjectName>& object_file_name_dir)
{
	std::vector<std::pair<int, std::wstring>> array_string;
	std::map<int, std::wstring> temp_array;
	std::wstring write_wstr;
	while (true)
	{
		if (read_wstr.find(L"</table>") != (-1))	// Если тело раздела кончилось, прерываем цикл
		{
			break;
		}
		MyGetLine(rfile, read_wstr);
		if (read_wstr.find(L"<tr><td class='h'>") != (-1))
		{
			Get_WText_Head(read_wstr, write_wstr);
			array_string.push_back(std::make_pair(ptr->m_position[0], write_wstr));	// 0 потому что в массиве всего 1 элемент
		}
	}
	WriteInFinalyArray(temp_array, array_string, final_array_text, object_file_name_dir);
}

void Find_For_2201(std::wifstream& rfile, std::wstring& read_wstr, std::map<int, std::wstring>& final_array_text, std::vector<MainContainer>::iterator& ptr, std::vector<ObjectName>& object_file_name_dir)
{
	std::vector<std::pair<int, std::wstring>> array_string;
	std::map<int, std::wstring> temp_array;
	std::wstring write_wstr;
	while (true)
	{

		if (read_wstr.find(L"</table>") != (-1))	// Если тело раздела кончилось, прерываем цикл
		{
			break;
		}
		int num_field = MyGetLine(rfile, read_wstr);
		if (read_wstr.find(L"<tr><td class='h'>") != (-1))	// Если начало подраздела
		{
			if (read_wstr.find(L"<tr><td class='h'>&nbsp") == (-1))	// Если это точно начало подраздела(а не его конец)
			{
				Get_WText_Head(read_wstr, write_wstr);		// Вынимаем Имя подраздела
				array_string.push_back(std::make_pair(ptr->m_position[0], write_wstr));	 // 0 потому что наименование всегда первое
			}
			while (true)
			{
				if ((read_wstr.find(L"</table>") != (-1)))	// Если тело раздела кончилось, прерываем цикл
				{
					break;
				}
				int num_pos = 0;		// Для формирования временного массива с повторяющимися полями
				// Перебираем массив наименований полей привязанный к определенному id
				for (auto ptr1 = ptr->m_need_field.begin(); ptr1 != ptr->m_need_field.end(); ++ptr1)
				{
					if (read_wstr.find(ptr1->c_str()) != (-1))	// Ищем имя поля из массива в текущей строке
					{
						Get_WText_Col(read_wstr, write_wstr);	// Если нашли то вынимаем вторую часть поля("Значение")
						// и записываем ее во временный массив массив с привязкой к позиции(для повторяющихся полей, например если несколько планок ОЗУ)
						array_string.push_back(std::make_pair(ptr->m_position[num_pos], write_wstr));
					}
					++num_pos;
				}
				MyGetLine(rfile, read_wstr);
			}
		}
	}
	WriteInFinalyArray(temp_array, array_string, final_array_text, object_file_name_dir);
}

void Find_For_2350(std::wifstream& rfile, std::wstring& read_wstr, std::map<int, std::wstring>& final_array_text, std::vector<MainContainer>::iterator& ptr, std::vector<ObjectName>& object_file_name_dir)
{
	std::vector<std::pair<int, std::wstring>> array_string;
	std::map<int, std::wstring> temp_array;
	std::wstring write_wstr;
	while (true)
	{
		if (read_wstr.find(L"</table>") != (-1))	// Если тело раздела кончилось, прерываем цикл
		{
			break;
		}
		int num_field = MyGetLine(rfile, read_wstr);
		if (read_wstr.find(L"<tr><td class='c'>") != (-1))
		{
			while (true)
			{
				MyGetLine(rfile, read_wstr);
				if (read_wstr.find(L"</table>") != (-1))	// Если тело раздела кончилось, прерываем цикл
				{
					break;
				}
				Get_WText_T1(read_wstr, write_wstr);		// Вынимаем значение 4го столбца ("Отключен")
				if (write_wstr.find(L"Нет") != (-1))		// Если значение "Нет"
				{
					Get_WText_Chain(read_wstr, write_wstr);									// Вынимаем имя пользователя...
					array_string.push_back(std::make_pair(ptr->m_position[0], write_wstr));	// ...и записываем в массив
				}
				
			}
		}
	}
	WriteInFinalyArray(temp_array, array_string, final_array_text, object_file_name_dir);
}

void Find_For_2311(std::wifstream& rfile, std::wstring& read_wstr, std::map<int, std::wstring>& final_array_text, std::vector<MainContainer>::iterator& ptr, std::vector<ObjectName>& object_file_name_dir)
{
	std::vector<std::pair<int, std::wstring>> array_string;
	std::map<int, std::wstring> temp_array;
	std::wstring write_wstr;
	while (true)
	{
		if (read_wstr.find(L"</table>") != (-1))	// Если тело раздела кончилось, прерываем цикл
		{
			break;
		}
		int num_field = MyGetLine(rfile, read_wstr);
		if (read_wstr.find(L"<tr><td class='c'>") != (-1))
		{
			while (true)
			{
				if (read_wstr.find(L"</table>") != (-1))	// Если тело раздела кончилось, прерываем цикл
				{
					break;
				}
				Get_WText_Chain(read_wstr, write_wstr);									// Вынимаем имя пользователя...
				array_string.push_back(std::make_pair(ptr->m_position[0], write_wstr));	// ...и записываем в массив
				MyGetLine(rfile, read_wstr);
			}
		}
	}
	WriteInFinalyArray(temp_array, array_string, final_array_text, object_file_name_dir);
}

int main(void)
{
	setlocale(LC_ALL, "Rus.utf8");						// Для отоброжения русских символов в utf8
	srand(static_cast<unsigned>(time(NULL)));

	std::vector<MainContainer> main_array;				// Объявляем главный контейнер содержащий захардкоженую информацию для парсинга
	std::vector<ObjectName> object_file_name_dir;		// Для сопостовления имени цеха-объекта-имени файла	
	std::wifstream rfile;
	std::wofstream wfile;
	//const std::wstring dir(L"C:\\Users\\Administrator\\Desktop\\dng1");							// Начальная дирректория
	std::wstring current_path(fs::current_path().wstring());
	const std::wstring dir(current_path);														// Начальная дирректория
	//const wchar_t WRITE_FILE_PATH[] = L"C:\\Users\\Administrator\\Desktop\\dng1\\test.txt";		// -----------Удалить после объединения
	const std::wstring WRITE_FILE_PATH(current_path + L"\\parser.txt");		// -----------Удалить после объединения
	
	// ----------------------Перебор дирректорий и поиск файлов для парсинга----------------------------------------------------
	std::vector<std::filesystem::path> subdirs;
	subdirs.push_back(fs::path(dir));								// Вставляем текущую дирректорию начальной
	{
		// Получить список всех дирректорий от указанной директории и в глубь
		fs::recursive_directory_iterator begin(dir);
		fs::recursive_directory_iterator end;
		std::copy_if(begin, end, std::back_inserter(subdirs), [](const fs::path& path) {
			return fs::is_directory(path);
			});
	}

	for (auto& ptr : subdirs)										// Получаем список файлов из всех поддиректорий
	{
		fs::directory_iterator begin(ptr);
		fs::directory_iterator end;
		std::vector<fs::path> temp_htm_files;						// Массив для проверки, есть ли в папке файлы с указанным расширением
		std::copy_if(begin, end, std::back_inserter(temp_htm_files), [](const fs::path& path) {
			return fs::is_regular_file(path) && (path.extension() == ".htm");
			});
		size_t pos = ptr.wstring().rfind(L"\\");
		std::wstring folder_name = ptr.wstring().substr(pos + 1);	// Вынимаем имя папки
		if (temp_htm_files.empty())									// Проверка есть ли в папке файлы с указанным расширением 
		{
			std::wcout << L"Папка: " << folder_name << L" пуста.\n";
		}
		else
		{
			std::wstring temp_path = ptr.wstring();
			std::wstring temp_text;
			std::wstring temp_workshop_name;
			std::wstring temp_object_name;
			int pos_first_start = (temp_path.find(dir) + dir.size());
			int pos_first_end = (temp_path.find(L'\\', pos_first_start));

			if (pos_first_start < temp_path.size())					// Защита от разбора корневого католога
			{
				temp_text = temp_path.substr(pos_first_start + 1);
				int pos_second_start = temp_text.find(L'\\');
				if (pos_second_start != (-1))
				{
					auto it_start = temp_text.begin();
					auto it_end = temp_text.begin();
					it_end += (pos_second_start);
					for (; it_start != it_end; ++it_start)			// Вынимание имени цеха
					{
						temp_workshop_name.push_back(*it_start);
					}
					int pos_second_end = temp_text.find(L'\\', pos_second_start + 1);
					if (pos_second_end != -1)						// Вынимание имени объекта
					{
						auto it_second_start = temp_text.begin();
						auto it_second_end = temp_text.begin();;
						it_second_start += (pos_second_start + 1);
						it_second_end += (pos_second_end);
						for (; it_second_start != it_second_end; ++it_second_start)
						{
							temp_object_name.push_back(*it_second_start);
						}
					}
					else											// Вынимание имени объекта
					{
						temp_object_name.append(temp_text.substr(pos_second_start + 1));
					}
				}
				else												// Вынимание имени цеха
				{
					temp_workshop_name.append(temp_text);
				}
			}
			for (auto& ptr1 : temp_htm_files)						// Заполняем массив дирректорий необходимых файлов
			{
				object_file_name_dir.push_back(ObjectName(temp_workshop_name, temp_object_name, ptr1.wstring()));
			}
		}
	}

	//for (auto ptr_path = object_file_name_dir.begin(); ptr_path != object_file_name_dir.end(); ++ptr_path)	//-----------Удалить
	//{
	//	std::wcout << ptr_path->m_workshop << L'\t' << ptr_path->m_object << L'\n';
	//}

	// --------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------Парсинг файла---------------------------------------------------------------------
	for (auto ptr_path = object_file_name_dir.begin(); ptr_path != object_file_name_dir.end(); ++ptr_path)
	{
		//system("cls");									//------------Удалить
		//num_field = 0;									//------------Удалить
		std::wcout << ptr_path->m_file_path << L'\n';

		std::vector<std::wstring> menu;						// Массив (для формирования оглавления)
		std::map<int, std::wstring> final_array_text;		// Для строго определенного порядка записей
		std::wstring read_wstr;								// сюда будем класть считанные из файла строки
		std::wstring write_wstr;							// сюда будем класть строки перед записью в файл	
		// -----------------------------------Загрузка "оглавления" из файла-----------------------------------------------------
		rfile.open(ptr_path->m_file_path);	// файл из которого читаем (для линукс путь будет выглядеть по другому)
		rfile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		while (!rfile.eof())
		{
			std::getline(rfile, read_wstr);
			int pos_id_start = read_wstr.find(L"<li><a href='#i");		// Для расчета начала id
			int pos_id_end = read_wstr.find(L"'>");						// Для рассчета начала наименования раздела
			//int pos_end_name = read_string.find("</a></li>");			// Для рассчета окончания наименования раздела

			//Вставка наименования раздела
			if (pos_id_start != (-1))
			{
				if (pos_id_end != (-1))
				{
					Search_And_Copy_WStr((pos_id_start + 15), (pos_id_end - 1), read_wstr, write_wstr);
					menu.push_back(write_wstr);							// Помещаем id пунктов оглавления в массив
				}
			}
		}
		// Проверяем есть ли в текущем файле необходимые разделы, если есть то формируем главный массив из имеющихся
		for (auto ptr1 = menu.begin(); ptr1 != menu.end(); ++ptr1)
		{
			for (auto ptr2 = need_info.begin(); ptr2 != need_info.end(); ++ptr2)
			{
				if (ptr1->find(ptr2->first.c_str()) != (-1))			// если найдено соответствие
				{
					main_array.push_back(MainContainer(ptr2->first, ptr2->second.first, ptr2->second.second));
				}
			}
		}
		rfile.close();
		// ----------------------------------------------------------------------------------------------------------------------
		// --------------------------------------Сбор информации и запись в файл-------------------------------------------------
		rfile.open(ptr_path->m_file_path);
		wfile.open(WRITE_FILE_PATH, std::ios::app);
		rfile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		wfile.imbue(std::locale("Ru.utf8"));

		if (!main_array.empty())											// Если меню не пусто
		{
			while (!rfile.eof())
			{
				read_wstr.erase();
				MyGetLine(rfile, read_wstr);								//-------------Переделать

				auto pos_header_start = read_wstr.find(L"<h2 id='i");		// Ищем начало id заголовка
				if (pos_header_start != (-1))								// Если начало заголовка найдено
				{
					auto pos_header_end = read_wstr.find(L"'>");			// Ищем конец id заголовка
					Search_And_Copy_WStr((pos_header_start + 9), (pos_header_end - 1), read_wstr, write_wstr); // Вынимаем id заголовка
					//Сравниваем полученный id заголовока с главным массивом (захардкоденой информации)
					for (auto ptr = main_array.begin(); ptr != main_array.end(); ++ptr)
					{
						if (write_wstr.find(ptr->m_id.c_str()) != (-1))		// Перебираем массив и ищем совпадение id
						{
							MyGetLine(rfile, read_wstr);					//-------------Переделать
							if (read_wstr.find(L"<table") != (-1))			// Начало тела раздела
							{
								if (ptr->m_id.find(L"2056") != (-1))		// Если раздел "Устройства памяти" - оперативка
								{
									Find_For_2056(rfile, read_wstr, final_array_text, ptr, object_file_name_dir);
									write_wstr.erase();
								}
								else if (ptr->m_id.find(L"2055") != (-1))	// Если раздел "Процессоры"
								{
									Find_For_2055(rfile, read_wstr, final_array_text, ptr, object_file_name_dir);
									write_wstr.erase();
								}
								else if (ptr->m_id.find(L"2201") != (-1))	// Сетевые адаптеры
								{
									Find_For_2201(rfile, read_wstr, final_array_text, ptr, object_file_name_dir);
									write_wstr.erase();
								}
								else if (ptr->m_id.find(L"2350") != (-1))	// Пользователи
								{
									Find_For_2350(rfile, read_wstr, final_array_text, ptr, object_file_name_dir);
									write_wstr.erase();
								}
								else if (ptr->m_id.find(L"2311") != (-1))	// Активные сесии
								{
									Find_For_2311(rfile, read_wstr, final_array_text, ptr, object_file_name_dir);
									write_wstr.erase();
								}
								else
								{
									Find_For_Test(rfile, read_wstr, final_array_text, ptr, object_file_name_dir);
									write_wstr.erase();
								}
							}
						}
					}
				}
			}
			final_array_text.insert(std::make_pair(1, ptr_path->m_workshop));	// Добавляем имя цеха
			final_array_text.insert(std::make_pair(2, ptr_path->m_object));		// Добавляем имя объекта
			// Удаляем лишние запятые из всех строк, и если строка пуста записываем в нее прочерк "-"
			for (auto ptr = final_array_text.begin(); ptr != final_array_text.end(); ++ptr)
			{
				bool check = false;
				while (true)												// Удаляем запятые в конце строки
				{
					if (ptr->second.size() > 0)
					{
						if (ptr->second[ptr->second.size() - 1] == L',' || ptr->second[ptr->second.size() - 1] == L' ')
						{
							ptr->second.pop_back();
							check = true;
							continue;
						}
						//else if (!check)
						//{
						//	ptr->second.pop_back();
						//}
						if (check)
							break;
					}

					break;
				}
				while (true)												// Удаляем запятые в начале строки
				{
					if (ptr->second.size() > 0)
					{
						if (ptr->second[0] == L',')
						{
							ptr->second.erase(0, 1);
							continue;
						}
						if (ptr->second[0] == L' ')
						{
							ptr->second.erase(0, 1);
							continue;
						}
					}
					break;
				}
				if (ptr->second.size() == 0)
				{
					ptr->second = '-';
				}
			}

			// Перебираем массив и ищем отсутствующие ячейки, затем создаем их и заполняем их "-"...
			std::set<int> int_array;
			for (auto ptr_temp = final_array_text.begin(); ptr_temp != final_array_text.end(); ++ptr_temp)
			{
				int_array.insert(ptr_temp->first);			// Создаем массив "имеющихся" ячеек
			}

			for (int i = 1; i < (MAX_COL + 1); ++i)
			{
				bool check = false;
				for (auto ptr_temp = int_array.begin(); ptr_temp != int_array.end(); ++ptr_temp)
				{
					//int empty_num = -1;
					if ((*ptr_temp) == i)
						check == true;
				}
				if (!check)
				{
					final_array_text.insert(std::make_pair(i, L"-"));
				}
			}

			//---------------------------------------------Запись в файл----------------------------------------------------------
			bool is_empty = true;
			int count = 0;
			for (auto ptr_temp = final_array_text.begin(); ptr_temp != final_array_text.end(); ++ptr_temp)	// Пуста ли строка
			{
				if (count > 1)
				{
					if (ptr_temp->second == L"-" || ptr_temp->second == L"-\n")
					{
					}
					else
					{
						is_empty = false;
					}
				}
				++count;
			}
			if (!is_empty)	// Если не пуста то записываем ее в файл
			{
				for (auto ptr_temp = final_array_text.begin(); ptr_temp != final_array_text.end(); ++ptr_temp)
				{
					if (!is_empty)
					{
						wfile << ptr_temp->second << '\t';
					}
				}
				wfile << L'\n';		// Добавить в конец ячейки 38
			}
			// -------------------------------------------------------------------------------------------------------------------
		}
		else
		{
			std::wcout << L"Отсутствует оглавление в файле, без оглавления невозможно произвести парсинг!\n";
			std::wcout << L"Поэто файл по следующему пути будет пропущен: " << ptr_path->m_file_path << L"\n\n";
		}

		rfile.close();
		wfile.close();
	}

	std::wcout << L"\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
	std::wcout << L"-----------------ПАРСИНГ ЗАВЕРШЕН-----------------\n";
	std::wcout << L"\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
	system("pause");

	return 0;
}