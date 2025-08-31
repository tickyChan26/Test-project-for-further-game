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
#include <cstdlib>  // ��� rand() � srand()
#include <ctime>    // ��� time()
#include <random>
#include "tinyxml2.h"


#pragma comment(lib, "Shell32.lib")


using namespace std;

//////////////   ���������� ����������

// ������ //
static int Save_ID;
static int Last_Save_ID;
static string Name_Save;

//�����
static int Money;
static int Player_Health_Max, Player_Health;
static int Player_Scharge_Max, Player_Scharge;
static float Start_Pos_Player_X = 0, Start_Pos_Player_Y = 5000 - 260;
// ���� //
extern string Path_directory_game; // ���� � ����



class SaveData
{
public:

	//��� ��������� ����������

	// �������
	int Level_Num = 0;


	SaveData(sf::RenderWindow& windowRef)
	{
		
	}
	void SAVEDATA_STARDANT_LOAD()
	{
		cout << "----------------------------------------------------------------\n";
		cout << ">>> ��������� �������� ����������� ���������� ������ SaveData \n\n";
	}


	void Detect_ID_Saves()
	{	
		//�������� ����� ���� ������������
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
		ofstream file(Path_directory_game + "Saves\\" + Name_Save + ".ini"); // ��������� ���� ��� ������
		if (!file)
		{
			cout << Path_directory_game + "Saves\\" + Name_Save + ".ini  �� ������\n";
			return;
		}
		file.close();

		cout << Path_directory_game + "Saves\\" + Name_Save + ".ini  C��������� ����������\n";
	}

	void Load_Saves_Data()
	{

		ifstream file(Path_directory_game + "Saves\\" + Name_Save + ".ini"); // ��������� ���� ��� ������
		if (!file)
		{
			cout << Path_directory_game + "���� [ Saves\\" + Name_Save + ".ini ]   �� ������ ����� �������� ����� ����������!\n";
			Create_Save_With_Defalut_Data();
		}
		else 
		{  
			cout << Path_directory_game + "���� [ Saves\\" + Name_Save + ".ini ]    ��������!\n";
		
		}
		



		file.close();
	}
private:

	//////////////        �������� �� ���������       //////////////


	 //��� �������� ��� �������             ����� ����   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	void Create_Save_With_Defalut_Data()
	{
		// �����
        Player_Health = 50;

		// �������
		int Level_Num = 0;


		ofstream file(Path_directory_game + "Saves\\" + Name_Save + ".ini"); // ��������� ���� ��� ������
		if (!file)
		{
			cout << Path_directory_game + "Saves\\" + Name_Save + ".ini  �� ������\n";
			return;
		}

// ������������ ��������� � ������ �� �� ���������
 
		// �����
		file << "Player_Health=" << Player_Health << "\n";
		// �������
		file << "Level_Num=" << Level_Num << "\n";
		file.close();

		cout << "Create_Save_With_Defalut_Data    ����� ������ ���� � ���� ���������� ! \n";
	}


};

