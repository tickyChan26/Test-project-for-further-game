#include <Aurora/Config.hpp>
#include <Thor/Input.hpp>
#include <Thor/Input/ActionMap.hpp>
#include <Thor/Animations.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window/Keyboard.hpp>


#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstdlib>  // Для rand() и srand()
#include <ctime>    // Для time()
#include <random>
#include "tinyxml2.h"


#pragma comment(lib, "Shell32.lib")


using namespace std;

//////////////   глобальные переменные

// отдача //
static int Save_ID;
static int Last_Save_ID;
static string Name_Save;

//Игрок
static int Money;
static int Player_Health_Max, Player_Health;
static int Player_Scharge_Max, Player_Scharge;
static float Start_Pos_Player_X = 0, Start_Pos_Player_Y = 5000 - 260;
// приём //
extern string Path_directory_game; // путь к игре



class SaveData
{
public:

	//тех параметры сохранения

	// уровень
	int Level_Num = 0;


	SaveData(sf::RenderWindow& windowRef)
	{
		
	}
	void SAVEDATA_STARDANT_LOAD()
	{
		cout << "----------------------------------------------------------------\n";
		cout << ">>> Завершена загрузка стандартных параметров класса SaveData \n\n";
	}


	void Detect_ID_Saves()
	{	
		//детектим какой сейв используется
		if (Last_Save_ID == 1)
		{
			Save_ID = 1;
			Name_Save = "Saves_1";
		}
		if (Last_Save_ID == 2)
		{
			Save_ID = 2;
			Name_Save = "Saves_2";
		}
		if (Last_Save_ID == 3)
		{
			Save_ID = 3;
			Name_Save = "Saves_3";
		}
		if (Last_Save_ID == 4)
		{
			Save_ID = 4;
			Name_Save = "Saves_4";
		}
		if (Last_Save_ID == 5)
		{
			Save_ID = 5;
			Name_Save = "Saves_5";
		}
		if (Last_Save_ID == 6)
		{
			Save_ID = 6;
			Name_Save = "Saves_6";
		}
	}

	void Save_Saves_Data()
	{
		ofstream file(Path_directory_game + "Saves\\" + Name_Save + ".ini"); // Открываем файл для записи
		if (!file)
		{
			cout << Path_directory_game + "Saves\\" + Name_Save + ".ini  не найден\n";
			return;
		}
		file.close();

		cout << Path_directory_game + "Saves\\" + Name_Save + ".ini  Cохранение завершенно\n";
	}

	void Load_Saves_Data()
	{

		ifstream file(Path_directory_game + "Saves\\" + Name_Save + ".ini"); // Открываем файл для чтения
		if (!file)
		{
			cout << Path_directory_game + "Файл [ Saves\\" + Name_Save + ".ini ]   Не найден будет созданно новое сохранение!\n";
			Create_Save_With_Defalut_Data();
		}
		else 
		{  
			cout << Path_directory_game + "Файл [ Saves\\" + Name_Save + ".ini ]    Загружен!\n";
		
		}
		



		file.close();
	}
private:

	//////////////        значение по умолчанию       //////////////


	 //ещё добавить при нажатии             новая игра   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	void Create_Save_With_Defalut_Data()
	{
		// игрок
        Player_Health = 50;

		// уровень
		int Level_Num = 0;


		ofstream file(Path_directory_game + "Saves\\" + Name_Save + ".ini"); // Открываем файл для записи
		if (!file)
		{
			cout << Path_directory_game + "Saves\\" + Name_Save + ".ini  не найден\n";
			return;
		}

// переписываем параметры и ставим их по умолчанию
 
		// игрок
		file << "Player_Health=" << Player_Health << "\n";
		// уровень
		file << "Level_Num=" << Level_Num << "\n";
		file.close();

		cout << "Create_Save_With_Defalut_Data    Новые данные игры с нуля загруженны ! \n";
	}


};

