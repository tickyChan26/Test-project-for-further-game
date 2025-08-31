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
static bool startGame; static bool press_NewGame; 

static sf::Font Font_words;
static sf::Font Font_no_words;

static vector<sf::Texture> MASIV_Cursor_Texture;
static sf::RectangleShape Cursor; static sf::RectangleShape Vignette;

static bool VSYNC = false; 
static bool FPS_ON = true;
static sf::Text Text_MaxFPS_num;

// приём //
extern string Path_directory_game; // путь к игре
extern string Name_Save; // название сейва
extern int Last_Save_ID; //номер  последнего использованного сохранения
extern sf::Text FpsText;

extern bool Refresh_VSYNC;  
extern bool Refresh_MAXFPS; extern int MAXFPS;


extern sf::Text Text_PAUSE;
      extern sf::Text Text_SaveGame;
      extern sf::Text Text_Exit;

class Menu
{
public:
    
///////////////////////////////////////////  запуск меню  /////////////////////////////////////////////////
    int One_load_Logo = 0;

    Menu(sf::RenderWindow& windowRef) : window(windowRef)
    {
       
    }
    void MENU_STARDANT_LOAD()
    {
        cout << "----------------------------------------------------------------\n";

        press_NewGame = false;
        loadSettings_Last_Save_ID(); // загружаем ID последнего сейва для того что бы игрок мог нажать просто PLAY
        Check_saves(); // проверяем наличие сейвов
        load_sounds(); //загрузка музыки-звуков
        loadSettings_Lenguage(); // загрузка языка
        if (One_load_Logo == 0) { One_load_Logo = 1; Logo_play(); } // запуск лого   

        UI_load_texture(); //загрузка текстур меню
        load_ttf(); //загрузка шрифта
        UI_load_text(); // задаётся текст в зависимости от языка
        Basic_text_setting();  // базовая настройка текста (цвет, размер, шрифт)

        //////// загрузка цыфр при их максимальном значении

        sf::Vector2u Menu_Size = window.getSize();
        MAXFPS = 222;
        UI_dynamic_text(); // задаётся текст который меняется
        UI_menu_load(Menu_Size);
        UI_load_setting(Menu_Size); //загрузка интерфейса меню (размеры.   расположение)




        //////// загрузка цыфр при их реальных значениях без замены размера

        loadSettings(); //загрузка настроек c ini
        UI_dynamic_text(); // задаётся текст который меняется
        

        Vignette_setting(); //расчёт силы виньетки
        Settings_music(); // загрузка параметров музыки
        Settings_FX(); // загрузка параметров звуков


        // двигаем неисправный текст
        if (Lenguage == 11) { Move_text_CHS(); }

        if (Lenguage == 7) { Move_text_KOR(); }

        if (Lenguage == 8) { Move_text_JPN(); }

        cout << ">>> Завершена загрузка стандартных параметров класса Menu \n\n"; 
    }

    // дополнительные мелкие настройки
    void Load_Additional_Settings_Menu()
    {
        
        sf::Vector2u Menu_Size = window.getSize();
        Cursor.setSize(sf::Vector2f(Menu_Size.x * 0.02525252525 / 1.1, Menu_Size.y * 0.04629629629 / 1.2));

        cout << "> Загружены дополнительные настройки меню\n";
    }

//////////////////////////////////////    рендер всего  ////////////////////////////////
    void render()
    {

        // Главные кнопки, и графика меню
        {
            window.draw(Fon_menu);
            window.draw(Clouds_1);
            window.draw(Clouds_2);
            window.draw(Clouds_3);
            window.draw(Clouds_4);
            window.draw(Fon_menu_foreground);


            window.draw(Vignette);

            window.draw(PlayButton);     window.draw(Text_PlayButton);
            window.draw(LoadgameButton); window.draw(Text_LoadgameButton);
            window.draw(SettingsButton); window.draw(Text_SettingsButton);
            window.draw(ExitButton);     window.draw(Text_ExitButton);
        }

        if (isSetting == true) 
        {
            
            window.draw(SettingsPanel_Global);
            window.draw(SettingsPanel_Graphics);
            window.draw(SettingsPanel_Controls);
            
            window.draw(SettingsPanel);       window.draw(Text_SETTINGS);

            if (Global == true) 
            {
               
                window.draw(Text_Resolution);
                window.draw(Text_Sound_FX);
                window.draw(Text_Music);
                window.draw(Text_Vignette);
                window.draw(Text_Fullscreen);


                window.draw(Fon_Sound_FX_down);   window.draw(Text_Sound_FX_down);
                window.draw(Fon_Sound_FX_up);     window.draw(Text_Sound_FX_up);
                window.draw(Fon_Sound_FX_num);    window.draw(Text_Sound_FX_num);


                window.draw(Fon_Music_down);      window.draw(Text_Music_down);
                window.draw(Fon_Music_up);        window.draw(Text_Music_up);
                window.draw(Fon_Music_num);       window.draw(Text_Music_num);


                window.draw(Fon_Vignette_down);   window.draw(Text_Vignette_down);
                window.draw(Fon_Vignette_up);     window.draw(Text_Vignette_up);
                window.draw(Fon_Vignette_num);    window.draw(Text_Vignette_num);

                window.draw(Fon_Fullscreen);
                if (Fullscreen == true || FL != 0) { window.draw(Text_Fullscreen_num); }
                

                window.draw(Fon_Resolution_down); window.draw(Text_Resolution_down);
                window.draw(Fon_Resolution_up);   window.draw(Text_Resolution_up);
                window.draw(Fon_Resolution_num);  window.draw(Text_Resolution_num);

                if (i_Resolution_num == 0) { window.draw(Text_Your_Resolution); }
          

                window.draw(ENG_LenguageButton);
                window.draw(RUS_LenguageButton);
                window.draw(TUR_LenguageButton);
                window.draw(UKR_LenguageButton);
                window.draw(SPA_LenguageButton);
                window.draw(POR_LenguageButton);
                window.draw(POL_LenguageButton);
                window.draw(KOR_LenguageButton);
                window.draw(JPN_LenguageButton);
                window.draw(FRA_LenguageButton);
                window.draw(DEU_LenguageButton);
                window.draw(CHS_LenguageButton);

                if (Changes == true) { window.draw(Text_Changes); } // текст если что-то поменялось

            }
            else if(Graphics == true)
            {
                window.draw(Text_Vsync);   window.draw(Fon_Vsync);               
                if (VSYNC == true || Vsy != 0) { window.draw(Text_Vsync_num); }


                window.draw(Text_MaxFPS); window.draw(Fon_MaxFPS_down);      window.draw(Text_MaxFPS_down);
                                          window.draw(Fon_MaxFPS_up);        window.draw(Text_MaxFPS_up);
                                          window.draw(Fon_MaxFPS_num);       window.draw(Text_MaxFPS_num);

               window.draw(Text_FPS_visibility);   window.draw(Fon_FPS_visibility); 
               if (FPS_ON == true || Vfps != 0) { window.draw(Text_FPS_visibility_num);  }

            }
            else if (Controls == true){}

            window.draw(SaveButton);       window.draw(Text_SaveButton);
            window.draw(CancelButton);     window.draw(Text_CancelButton);

        }

        if (isLoad  == true)
        {
            window.draw(SavePanel);
            
            window.draw(Text_SAVE_1);
            window.draw(Text_SAVE_2);
            window.draw(Text_SAVE_3);
            window.draw(Text_SAVE_4);
            window.draw(Text_SAVE_5);
            window.draw(Text_SAVE_6);

            window.draw(LoadGame_Button_1); window.draw(Text_LoadGame_Button_1);
            window.draw(LoadGame_Button_2); window.draw(Text_LoadGame_Button_2);
            window.draw(LoadGame_Button_3); window.draw(Text_LoadGame_Button_3);
            window.draw(LoadGame_Button_4); window.draw(Text_LoadGame_Button_4);
            window.draw(LoadGame_Button_5); window.draw(Text_LoadGame_Button_5);
            window.draw(LoadGame_Button_6); window.draw(Text_LoadGame_Button_6);
            
            if (is_Save_1 == false) { window.draw(NewGame_Button_1); window.draw(Text_NewGame_Button_1); }
            if (is_Save_2 == false) { window.draw(NewGame_Button_2); window.draw(Text_NewGame_Button_2); }
            if (is_Save_3 == false) { window.draw(NewGame_Button_3); window.draw(Text_NewGame_Button_3); }
            if (is_Save_4 == false) { window.draw(NewGame_Button_4); window.draw(Text_NewGame_Button_4); }
            if (is_Save_5 == false) { window.draw(NewGame_Button_5); window.draw(Text_NewGame_Button_5); }
            if (is_Save_6 == false) { window.draw(NewGame_Button_6); window.draw(Text_NewGame_Button_6); }
            
            if (is_Save_1 == true) { window.draw(DelGame_Button_1); window.draw(Text_DelGame_Button_1); }
            if (is_Save_2 == true) { window.draw(DelGame_Button_2); window.draw(Text_DelGame_Button_2); }
            if (is_Save_3 == true) { window.draw(DelGame_Button_3); window.draw(Text_DelGame_Button_3); }
            if (is_Save_4 == true) { window.draw(DelGame_Button_4); window.draw(Text_DelGame_Button_4); }
            if (is_Save_5 == true) { window.draw(DelGame_Button_5); window.draw(Text_DelGame_Button_5); }
            if (is_Save_6 == true) { window.draw(DelGame_Button_6); window.draw(Text_DelGame_Button_6); }


            if (is_Save_delete == true) 
            {
                window.draw(ConfirmPanel);

                window.draw(Text_DeleteSave);
                window.draw(Yes_Button); window.draw(Text_Yes_Button);
                window.draw(No_Button);  window.draw(Text_No_Button);
            }
            
        }       
        
    }


/////////////////////////////  анимации в меню  ////////////////////////



    sf::Clock clock;
    int indd;
    int C_speed_1 = 15; // Скорость облака 1
    int C_speed_2 = 20; // Скорость облака 2  
    int C_speed_3 = 10; // Скорость облака 3
    int C_speed_4 = 150; // Скорость облака 4

    bool Play_button_press = false, Save_button_press = false;
    sf::Clock Clock_Button;
    int Misk = 0;
    float Interval = 0.09f; // Интервал смены цвета в секундах
    int rd = 1;
    int Random_Number(int min, int max)
    {
        if (rd == 1) 
        {
            rd = 0;
            srand(time(0));
        }
        static std::random_device rd;     // Источник случайности
        static std::mt19937 gen(rd());    // Генератор Mersenne Twister
        std::uniform_int_distribution<int> dist(min, max);

        return dist(gen);
    }
    

    void update(float deltaTime)
    {

        sf::Vector2u Menu_Size = window.getSize();   
        
        if (Play_button_press == true && Clock_Button.getElapsedTime().asSeconds() >= Interval)
        {

            // Меняем цвет текста
            if (Text_PlayButton.getFillColor() == sf::Color::Black)
            {
                Text_PlayButton.setFillColor(sf::Color::White);
            }
            else { Text_PlayButton.setFillColor(sf::Color::Black); }

            Misk++;
            if (Misk == 10)
            {
                Misk = 0;
                Play_button_press = false;
                Text_PlayButton.setFillColor(sf::Color::Black);

                if (Last_Save_ID == 0 || Last_Save_ID > 6) { Last_Save_ID = 1; }// проверка на на личие багов с сейвом, если сейв нулевой или больше 6 
                startGame = true; // Устанавливаем флаг начала игры  

            }

            Clock_Button.restart(); // Перезапускаем таймер

        }

        if (Save_button_press == true && Clock_Button.getElapsedTime().asSeconds() >= Interval)
        {
            // Меняем цвет текста
            if (Text_SaveButton.getFillColor() == sf::Color::Black)
            {
                Text_SaveButton.setFillColor(sf::Color::White);
            }
            else { Text_SaveButton.setFillColor(sf::Color::Black); }

            Misk++;
            if (Misk == 5)
            {
                Misk = 0;
                Save_button_press = false;
                Text_SaveButton.setFillColor(sf::Color::Black);

            }

            Clock_Button.restart(); // Перезапускаем таймер

        }

        move_Clouds(deltaTime, Menu_Size); // Двигаем облака

        move_Panels(deltaTime);

        
    }  

    //////////////// Двигаем облака ////////////////
    sf::Vector2f int_size_texture_clouds(size_t ind, sf::Vector2u Menu_Size)
    {
        sf::Vector2f size_cloud;

        if (ind == 0)
        {
            size_cloud.x = Menu_Size.x * 0.17708333333;
            size_cloud.y = Menu_Size.y * 0.06481481481;
        }
        else if (ind == 1)
        {
            size_cloud.x = Menu_Size.x * 0.21354166666;
            size_cloud.y = Menu_Size.y * 0.09259259259;
        }
        else if (ind == 2)
        {
            size_cloud.x = Menu_Size.x * 0.08854166666;
            size_cloud.y = Menu_Size.y * 0.08333333333;
        }
        else if (ind == 3)
        {
            size_cloud.x = Menu_Size.x * 0.16145833333;
            size_cloud.y = Menu_Size.y * 0.10185185185;
        }
        else if (ind == 4)
        {
            size_cloud.x = Menu_Size.x * 0.20833333333;
            size_cloud.y = Menu_Size.y * 0.12962962963;
        }
        else if (ind == 5)
        {
            size_cloud.x = Menu_Size.x * 0.04166666666;
            size_cloud.y = Menu_Size.y * 0.03703703703;
        }

        return size_cloud;
    }
    void move_Clouds(float deltaTime, sf::Vector2u Menu_Size)
    {

            Clouds_1.move(C_speed_1 * deltaTime, 0); // Двигаем вправо 
            if (Clouds_1.getPosition().x >= Menu_Size.x)
            {
                Clouds_1.setPosition( -Clouds_1.getGlobalBounds().width - 250,  /* Телепортируем влево за экран */ Random_Number(-10, Menu_Size.y / 2) /* Случайный Y */ );
            
                size_t ind = Random_Number(0, MASIV_Clouds_Texture.size() - 1); // Случайное число от до максимума масива: на данный момент это текстуры о 0 до 5 (всего их 6 но масив начинается с 0)
                C_speed_1 = Random_Number(15, 150);

                if (indd == ind)
                {
                    if (ind >= MASIV_Clouds_Texture.size() - 1) {ind--;  /* Уменьшаем индекс, если он последний  */}
                    else { ind++;  /* Увеличиваем, если это не последний индекс */ }

                }

                    Clouds_1.setSize(int_size_texture_clouds(ind, Menu_Size)); //задаём размер в зависимости от текстуры
                    Clouds_1.setTexture(&MASIV_Clouds_Texture[ind]); //задаём новую текстуру
                    Clouds_1.setTextureRect(sf::IntRect(0, 0, MASIV_Clouds_Texture[ind].getSize().x, MASIV_Clouds_Texture[ind].getSize().y));
                   
                    indd = ind;
               
            }

            Clouds_2.move(C_speed_2 * deltaTime, 0);
            if (Clouds_2.getPosition().x >= Menu_Size.x)
            {
                Clouds_2.setPosition(-Clouds_2.getGlobalBounds().width - 250, Random_Number(-10, Menu_Size.y / 2));
                size_t ind = Random_Number(0, MASIV_Clouds_Texture.size() - 1);
                C_speed_2 = Random_Number(15, 100);
                if (indd == ind)
                {
                    if (ind >= MASIV_Clouds_Texture.size() - 1) { ind--;  /* Уменьшаем индекс, если он последний  */ }
                    else { ind++;  /* Увеличиваем, если это не последний индекс */ }

                }
                Clouds_2.setSize(int_size_texture_clouds(ind, Menu_Size));
                Clouds_2.setTexture(&MASIV_Clouds_Texture[ind]);
                Clouds_2.setTextureRect(sf::IntRect(0, 0, MASIV_Clouds_Texture[ind].getSize().x, MASIV_Clouds_Texture[ind].getSize().y));
                indd = ind;
            }

            Clouds_3.move(C_speed_3 * deltaTime, 0);
            if (Clouds_3.getPosition().x >= Menu_Size.x)
            {
                Clouds_3.setPosition(-Clouds_3.getGlobalBounds().width - 250, Random_Number(-10, Menu_Size.y / 2));
                size_t ind = Random_Number(0, MASIV_Clouds_Texture.size() - 1);
                C_speed_3 = Random_Number(15, 80);
                if (indd == ind)
                {
                    if (ind >= MASIV_Clouds_Texture.size() - 1) { ind--;  /* Уменьшаем индекс, если он последний  */ }
                    else { ind++;  /* Увеличиваем, если это не последний индекс */ }

                }
                Clouds_3.setSize(int_size_texture_clouds(ind, Menu_Size));
                Clouds_3.setTexture(&MASIV_Clouds_Texture[ind]);
                Clouds_3.setTextureRect(sf::IntRect(0, 0, MASIV_Clouds_Texture[ind].getSize().x, MASIV_Clouds_Texture[ind].getSize().y));
                indd = ind;
            }

            Clouds_4.move(C_speed_4 * deltaTime, 0);
            if (Clouds_4.getPosition().x >= Menu_Size.x)
            {
                Clouds_4.setPosition(-Clouds_4.getGlobalBounds().width - 250, Random_Number(-10, Menu_Size.y / 2));

                size_t ind = Random_Number(0, MASIV_Clouds_Texture.size() - 1);
                C_speed_4 = Random_Number(15, 90);
                if (indd == ind)
                {
                    if (ind >= MASIV_Clouds_Texture.size() - 1) { ind--;  /* Уменьшаем индекс, если он последний  */ }
                    else { ind++;  /* Увеличиваем, если это не последний индекс */ }

                }

                Clouds_4.setSize(int_size_texture_clouds(ind, Menu_Size));
                Clouds_4.setTexture(&MASIV_Clouds_Texture[ind]);
                Clouds_4.setTextureRect(sf::IntRect(0, 0, MASIV_Clouds_Texture[ind].getSize().x, MASIV_Clouds_Texture[ind].getSize().y));

                indd = ind;
            }

    }
    


    //////////////// Двигаем панели ////////////////
    float Speed_panels = 10, Hide_Pos, Open_Pos;
    float  lerp(float  start, float  end, float t)
    {
        return start + (end - start) * t;
    }
    void move_Panels(float deltaTime)
    {
       
        if(Global == true && Global_Animation == true)
        {
            float Global_X = SettingsPanel_Global.getPosition().x;
            float Distance = abs(Open_Pos - Global_X);
            float SpeedBoost = max(clamp(Distance / 20.0f, 0.1f, 1.0f), 0.35f);

            SettingsPanel_Global.setPosition(lerp(Global_X, Open_Pos, Speed_panels * SpeedBoost * deltaTime), SettingsPanel_Global.getPosition().y);
            if (Distance < 2.0f) { Global_Animation = false; }  
            SettingsPanel_Global.setFillColor(sf::Color(255, 255, 255));
        }
        else if(Global == false && Global_Animation == true)
        {
            float Global_X = SettingsPanel_Global.getPosition().x;
            float Distance = abs(Hide_Pos - Global_X);
            float SpeedBoost = max(clamp(Distance / 20.0f, 0.1f, 1.0f), 0.35f);

            SettingsPanel_Global.setPosition(lerp(Global_X, Hide_Pos, Speed_panels * SpeedBoost * deltaTime), SettingsPanel_Global.getPosition().y);
            if (Distance < 2.0f) { Global_Animation = false; }  
            SettingsPanel_Global.setFillColor(sf::Color(100, 100, 100));
        }

        if (Graphics == true && Graphics_Animation == true)
        {
            float Graphics_X = SettingsPanel_Graphics.getPosition().x;
            float Distance = abs(Open_Pos - Graphics_X);
            float SpeedBoost = max(clamp(Distance / 20.0f, 0.1f, 1.0f), 0.35f);

            SettingsPanel_Graphics.setPosition(lerp(Graphics_X, Open_Pos, Speed_panels * SpeedBoost * deltaTime), SettingsPanel_Graphics.getPosition().y);
            if (Distance < 2.0f) { Graphics_Animation = false; }
            SettingsPanel_Graphics.setFillColor(sf::Color(255, 255, 255));
        }
        else if(Graphics == false && Graphics_Animation == true)
        {
            float Graphics_X = SettingsPanel_Graphics.getPosition().x;
            float Distance = abs(Hide_Pos - Graphics_X);
            float SpeedBoost = max(clamp(Distance / 20.0f, 0.1f, 1.0f), 0.35f);

            SettingsPanel_Graphics.setPosition(lerp(Graphics_X, Hide_Pos, Speed_panels * SpeedBoost * deltaTime), SettingsPanel_Graphics.getPosition().y);
            if (Distance < 2.0f) { Graphics_Animation = false; }
            SettingsPanel_Graphics.setFillColor(sf::Color(100, 100, 100));
        }

        if (Controls == true && Controls_Animation == true)
        {
            float Controls_X = SettingsPanel_Controls.getPosition().x;
            float Distance = abs(Open_Pos - Controls_X);
            float SpeedBoost = max(clamp(Distance / 20.0f, 0.1f, 1.0f), 0.35f);

            SettingsPanel_Controls.setPosition(lerp(Controls_X, Open_Pos, Speed_panels * SpeedBoost * deltaTime), SettingsPanel_Controls.getPosition().y);
            if (Distance < 2.0f) { Controls_Animation = false; }
            SettingsPanel_Controls.setFillColor(sf::Color(255, 255, 255));
        }
        else if (Controls == false && Controls_Animation == true)
        {
            float Controls_X = SettingsPanel_Controls.getPosition().x;
            float Distance = abs(Hide_Pos - Controls_X);
            float SpeedBoost = max(clamp(Distance / 20.0f, 0.1f, 1.0f), 0.35f);

            SettingsPanel_Controls.setPosition(lerp(Controls_X, Hide_Pos, Speed_panels * SpeedBoost * deltaTime), SettingsPanel_Controls.getPosition().y);
            if (Distance < 2.0f) { Controls_Animation = false; }
            SettingsPanel_Controls.setFillColor(sf::Color(100, 100, 100));
        }

    }




/////////////////////////////  функции  ////////////////////////
    // запуск музыки
    void play_music() 
    {
            Menu_music_1.play();
    }

    //  функция воспроизведения звуков при наведении на главные кнопки
    void Move_button_sound()
    {

            int ind = Random_Number(1, 3); // Случайное число от до
        if (ind == 1) { Move_batton_1.play(); }
        if (ind == 2) { Move_batton_2.play(); }
        if (ind == 3) { Move_batton_3.play(); }
    
    }



    //  функция обновления мыши (использую при запуске лого)
    void Update_Mouse()
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window); // Получаем координаты мыши

        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::MouseButtonPressed) // Нажатие кнопки
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (Loading == true) { Loading = false; Load_sound.stop(); }
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (Loading == true) { Loading = false; Load_sound.stop(); }
                }
            }
        }
    }
    // воспроизведение лого 
    float FrameLoadTime = 0.023f; int Transparency = 255;  bool Loading = true;
    sf::Clock Frame_Clock; // Таймер кадров

    sf::RectangleShape Logo;   sf::Texture Logo_Texture;  sf::RectangleShape Dark_logo;
    void Logo_play()
    {
        //загружаем один раз
        {

            sf::Vector2u Menu_Size = window.getSize();

            // текстура лого
            sf::Texture* LogoTexture;
            LogoTexture = &Logo_Texture;
            if (!Logo_Texture.loadFromFile(Path_directory_game + "Texture\\Menu\\Logo.png"))
            {
                std::cout << "Ошибка загрузки логотипа!" << std::endl;
            }
            Logo.setTexture(LogoTexture);

            // Настройка затемнения
            Dark_logo.setSize(sf::Vector2f(Menu_Size.x, Menu_Size.y));
            Dark_logo.setPosition(0, 0);
            Dark_logo.setFillColor(sf::Color(0, 0, 0, Transparency));

            Logo.setSize(sf::Vector2f(Menu_Size.x, Menu_Size.y));
            Logo.setPosition(0, 0);

            Load_sound.play();

        }

        while (Loading = true)
        {
            Update_Mouse();

            float Elapsed = Frame_Clock.getElapsedTime().asSeconds();
            if (Elapsed >= FrameLoadTime)
            {
                Transparency = Transparency - 2;
                if (Transparency <= 0)
                {
                    Transparency = 0;
                    //завершаем загрузку
                    if (Loading == true) { Loading = false; Load_sound.stop(); }
                }

                Dark_logo.setFillColor(sf::Color(0, 0, 0, Transparency));
                Frame_Clock.restart();
            }

            window.clear();
            window.draw(Logo);
            window.draw(Dark_logo);
            window.display();

            if (Loading == false) { break; }
        }
    }

/////////////////////////////   флаги и всякое мелкое  ////////////////////////

    // Загружаем шрифт
     string Path_arial;
    void load_ttf()
    {
        // ENG
        if (Lenguage == 0)
        {
            Path_arial = Path_directory_game + "Language\\ENG.ttf";
        }
        // RUS
        if (Lenguage == 1)
        {
            Path_arial = Path_directory_game + "Language\\RUS.ttf";
        }
        // TUR
        if (Lenguage == 2)
        {
            Path_arial = Path_directory_game + "Language\\TUR.ttf";
        }
        // UKR
        if (Lenguage == 3)
        {
            Path_arial = Path_directory_game + "Language\\UKR.ttf";
        }
        // SPA
        if (Lenguage == 4)
        {
            Path_arial = Path_directory_game + "Language\\SPA.ttf";
        }
        // POR
        if (Lenguage == 5)
        {
            Path_arial = Path_directory_game + "Language\\POR.ttf";
        }
        // POL
        if (Lenguage == 6)
        {
            Path_arial = Path_directory_game + "Language\\POL.ttf";
        }
        // KOR
        if (Lenguage == 7)
        {
            Path_arial = Path_directory_game + "Language\\KOR.ttf";
        }
        // JPN
        if (Lenguage == 8)
        {
            Path_arial = Path_directory_game + "Language\\JPN.ttf";
        }
        // FRA
        if (Lenguage == 9)
        {
            Path_arial = Path_directory_game + "Language\\FRA.ttf";
        }
        // DEU
        if (Lenguage == 10)
        {
            Path_arial = Path_directory_game + "Language\\DEU.ttf";
        }
        // CHS
        if (Lenguage == 11)
        {
            Path_arial = Path_directory_game + "Language\\CHS.ttf";

        }

        // Загружаем шрифт для слов
        if (!Font_words.loadFromFile(Path_arial))
        {
            cout << "Ошибка загрузки шрифта! " << Path_arial << endl;
        }

        // Загружаем шрифт для не слов
        if (!Font_no_words.loadFromFile(Path_directory_game + "Language\\PressStart2P.ttf"))
        {
            cout << "Ошибка загрузки шрифта! PressStart2P" << endl;
        }
    }



    bool Fullscreen;

    bool isLoad = false;   // нажатия кнопки сохранений
    bool isActive_Save;   // если есть активное(-ые) сохранения

    bool is_Save_1, is_Save_2, is_Save_3, is_Save_4, is_Save_5, is_Save_6;   // если  активное сохранения 1-6
    bool is_Save_delete;   // если какой либо сейв удаляется
    int Save_delete;// номер сейва



    bool isSetting = false;   // нажатия настроек
    bool Changes = false; // если что то поменялось в настройках

    bool Global = true, Global_Animation = true;
    bool Graphics = false, Graphics_Animation = true;
    bool Controls = false, Controls_Animation = true;
    


//////////////////////////     файл Настройки        //////////////////////////////////////////////////////////////////////////////////////////////////////////

    int resolutionX, resolutionY, i_Sound_FX_num = 100, i_Music_num = 100, i_Vignette_num = 100, i_Resolution_num, Lenguage = 0;
    

    ////сохранение настроек
    void saveSettings()
    {

        ofstream file(Path_directory_game + "Settings\\Settings.ini"); // Открываем файл для записи
        if (!file)
        {
            std::cout << "Ошибка открытия файла  Settings.ini" << std::endl;
            return;
        }

        file << "Lenguage=" << Lenguage << "\n\n";

        file << "[Graphics]\n";       
        file << "ResolutionX=" << resolutionX << "\n";
        file << "ResolutionY=" << resolutionY << "\n";
        file << "Fullscreen=" << i_Resolution_num << "\n\n";

        file << "Vignette=" << i_Vignette_num << "\n";
        file << "MusicVolume=" << i_Music_num << "\n";
        file << "MaxFPS=" << MAXFPS << "\n";
        file << "Vsync=" << VSYNC << "\n";
        file << "FPS_ON=" << FPS_ON << "\n\n";
        
        file << "[Audio]\n";
        file << "SoundVolume=" << i_Sound_FX_num << "\n";
        file << "MusicVolume=" << i_Music_num << "\n\n";

        cout << "Сохранение настроек прошло успешно";

        file.close();
    }
    
    ////выгрузка настроек
    void loadSettings()
    {
 
        ifstream file(Path_directory_game + "Settings\\Settings.ini"); // Открываем файл для чтения
        if (!file)
        {
            std::cout << "Ошибка открытия файла  Settings.ini" << std::endl;
            return;
        }

        string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string key;
            if (std::getline(iss, key, '='))
            {
                std::string value;
                if (std::getline(iss, value))
                {
                    if (key == "ResolutionX")      resolutionX =        stoi(value);
                    else if (key == "ResolutionY") resolutionY =        stoi(value);
                    else if (key == "Fullscreen")  i_Resolution_num =   stoi(value);
                    else if (key == "SoundVolume") i_Sound_FX_num =     stoi(value);
                    else if (key == "MusicVolume") i_Music_num =        stoi(value);
                    else if (key == "Vignette") i_Vignette_num = stoi(value);
                    else if (key == "Lenguage") Lenguage = stoi(value);
                    else if (key == "Vsync") VSYNC = stoi(value);
                    else if (key == "MaxFPS") MAXFPS = stoi(value);
                    else if (key == "FPS_ON") FPS_ON = stoi(value);
                    
                }
            }
        }

        if (i_Resolution_num != 0) { Fullscreen = false; }
        if (i_Resolution_num == 0) { Fullscreen = true; }

        file.close();
    }

    ////выгрузка настроек языка
    void loadSettings_Lenguage()
    {
        ifstream file(Path_directory_game + "Settings\\Settings.ini"); // Открываем файл для чтения
        if (!file)
        {
            std::cout << "Ошибка открытия файла  Settings.ini" << std::endl;
            return;
        }

        string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string key;
            if (std::getline(iss, key, '='))
            {
                std::string value;
                if (std::getline(iss, value))
                {
                    if (key == "Lenguage") Lenguage = stoi(value);
                }
            }
        }
        file.close();
    }

    ////выгрузка\сохранение ID последнего сохранения
    void saveSettings_Last_Save_ID()
    {

        ofstream file(Path_directory_game + "Settings\\LastSave.ini"); // Открываем файл для записи
        if (!file)
        {
            std::cout << "Ошибка открытия файла  Settings.ini" << std::endl;
            return;
        }
        file << "LastSave=" << Last_Save_ID << "\n";
        file.close();
        cout << "LastSave сохранилось =   " << Last_Save_ID << "\n";
    }
    void loadSettings_Last_Save_ID()
    {
        ifstream file(Path_directory_game + "Settings\\LastSave.ini"); // Открываем файл для чтения
        if (!file)
        {
            std::cout << "Ошибка открытия файла  LastSave.ini" << std::endl;
            return;
        }
        string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string key;
            if (std::getline(iss, key, '='))
            {
                std::string value;
                if (std::getline(iss, value))
                {
                    if (key == "LastSave") Last_Save_ID = stoi(value);
                }
            }
        }

        file.close();
        cout << "> Последний используемый сейв= " << Last_Save_ID << "\n\n";
    }

    


//////////////////////////     файл сохранения    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Проверка наличия сохранений
    void Check_saves()
    {
        cout << ">> Проверка наличия сохранений...\n";

        // Saves_1
        {
            ifstream file(Path_directory_game + "Saves\\Saves_1.ini"); // Открываем файл для чтения
            if (!file)
            {
                cout << "Файл Saves_1.ini не найден\n"; is_Save_1 = false;
                // если последнее сохранение было это но оно не обнаруженно ставим 0
                if (Last_Save_ID == 1) { Last_Save_ID = 0; cout << "Последнее задействованное сохранение было поврежденно [ Last_Save_ID 1 = 0\n"; } 
            }
            else { is_Save_1 = true; isActive_Save = true; }
            file.close();
        }     
        // Saves_2 
        {
            ifstream file(Path_directory_game + "Saves\\Saves_2.ini"); // Открываем файл для чтения
            if (!file)
            {
                cout << "Файл Saves_2.ini не найден\n"; is_Save_2 = false;
                if (Last_Save_ID == 2) { Last_Save_ID = 0;cout << "Последнее задействованное сохранение было поврежденно [ Last_Save_ID 2 = 0\n"; } 
            } 
            else { is_Save_2 = true; isActive_Save = true; }
            file.close();
        }
        // Saves_3 
        {
            ifstream file(Path_directory_game + "Saves\\Saves_3.ini"); // Открываем файл для чтения
            if (!file)
            {
                cout << "Файл Saves_3.ini не найден\n"; is_Save_3 = false;
                if (Last_Save_ID == 3) { Last_Save_ID = 0;cout << "Последнее задействованное сохранение было поврежденно [ Last_Save_ID 3 = 0\n"; } 
            }
            else { is_Save_3 = true; isActive_Save = true; }
            
            file.close();
        }
        // Saves_4
        {
            ifstream file(Path_directory_game + "Saves\\Saves_4.ini"); // Открываем файл для чтения
            if (!file)
            {
                cout << "Файл Saves_4.ini не найден\n"; is_Save_4 = false;
                if (Last_Save_ID == 4) { Last_Save_ID = 0;cout << "Последнее задействованное сохранение было поврежденно [ Last_Save_ID 4 = 0\n"; } 
            }
            else { is_Save_4 = true; isActive_Save = true; }
            
            file.close();
        }
        // Saves_5
        {
            ifstream file(Path_directory_game + "Saves\\Saves_5.ini"); // Открываем файл для чтения
            if (!file)
            {
                cout << "Файл Saves_5.ini не найден\n"; is_Save_5 = false;
                if (Last_Save_ID == 5) { Last_Save_ID = 0;cout << "Последнее задействованное сохранение было поврежденно [ Last_Save_ID 5 = 0\n"; } 
            }
            else { is_Save_5 = true; isActive_Save = true; }
            
            file.close();
        }
        // Saves_6
        {
            ifstream file(Path_directory_game + "Saves\\Saves_6.ini"); // Открываем файл для чтения
            if (!file)
            {
                cout << "Файл Saves_6.ini не найден\n"; is_Save_6 = false;
                if (Last_Save_ID == 6) { Last_Save_ID = 0; cout << "Последнее задействованное сохранение было поврежденно [ Last_Save_ID 6 = 0\n"; } 
            }
            else { is_Save_6 = true; isActive_Save = true; }
            
            file.close();
        }

        cout << "Проверка наличия сохранений pавершенна\n\n";
    }

    // оперделение и удаление сейва
    void delete_save()
    {
        if (Save_delete == 1)
        {
            if (std::filesystem::remove(Path_directory_game + "Saves\\Saves_1.ini")) // Удаляем файл
            {
                std::cout << "Файл Saves_1 успешно удалён.\n";
                is_Save_1 = false;
            }
            else
            {
                std::cout << "Ошибка удаления файла Saves_1 " << "\n";
            }

        }
        if (Save_delete == 2)
        {
            if (std::filesystem::remove(Path_directory_game + "Saves\\Saves_2.ini")) // Удаляем файл
            {
                std::cout << "Файл Saves_2 успешно удалён.\n";
                is_Save_2 = false;
            }
            else
            {
                std::cout << "Ошибка удаления файла Saves_2 " << "\n";
            }

        }
        if (Save_delete == 3)
        {
            if (std::filesystem::remove(Path_directory_game + "Saves\\Saves_3.ini")) // Удаляем файл
            {
                std::cout << "Файл Saves_3 успешно удалён.\n";
                is_Save_3 = false;
            }
            else
            {
                std::cout << "Ошибка удаления файла Saves_3 " << "\n";
            }

        }
        if (Save_delete == 4)
        {
            if (std::filesystem::remove(Path_directory_game + "Saves\\Saves_4.ini")) // Удаляем файл
            {
                std::cout << "Файл Saves_4 успешно удалён.\n";
                is_Save_4 = false;
            }
            else
            {
                std::cout << "Ошибка удаления файла Saves_4 " << "\n";
            }

        }
        if (Save_delete == 5)
        {
            if (std::filesystem::remove(Path_directory_game + "Saves\\Saves_5.ini")) // Удаляем файл
            {
                std::cout << "Файл Saves_5 успешно удалён.\n";
                is_Save_5 = false;
            }
            else
            {
                std::cout << "Ошибка удаления файла Saves_5 " << "\n";
            }

        }
        if (Save_delete == 6)
        {
            if (std::filesystem::remove(Path_directory_game + "Saves\\Saves_6.ini")) // Удаляем файл
            {
                std::cout << "Файл Saves_6 успешно удалён.\n";
                is_Save_6 = false;
            }
            else
            {
                std::cout << "Ошибка удаления файла Saves_6 " << "\n";
            }

        }

        Color_LoadGame_Button();
        Save_delete = 0;

    }

///////////////////////////////            Обработка событий (нажатие кнопок)         //////////////////////////////////////////////////////////////////////////////////////////////////
    void handleEvents_menu(sf::Event event)
    {   

        if (event.type == sf::Event::MouseButtonPressed) // Если нажата кнопка мыши
        {

            if (event.mouseButton.button == sf::Mouse::Left) // Проверяем, что нажата именно ЛКМ
            {
                           sf::Vector2i mousePos = sf::Mouse::getPosition(window);  

                if (is_Save_delete == false) // если не идёт удаление сейва
                {


                    if (Play_button_press == false)
                    {
                        if (PlayButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        {
                            Play_button_press = true;
                            Confirm_play.play();

                        }
                    }

                    if (isActive_Save == true)
                    {
                        if (LoadgameButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        {
                            while (true)
                            {
                                Confirm_SettingsButton.play();
                                if (isSetting == true) { isSetting = false; }
                                if (isLoad == true) { isLoad = false; break; }
                                if (isLoad == false) { isLoad = true; break; }
                                break;
                            }
                        }
                    }

                    if (SettingsButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        while (true)
                        {
                            Confirm_SettingsButton.play();
                            if (isLoad == true) { isLoad = false; }
                            if (isSetting == true) { isSetting = false; break; }
                            if (isSetting == false) { isSetting = true; break; }
                            break;
                        }
                    }

                    if (ExitButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        window.close();
                    }


                    //кнопки настроек
                    if (isSetting == true)
                    {
                        if (SettingsPanel_Global.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        {
                            Global = true;
                            Graphics = false;
                            Controls = false;

                            Global_Animation = true;
                            Graphics_Animation = true;
                            Controls_Animation = true;
                            
                        }
                        else if (SettingsPanel_Graphics.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        {
                            Global = false;
                            Graphics = true;
                            Controls = false;

                            Global_Animation = true;
                            Graphics_Animation = true; 
                            Controls_Animation = true;
                        }
                        else if (SettingsPanel_Controls.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        {
                            Global = false;
                            Graphics = false;
                            Controls = true;

                            Global_Animation = true;
                            Graphics_Animation = true;
                            Controls_Animation = true;
                        }
                            

                        if (Save_button_press == false)
                        {
                            if (SaveButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                Confirm_settings.play();
                                UI_dynamic_text();
                                saveSettings();
                                Save_button_press = true;
                            }
                        }
                        
                        if (CancelButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        {
                            loadSettings();
                            UI_dynamic_text();
                            Vignette_setting();
                            if (isSetting == true) { isSetting = false; Changes = false; }

                            Refresh_VSYNC = true;
                            Refresh_MAXFPS = true;

                        }


                        if (Global == true)
                        {
                            if (Fon_Sound_FX_down.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                if (i_Sound_FX_num > 0)
                                {
                                    i_Sound_FX_num = i_Sound_FX_num - 5;
                                    Refresh_Sound_FX_Num();
                                    Settings_FX();
                                    Move_batton_down.play();
                                }
                            }
                            else if (Fon_Sound_FX_up.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                if (i_Sound_FX_num < 100)
                                {
                                    i_Sound_FX_num = i_Sound_FX_num + 5;
                                    Refresh_Sound_FX_Num();
                                    Settings_FX();
                                    Move_batton_up.play();
                                }
                            }

                            else if (Fon_Music_down.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {

                                if (i_Music_num > 0)
                                {
                                    i_Music_num = i_Music_num - 5;
                                    Refresh_Music_Num(); 
                                    Settings_music();
                                    Move_batton_down.play();
                                }
                            }
                            else if (Fon_Music_up.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                if (i_Music_num < 100)
                                {
                                    i_Music_num = i_Music_num + 5;
                                    Refresh_Music_Num();
                                    Settings_music();
                                    Move_batton_up.play();
                                }
                            }

                            else if (Fon_Vignette_down.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {

                                if (i_Vignette_num > 0)
                                {
                                    i_Vignette_num = i_Vignette_num - 5;
                                   
                                    Refresh_Vignette_Num(); 
                                    Vignette_setting();
                                    Move_batton_down.play();
                                }
                            }
                            else if (Fon_Vignette_up.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                if (i_Vignette_num < 100)
                                {
                                    i_Vignette_num = i_Vignette_num + 5;
                                    Refresh_Vignette_Num();
                                    Vignette_setting();
                                    Move_batton_up.play();
                                }
                            }

                            else if (Fon_Fullscreen.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                while (true)
                                {
                                    Changes = true;
                                    if (Fullscreen == true) { Fullscreen = false; Move_batton_down.play();  break; }
                                    if (Fullscreen == false) { Fullscreen = true; i_Resolution_num = 0; Refresh_Resolutione_Num(); Move_batton_up.play(); break; }
                                    break;
                                }
                            }
                            
                            else if (Fon_Resolution_down.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                Changes = true;
                                if (i_Resolution_num > 1)
                                {

                                    i_Resolution_num = i_Resolution_num - 1;
                                    Refresh_Resolutione_Num();
                                    if (Fullscreen == true) { Fullscreen = false; }
                                    Move_batton_down.play();

                                }
                                if (i_Resolution_num == 0)
                                {
                                    i_Resolution_num = 1;
                                    Refresh_Resolutione_Num();
                                    if (Fullscreen == true) { Fullscreen = false; }
                                    Move_batton_down.play();
                                }
                            }
                            else if (Fon_Resolution_up.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                Changes = true;
                                if (i_Resolution_num < 4)
                                {

                                    i_Resolution_num = i_Resolution_num + 1;
                                    Refresh_Resolutione_Num();
                                    if (Fullscreen == true) { Fullscreen = false; }
                                    Move_batton_up.play();

                                }
                            }

                           

                            //LenguageButton
                            {
                                if (Lenguage != 0 && ENG_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    Changes = true;
                                    Lenguage_select.play();
                                    Lenguage = 0;
                                }
                                if (Lenguage != 1 && RUS_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    Changes = true;
                                    Lenguage_select.play();
                                    Lenguage = 1;
                                }
                                if (Lenguage != 2 && TUR_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {

                                    Changes = true;
                                    Lenguage_select.play();
                                    Lenguage = 2;
                                }
                                if (Lenguage != 3 && UKR_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    Changes = true;
                                    Lenguage_select.play();
                                    Lenguage = 3;
                                }
                                if (Lenguage != 4 && SPA_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    Changes = true;
                                    Lenguage_select.play();
                                    Lenguage = 4;

                                }
                                if (Lenguage != 5 && POR_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    Changes = true;
                                    Lenguage_select.play();
                                    Lenguage = 5;
                                }
                                if (Lenguage != 6 && POL_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {

                                    Changes = true;
                                    Lenguage_select.play();
                                    Lenguage = 6;

                                }
                                if (Lenguage != 7 && KOR_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {

                                    Changes = true;
                                    Lenguage_select.play();
                                    Lenguage = 7;

                                }
                                if (Lenguage != 8 && JPN_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    Changes = true;
                                    Lenguage_select.play();
                                    Lenguage = 8;
                                }
                                if (Lenguage != 9 && FRA_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    Changes = true;
                                    Lenguage_select.play();
                                    Lenguage = 9;

                                }
                                if (Lenguage != 10 && DEU_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    Changes = true;
                                    Lenguage_select.play();
                                    Lenguage = 10;

                                }
                                if (Lenguage != 11 && CHS_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    Changes = true;
                                    Lenguage_select.play();
                                    Lenguage = 11;
                                }

                            }

                        }
                        else if (Graphics == true)
                        {

                            if (Fon_Vsync.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                while (true)
                                {
                                    Refresh_VSYNC = true;
                                    if (VSYNC == true) { VSYNC = false; Move_batton_down.play(); Text_MaxFPS_num.setFillColor(sf::Color::Black);  break; }
                                    if (VSYNC == false) { VSYNC = true; Move_batton_up.play(); Text_MaxFPS_num.setFillColor(sf::Color::Red); break; }
                                    break;
                                }
                            }
                            
                            if (Fon_MaxFPS_down.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {

                                VSYNC = false; Refresh_VSYNC = true; Refresh_MAXFPS = true; Text_MaxFPS_num.setFillColor(sf::Color::Black);
                                if (MAXFPS > 5) { MAXFPS = MAXFPS - 5; }
                                Refresh_FPS_Num();
                                Move_batton_down.play(); 
                            }
                            else if (Fon_MaxFPS_up.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                               VSYNC = false; Refresh_VSYNC = true; Refresh_MAXFPS = true; Text_MaxFPS_num.setFillColor(sf::Color::Black);
                               if (MAXFPS < 245) { MAXFPS = MAXFPS + 5; }
                               Refresh_FPS_Num();
                               Move_batton_up.play();
                            }
                            

                            if (Fon_FPS_visibility.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                while (true)
                                {
                                    if (FPS_ON == true) { FPS_ON = false;  Move_batton_down.play(); break;}
                                    if (FPS_ON == false) { FPS_ON = true;  Move_batton_up.play(); break;}

                                    break;
                                    
                                }
                            }
                            

                        }

                    }

                    //кнопки сохранений
                    if (isLoad == true)
                    {
                        //кнопки загрузка сохранений
                        if (is_Save_1 == true)
                        {
                            if (LoadGame_Button_1.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {

                                Last_Save_ID = 1; 
                                startGame = true;
                                LoadGame_Sound.play();
                                cout << "Загружаем сейв 1\n";
 
                            }
                        }
                        if (is_Save_2 == true)
                        {
                            if (LoadGame_Button_2.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                Last_Save_ID = 2; 
                                startGame = true;
                                LoadGame_Sound.play();
                                cout << "Загружаем сейв 2\n";
                            }
                        }
                        if (is_Save_3 == true)
                        {
                            if (LoadGame_Button_3.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                Last_Save_ID = 3;
                                startGame = true;
                                LoadGame_Sound.play();
                                cout << "Загружаем сейв 3\n";
                            }
                        }
                        if (is_Save_4 == true)
                        {
                            if (LoadGame_Button_4.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                Last_Save_ID = 4; 
                                startGame = true;
                                LoadGame_Sound.play();
                                cout << "Загружаем сейв 4\n";
                            }
                        }
                        if (is_Save_5 == true)
                        {
                            if (LoadGame_Button_5.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                Last_Save_ID = 5; 
                                startGame = true;
                                LoadGame_Sound.play();
                                cout << "Загружаем сейв 5\n";
                            }
                        }
                        if (is_Save_6 == true)
                        {
                            if (LoadGame_Button_6.getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                Last_Save_ID = 6; 
                                startGame = true;
                                LoadGame_Sound.play();
                                cout << "Загружаем сейв 6\n";
                            }
                        }

                        while (true)
                        {

                            //кнопки удаления сохранений
                            if (is_Save_1 == true)
                            {
                                if (DelGame_Button_1.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    cout << "Удаление сейв 1...\n";
                                    DelGame_Sound.play();
                                    Save_delete = 1;
                                    is_Save_delete = true;
                                    break;
                                }
                            }
                            if (is_Save_2 == true)
                            {
                                if (DelGame_Button_2.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    cout << "Удаление сейв 2...\n";

                                    DelGame_Sound.play();
                                    Save_delete = 2;
                                    is_Save_delete = true;
                                    break;
                                }
                            }
                            if (is_Save_3 == true)
                            {
                                if (DelGame_Button_3.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    cout << "Удаление сейв 3...\n";

                                    DelGame_Sound.play();
                                    Save_delete = 3;
                                    is_Save_delete = true;
                                    break;
                                }
                            }
                            if (is_Save_4 == true)
                            {
                                if (DelGame_Button_4.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    cout << "Удаление сейв 4...\n";

                                    DelGame_Sound.play();
                                    Save_delete = 4;
                                    is_Save_delete = true;
                                    break;
                                }
                            }
                            if (is_Save_5 == true)
                            {
                                if (DelGame_Button_5.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    cout << "Удаление сейв 5...\n";

                                    DelGame_Sound.play();
                                    Save_delete = 5;
                                    is_Save_delete = true;
                                    break;
                                }
                            }
                            if (is_Save_6 == true)
                            {        
                                if (DelGame_Button_6.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    cout << "Удаление сейв 6...\n";

                                    DelGame_Sound.play();
                                    Save_delete = 6;
                                    is_Save_delete = true;
                                    break;
                                }
                            }

                            //кнопки создания новой игры через сохранения
                            if (is_Save_1 == false)
                            {

                                if (NewGame_Button_1.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    NewGame_Sound.play();                                      

                                    press_NewGame = true;                                    
                                    Name_Save = "Saves_1";
                                    Last_Save_ID = 1;
                                    
                                    is_Save_1 = true;
                                    Color_LoadGame_Button();
                                    break;
                                }
                            }
                            if (is_Save_2 == false)
                            {

                                if (NewGame_Button_2.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    NewGame_Sound.play();

                                    press_NewGame = true;
                                    Name_Save = "Saves_2";
                                    Last_Save_ID = 2;

                                    is_Save_2 = true;
                                    Color_LoadGame_Button();
                                    break;
                                }
                            }
                            if (is_Save_3 == false)
                            {

                                if (NewGame_Button_3.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    NewGame_Sound.play();

                                    press_NewGame = true;
                                    Name_Save = "Saves_3";
                                    Last_Save_ID = 3;


                                    is_Save_3 = true;
                                    Color_LoadGame_Button();
                                    break;
                                }
                            }
                            if (is_Save_4 == false)
                            {

                                if (NewGame_Button_4.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    NewGame_Sound.play();

                                    press_NewGame = true;
                                    Name_Save = "Saves_4";
                                    Last_Save_ID = 4;


                                    is_Save_4 = true;
                                    Color_LoadGame_Button();
                                    break;
                                }
                            }
                            if (is_Save_5 == false)
                            {

                                if (NewGame_Button_5.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    NewGame_Sound.play();

                                    press_NewGame = true;
                                    Name_Save = "Saves_5";
                                    Last_Save_ID = 5;


                                    is_Save_5 = true;
                                    Color_LoadGame_Button();
                                    break;
                                }
                            }
                            if (is_Save_6 == false)
                            {

                                if (NewGame_Button_6.getGlobalBounds().contains(mousePos.x, mousePos.y))
                                {
                                    NewGame_Sound.play();

                                    press_NewGame = true;
                                    Name_Save = "Saves_6";
                                    Last_Save_ID = 6;


                                    is_Save_6 = true;
                                    Color_LoadGame_Button();
                                    break;
                                }
                            }

                            break;
                        }
                    }
                }
                else if (is_Save_delete == true)
                {

                    if (Yes_Button.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        DelGame_Yes_Sound.play();
                        delete_save();
                        is_Save_delete = false;
                    }
                    if (No_Button.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        DelGame_No_Sound.play();
                        is_Save_delete = false;
                        Save_delete = 0;
                    }

                }



            }

            if (event.mouseButton.button == sf::Mouse::Right) // Проверяем, что нажата именно ЛКМ
            {

                  
                
            }
        }

        if (event.type == sf::Event::KeyPressed)
        {
            handleKeyPress(event.key.code, true);

        }
        else if (event.type == sf::Event::KeyReleased)
        {
            handleKeyReleased(event.key.code, false);

        }

    }
    void handleKeyPress(sf::Keyboard::Key key, bool isPressed)
    {
        switch (key)
        {

        case sf::Keyboard::R:
            break;
     

        }
    }
    void handleKeyReleased(sf::Keyboard::Key key, bool isReleased)
    {
        switch (key)
        {

        }
    }

    
    


//////////////////////////////     Обработка наведения мышки на что-то    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    int opti_P = 0; int opti_S = 0; int opti_E = 0; int opti_L = 0;

    int FL = 0;          /*Fullscreen*/               int Vsy = 0; /*Vsync*/
    int SFd = 0, SFu = 0; /* Sound FX Down \ Up  */   int Vfps = 0; /*visibility FPS*/
    int Md = 0,  Mu = 0;  // Music Down \ Up
    int Vd = 0,  Vu = 0;  // Vignette Down \ Up
    int Rd = 0,  Ru = 0;  // Resolution Down \ Up
    int FPd = 0, FPu = 0;  // MaxFPS down \ Up

    int SV = 0;  // Save Button
    int CC = 0;  // Cancel Button

    int eng = 0;  // ENG
    int rus = 0;  // RUS
    int tur = 0;  // TUR
    int ukr = 0;  // UKR
    int spa = 0;  // SPA
    int por = 0;  // POR
    int pol = 0;  // POL
    int kor = 0;  // KOR
    int jpn = 0;  // JPN
    int fra = 0;  // FRA
    int deu = 0;  // DEU
    int chs = 0;  // CHS
    int Yb = 0, Nb = 0;
    int sv_1 = 0, sv_11 = 0, sv_2 = 0, sv_22 = 0, sv_3 = 0, sv_33 = 0, sv_4 = 0, sv_44 = 0, sv_5 = 0, sv_55 = 0, sv_6 = 0, sv_66 = 0;  // LoadSave_Button


    void moveEvents(sf::Event event)
    {     

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (is_Save_delete == false)
        {
            // замена цвета текста
            if (PlayButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && opti_P == 0) {
                Move_button_sound();
                Text_PlayButton.setFillColor(sf::Color::White); opti_P = 1;
            }
            else if (!PlayButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && opti_P != 0) {
                Text_PlayButton.setFillColor(sf::Color::Black); opti_P = 0;
            }

            if (isActive_Save == true)
            {
                if (LoadgameButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && opti_L == 0) {
                    Move_button_sound();
                    Text_LoadgameButton.setFillColor(sf::Color::White); opti_L = 1;
                }
                else if (!LoadgameButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && opti_L != 0) {

                    Text_LoadgameButton.setFillColor(sf::Color::Black); opti_L = 0;
                }
            }

            if (SettingsButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && opti_S == 0) {
                Move_button_sound();
                Text_SettingsButton.setFillColor(sf::Color::White); opti_S = 1;
            }
            else if (!SettingsButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && opti_S != 0) {

                Text_SettingsButton.setFillColor(sf::Color::Black); opti_S = 0;
            }

            if (ExitButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && opti_E == 0) {
                Move_button_sound();
                Text_ExitButton.setFillColor(sf::Color::White); opti_E = 1;
            }
            else if (!ExitButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && opti_E != 0) {

                Text_ExitButton.setFillColor(sf::Color::Black); opti_E = 0;
            }


            // Save Button
            if (SaveButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && SV == 0) {
                Text_SaveButton.setFillColor(sf::Color::White);
                SV = 1;
                Move_button_sound();
            }
            else if (!SaveButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && SV != 0) {
                Text_SaveButton.setFillColor(sf::Color::Black);
                SV = 0;
            }
            // Cancel Button
            if (CancelButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && CC == 0) {
                Text_CancelButton.setFillColor(sf::Color::White);
                CC = 1;
                Move_button_sound();
            }
            else if (!CancelButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && CC != 0) {
                Text_CancelButton.setFillColor(sf::Color::Black);
                CC = 0;
            }


            if (isSetting == true)
            {
                if (Global == true) 
                {
                    // Fullscreen
                    if (Fon_Fullscreen.getGlobalBounds().contains(mousePos.x, mousePos.y) && FL == 0) {
                        CH_Sound.play();
                        Text_Fullscreen_num.setFillColor(sf::Color::White);FL = 1;
                    }
                    else if (!Fon_Fullscreen.getGlobalBounds().contains(mousePos.x, mousePos.y) && FL != 0) {
                        Text_Fullscreen_num.setFillColor(sf::Color::Black); FL = 0; }

                    // Up \ Down
                    {

                        // Sound FX Down
                        if (Fon_Sound_FX_down.getGlobalBounds().contains(mousePos.x, mousePos.y) && SFd == 0) {
                            CH_Sound.play();
                            Text_Sound_FX_down.setFillColor(sf::Color::White);
                            SFd = 1;

                        }
                        else if (!Fon_Sound_FX_down.getGlobalBounds().contains(mousePos.x, mousePos.y) && SFd != 0) {
                            Text_Sound_FX_down.setFillColor(sf::Color::Black);
                            SFd = 0;
                        }
                        // Sound FX Up
                        if (Fon_Sound_FX_up.getGlobalBounds().contains(mousePos.x, mousePos.y) && SFu == 0) {
                            CH_Sound.play();
                            Text_Sound_FX_up.setFillColor(sf::Color::White);
                            SFu = 1;
                        }
                        else if (!Fon_Sound_FX_up.getGlobalBounds().contains(mousePos.x, mousePos.y) && SFu != 0) {
                            Text_Sound_FX_up.setFillColor(sf::Color::Black);
                            SFu = 0;
                        }



                        // Music Down
                        if (Fon_Music_down.getGlobalBounds().contains(mousePos.x, mousePos.y) && Md == 0) {
                            CH_Sound.play();
                            Text_Music_down.setFillColor(sf::Color::White);
                            Md = 1;
                        }
                        else if (!Fon_Music_down.getGlobalBounds().contains(mousePos.x, mousePos.y) && Md != 0) {
                            Text_Music_down.setFillColor(sf::Color::Black);
                            Md = 0;
                        }
                        // Music Up
                        if (Fon_Music_up.getGlobalBounds().contains(mousePos.x, mousePos.y) && Mu == 0) {
                            CH_Sound.play();
                            Text_Music_up.setFillColor(sf::Color::White);
                            Mu = 1;
                        }
                        else if (!Fon_Music_up.getGlobalBounds().contains(mousePos.x, mousePos.y) && Mu != 0) {
                            Text_Music_up.setFillColor(sf::Color::Black);
                            Mu = 0;
                        }



                        // Vignette Down
                        if (Fon_Vignette_down.getGlobalBounds().contains(mousePos.x, mousePos.y) && Vd == 0) {
                            CH_Sound.play();
                            Text_Vignette_down.setFillColor(sf::Color::White);
                            Vd = 1;
                        }
                        else if (!Fon_Vignette_down.getGlobalBounds().contains(mousePos.x, mousePos.y) && Vd != 0) {
                            Text_Vignette_down.setFillColor(sf::Color::Black);
                            Vd = 0;
                        }
                        // Vignette Up
                        if (Fon_Vignette_up.getGlobalBounds().contains(mousePos.x, mousePos.y) && Vu == 0) {
                            CH_Sound.play();
                            Text_Vignette_up.setFillColor(sf::Color::White);
                            Vu = 1;
                        }
                        else if (!Fon_Vignette_up.getGlobalBounds().contains(mousePos.x, mousePos.y) && Vu != 0) {
                            Text_Vignette_up.setFillColor(sf::Color::Black);
                            Vu = 0;
                        }



                        // Resolution Down
                        if (Fon_Resolution_down.getGlobalBounds().contains(mousePos.x, mousePos.y) && Rd == 0) {
                            CH_Sound.play();
                            Text_Resolution_down.setFillColor(sf::Color::White);
                            Rd = 1;
                        }
                        else if (!Fon_Resolution_down.getGlobalBounds().contains(mousePos.x, mousePos.y) && Rd != 0) {
                            Text_Resolution_down.setFillColor(sf::Color::Black);
                            Rd = 0;
                        }
                        // Resolution    Up  
                        if (Fon_Resolution_up.getGlobalBounds().contains(mousePos.x, mousePos.y) && Ru == 0) {
                            CH_Sound.play();
                            Text_Resolution_up.setFillColor(sf::Color::White);
                            Ru = 1;
                        }
                        else if (!Fon_Resolution_up.getGlobalBounds().contains(mousePos.x, mousePos.y) && Ru != 0) {
                            Text_Resolution_up.setFillColor(sf::Color::Black);
                            Ru = 0;
                        }

                    }

                    // языки
                    {
                        // ENG
                        if (Lenguage != 0)
                        {
                            if (ENG_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && eng == 0)
                            {
                                ENG_LenguageButton.setOutlineThickness(5);
                                ENG_LenguageButton.setOutlineColor(sf::Color(255, 165, 0)); // Оранжевая рамка
                                eng = 1;
                            }
                            else if (!ENG_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && eng != 0) {
                                ENG_LenguageButton.setOutlineThickness(0);
                                eng = 0;
                            }
                        }
                        else if (eng == 0)
                        {
                            eng = 1;
                            ENG_LenguageButton.setOutlineThickness(5);
                            ENG_LenguageButton.setOutlineColor(sf::Color(255, 165, 0)); // Оранжевая рамка
                        }

                        // RUS
                        if (Lenguage != 1)
                        {
                            if (RUS_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && rus == 0)
                            {
                                RUS_LenguageButton.setOutlineThickness(5);
                                RUS_LenguageButton.setOutlineColor(sf::Color(255, 165, 0)); // Оранжевая рамка
                                rus = 1;
                            }
                            else if (!RUS_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && rus != 0) {
                                RUS_LenguageButton.setOutlineThickness(0);
                                rus = 0;
                            }
                        }
                        else if (rus == 0)
                        {
                            rus = 1;
                            RUS_LenguageButton.setOutlineThickness(5);
                            RUS_LenguageButton.setOutlineColor(sf::Color(255, 165, 0)); // Оранжевая рамка
                        }

                        // TUR
                        if (Lenguage != 2)
                        {
                            if (TUR_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && tur == 0)
                            {
                                TUR_LenguageButton.setOutlineThickness(5);
                                TUR_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                                tur = 1;
                            }
                            else if (!TUR_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && tur != 0) {
                                TUR_LenguageButton.setOutlineThickness(0);
                                tur = 0;
                            }
                        }
                        else if (tur == 0)
                        {
                            tur = 1;
                            TUR_LenguageButton.setOutlineThickness(5);
                            TUR_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                        }

                        // UKR
                        if (Lenguage != 3)
                        {
                            if (UKR_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && ukr == 0)
                            {
                                UKR_LenguageButton.setOutlineThickness(5);
                                UKR_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                                ukr = 1;
                            }
                            else if (!UKR_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && ukr != 0) {
                                UKR_LenguageButton.setOutlineThickness(0);
                                ukr = 0;
                            }
                        }
                        else if (ukr == 0)
                        {
                            ukr = 1;
                            UKR_LenguageButton.setOutlineThickness(5);
                            UKR_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                        }

                        // SPA
                        if (Lenguage != 4)
                        {
                            if (SPA_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && spa == 0)
                            {
                                SPA_LenguageButton.setOutlineThickness(5);
                                SPA_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                                spa = 1;
                            }
                            else if (!SPA_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && spa != 0) {
                                SPA_LenguageButton.setOutlineThickness(0);
                                spa = 0;
                            }
                        }
                        else if (spa == 0)
                        {
                            spa = 1;
                            SPA_LenguageButton.setOutlineThickness(5);
                            SPA_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                        }

                        // POR
                        if (Lenguage != 5)
                        {
                            if (POR_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && por == 0)
                            {
                                POR_LenguageButton.setOutlineThickness(5);
                                POR_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                                por = 1;
                            }
                            else if (!POR_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && por != 0) {
                                POR_LenguageButton.setOutlineThickness(0);
                                por = 0;
                            }
                        }
                        else if (por == 0)
                        {
                            por = 1;
                            POR_LenguageButton.setOutlineThickness(5);
                            POR_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                        }

                        // POL
                        if (Lenguage != 6)
                        {
                            if (POL_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && pol == 0)
                            {
                                POL_LenguageButton.setOutlineThickness(5);
                                POL_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                                pol = 1;
                            }
                            else if (!POL_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && pol != 0) {
                                POL_LenguageButton.setOutlineThickness(0);
                                pol = 0;
                            }
                        }
                        else if (pol == 0)
                        {
                            pol = 1;
                            POL_LenguageButton.setOutlineThickness(5);
                            POL_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                        }

                        // KOR
                        if (Lenguage != 7)
                        {
                            if (KOR_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && kor == 0)
                            {
                                KOR_LenguageButton.setOutlineThickness(5);
                                KOR_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                                kor = 1;
                            }
                            else if (!KOR_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && kor != 0) {
                                KOR_LenguageButton.setOutlineThickness(0);
                                kor = 0;
                            }
                        }
                        else if (kor == 0)
                        {
                            kor = 1;
                            KOR_LenguageButton.setOutlineThickness(5);
                            KOR_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                        }

                        // JPN
                        if (Lenguage != 8)
                        {
                            if (JPN_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && jpn == 0)
                            {
                                JPN_LenguageButton.setOutlineThickness(5);
                                JPN_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                                jpn = 1;
                            }
                            else if (!JPN_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && jpn != 0) {
                                JPN_LenguageButton.setOutlineThickness(0);
                                jpn = 0;
                            }
                        }
                        else if (jpn == 0)
                        {
                            jpn = 1;
                            JPN_LenguageButton.setOutlineThickness(5);
                            JPN_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                        }

                        // FRA
                        if (Lenguage != 9)
                        {
                            if (FRA_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && fra == 0)
                            {
                                FRA_LenguageButton.setOutlineThickness(5);
                                FRA_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                                fra = 1;
                            }
                            else if (!FRA_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && fra != 0) {
                                FRA_LenguageButton.setOutlineThickness(0);
                                fra = 0;
                            }
                        }
                        else if (fra == 0)
                        {
                            fra = 1;
                            FRA_LenguageButton.setOutlineThickness(5);
                            FRA_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                        }

                        // DEU
                        if (Lenguage != 10)
                        {
                            if (DEU_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && deu == 0)
                            {
                                DEU_LenguageButton.setOutlineThickness(5);
                                DEU_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                                deu = 1;
                            }
                            else if (!DEU_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && deu != 0) {
                                DEU_LenguageButton.setOutlineThickness(0);
                                deu = 0;
                            }
                        }
                        else if (deu == 0)
                        {
                            deu = 1;
                            DEU_LenguageButton.setOutlineThickness(5);
                            DEU_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                        }

                        // CHS (упрощённый китайский)
                        if (Lenguage != 11)
                        {
                            if (CHS_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && chs == 0)
                            {
                                CHS_LenguageButton.setOutlineThickness(5);
                                CHS_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                                chs = 1;
                            }
                            else if (!CHS_LenguageButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && chs != 0) {
                                CHS_LenguageButton.setOutlineThickness(0);
                                chs = 0;
                            }
                        }
                        else if (chs == 0)
                        {
                            chs = 1;
                            CHS_LenguageButton.setOutlineThickness(5);
                            CHS_LenguageButton.setOutlineColor(sf::Color(255, 165, 0));
                        }

                    }
                }
                else if (Graphics == true)
                {
                    // Vsync
                    if (Fon_Vsync.getGlobalBounds().contains(mousePos.x, mousePos.y) && Vsy == 0) {
                        CH_Sound.play();
                        Text_Vsync_num.setFillColor(sf::Color::White); Vsy = 1;
                    }
                    else if (!Fon_Vsync.getGlobalBounds().contains(mousePos.x, mousePos.y) && Vsy != 0) 
                    { Text_Vsync_num.setFillColor(sf::Color::Black); Vsy = 0; }

                    
                        /// Sound FX Down
                    if (Fon_MaxFPS_down.getGlobalBounds().contains(mousePos.x, mousePos.y) && FPd == 0) {
                        CH_Sound.play();
                        Text_MaxFPS_down.setFillColor(sf::Color::White);
                        FPd = 1;

                    }
                    else if (!Fon_MaxFPS_down.getGlobalBounds().contains(mousePos.x, mousePos.y) && FPd != 0) {
                        Text_MaxFPS_down.setFillColor(sf::Color::Black);
                        FPd = 0;
                    }
                    // Sound FX Up
                    if (Fon_MaxFPS_up.getGlobalBounds().contains(mousePos.x, mousePos.y) && FPu == 0) {
                        CH_Sound.play();
                        Text_MaxFPS_up.setFillColor(sf::Color::White);
                        FPu = 1;
                    }
                    else if (!Fon_MaxFPS_up.getGlobalBounds().contains(mousePos.x, mousePos.y) && FPu != 0) {
                        Text_MaxFPS_up.setFillColor(sf::Color::Black);
                        FPu = 0;
                    }


                    // FPS_visibility
                    if (Fon_FPS_visibility.getGlobalBounds().contains(mousePos.x, mousePos.y) && Vfps == 0) {
                        CH_Sound.play();
                        Text_FPS_visibility_num.setFillColor(sf::Color::White); Vfps = 1;
                    }
                    else if (!Fon_FPS_visibility.getGlobalBounds().contains(mousePos.x, mousePos.y) && Vfps != 0)
                    {
                        Text_FPS_visibility_num.setFillColor(sf::Color::Black); Vfps = 0;
                    }
                }

            }


            if (isLoad == true)
            {

                if (is_Save_1 == true)
                {
                    if (LoadGame_Button_1.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_1 == 0) {
                        CH_Sound.play();
                        Text_LoadGame_Button_1.setFillColor(sf::Color::White); sv_1 = 1;
                    }
                    else if (!LoadGame_Button_1.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_1 != 0) {

                        Text_LoadGame_Button_1.setFillColor(sf::Color::Black); sv_1 = 0;
                    }

                    if (DelGame_Button_1.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_11 == 0) {
                        CH_Sound.play();
                        Text_DelGame_Button_1.setFillColor(sf::Color::White); sv_11 = 1;
                    }
                    else if (!DelGame_Button_1.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_11 != 0) {

                        Text_DelGame_Button_1.setFillColor(sf::Color::Black); sv_11 = 0;
                    }         
                }
                else
                {
                    if (NewGame_Button_1.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_1 == 0) {
                        CH_Sound.play();
                        Text_NewGame_Button_1.setFillColor(sf::Color::White); sv_1 = 1;
                    }
                    else if (!NewGame_Button_1.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_1 != 0) {

                        Text_NewGame_Button_1.setFillColor(sf::Color::Black); sv_1 = 0;
                    }

                }

                if (is_Save_2 == true)
                {
                    if (LoadGame_Button_2.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_2 == 0) {
                        CH_Sound.play();
                        Text_LoadGame_Button_2.setFillColor(sf::Color::White); sv_2 = 1;
                    }
                    else if (!LoadGame_Button_2.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_2 != 0) {

                        Text_LoadGame_Button_2.setFillColor(sf::Color::Black); sv_2 = 0;
                    }

                    if (DelGame_Button_2.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_22 == 0) {
                        CH_Sound.play();
                        Text_DelGame_Button_2.setFillColor(sf::Color::White); sv_22 = 1;
                    }
                    else if (!DelGame_Button_2.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_22 != 0) {

                        Text_DelGame_Button_2.setFillColor(sf::Color::Black); sv_22 = 0;
                    }

                }
                else
                {
                    if (NewGame_Button_2.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_2 == 0) {
                        CH_Sound.play();
                        Text_NewGame_Button_2.setFillColor(sf::Color::White); sv_2 = 1;
                    }
                    else if (!NewGame_Button_2.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_2 != 0) {

                        Text_NewGame_Button_2.setFillColor(sf::Color::Black); sv_2 = 0;
                    }

                }

                if (is_Save_3 == true)
                {
                    if (LoadGame_Button_3.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_3 == 0) {
                        CH_Sound.play();
                        Text_LoadGame_Button_3.setFillColor(sf::Color::White); sv_3 = 1;
                    }
                    else if (!LoadGame_Button_3.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_3 != 0) {

                        Text_LoadGame_Button_3.setFillColor(sf::Color::Black); sv_3 = 0;
                    }

                    if (DelGame_Button_3.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_33 == 0) {
                        CH_Sound.play();
                        Text_DelGame_Button_3.setFillColor(sf::Color::White); sv_33 = 1;
                    }
                    else if (!DelGame_Button_3.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_33 != 0) {

                        Text_DelGame_Button_3.setFillColor(sf::Color::Black); sv_33 = 0;
                    }

                }
                else
                {
                    if (NewGame_Button_3.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_3 == 0) {
                        CH_Sound.play();
                        Text_NewGame_Button_3.setFillColor(sf::Color::White); sv_3 = 1;
                    }
                    else if (!NewGame_Button_3.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_3 != 0) {

                        Text_NewGame_Button_3.setFillColor(sf::Color::Black); sv_3 = 0;
                    }

                }

                if (is_Save_4 == true)
                {
                    if (LoadGame_Button_4.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_4 == 0) {
                        CH_Sound.play();
                        Text_LoadGame_Button_4.setFillColor(sf::Color::White); sv_4 = 1;
                    }
                    else if (!LoadGame_Button_4.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_4 != 0) {

                        Text_LoadGame_Button_4.setFillColor(sf::Color::Black); sv_4 = 0;
                    }

                    if (DelGame_Button_4.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_44 == 0) {
                        CH_Sound.play();
                        Text_DelGame_Button_4.setFillColor(sf::Color::White); sv_44 = 1;
                    }
                    else if (!DelGame_Button_4.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_44 != 0) {

                        Text_DelGame_Button_4.setFillColor(sf::Color::Black); sv_44 = 0;
                    }

                }
                else
                {
                    if (NewGame_Button_4.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_4 == 0) {
                        CH_Sound.play();
                        Text_NewGame_Button_4.setFillColor(sf::Color::White); sv_4 = 1;
                    }
                    else if (!NewGame_Button_4.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_4 != 0) {

                        Text_NewGame_Button_4.setFillColor(sf::Color::Black); sv_4 = 0;
                    }

                }

                if (is_Save_5 == true)
                {
                    if (LoadGame_Button_5.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_5 == 0) {
                        CH_Sound.play();
                        Text_LoadGame_Button_5.setFillColor(sf::Color::White); sv_5 = 1;
                    }
                    else if (!LoadGame_Button_5.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_5 != 0) {

                        Text_LoadGame_Button_5.setFillColor(sf::Color::Black); sv_5 = 0;
                    }

                    if (DelGame_Button_5.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_55 == 0) {
                        CH_Sound.play();
                        Text_DelGame_Button_5.setFillColor(sf::Color::White); sv_55 = 1;
                    }
                    else if (!DelGame_Button_5.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_55 != 0) {

                        Text_DelGame_Button_5.setFillColor(sf::Color::Black); sv_55 = 0;
                    }

                }
                else
                {
                    if (NewGame_Button_5.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_5 == 0) {
                        CH_Sound.play();
                        Text_NewGame_Button_5.setFillColor(sf::Color::White); sv_5 = 1;
                    }
                    else if (!NewGame_Button_5.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_5 != 0) {

                        Text_NewGame_Button_5.setFillColor(sf::Color::Black); sv_5 = 0;
                    }

                }

                if (is_Save_6 == true)
                {
                    if (LoadGame_Button_6.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_6 == 0) {
                        CH_Sound.play();
                        Text_LoadGame_Button_6.setFillColor(sf::Color::White); sv_6 = 1;
                    }
                    else if (!LoadGame_Button_6.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_6 != 0) {

                        Text_LoadGame_Button_6.setFillColor(sf::Color::Black); sv_6 = 0;
                    }

                    if (DelGame_Button_6.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_66 == 0) {
                        CH_Sound.play();
                        Text_DelGame_Button_6.setFillColor(sf::Color::White); sv_66 = 1;
                    }
                    else if (!DelGame_Button_6.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_66 != 0) {

                        Text_DelGame_Button_6.setFillColor(sf::Color::Black); sv_66 = 0;
                    }

                }
                else
                {
                    if (NewGame_Button_6.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_6 == 0) {
                        CH_Sound.play();
                        Text_NewGame_Button_6.setFillColor(sf::Color::White); sv_6 = 1;
                    }
                    else if (!NewGame_Button_6.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_6 != 0) {

                        Text_NewGame_Button_6.setFillColor(sf::Color::Black); sv_6 = 0;
                    }

                }


            }
        }
        //если происходит процесс удаления
        if(is_Save_delete == true)
        {
           
            if (Yes_Button.getGlobalBounds().contains(mousePos.x, mousePos.y) && Yb == 0) {
                sv_1 = 1; sv_2 = 1; sv_3 = 1; sv_4 = 1; sv_5 = 1; sv_6 = 1; 
                CH_Sound.play();
                Text_Yes_Button.setFillColor(sf::Color::White); Yb = 1;
            }
            else if (!Yes_Button.getGlobalBounds().contains(mousePos.x, mousePos.y) && Yb != 0) {
                Text_Yes_Button.setFillColor(sf::Color::Black); Yb = 0;
            }
            
            if (No_Button.getGlobalBounds().contains(mousePos.x, mousePos.y) && Nb == 0) {
                CH_Sound.play();
                Text_No_Button.setFillColor(sf::Color::White); Nb = 1;
            }
            else if (!No_Button.getGlobalBounds().contains(mousePos.x, mousePos.y) && Nb != 0) {
                Text_No_Button.setFillColor(sf::Color::Black); Nb = 0;
            }
        

            if (!NewGame_Button_1.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_1 != 0)
            {
                Text_NewGame_Button_1.setFillColor(sf::Color::Black); sv_1 = 0;
            }
            if (!NewGame_Button_2.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_2 != 0) {

                Text_NewGame_Button_2.setFillColor(sf::Color::Black); sv_2 = 0;
            }
            if (!NewGame_Button_3.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_3 != 0) {

                Text_NewGame_Button_3.setFillColor(sf::Color::Black); sv_3 = 0;
            }
            if (!NewGame_Button_4.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_4 != 0) {

                Text_NewGame_Button_4.setFillColor(sf::Color::Black); sv_4 = 0;
            }
            if (!NewGame_Button_5.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_5 != 0) {

                Text_NewGame_Button_5.setFillColor(sf::Color::Black); sv_5 = 0;
            }
            if (!NewGame_Button_6.getGlobalBounds().contains(mousePos.x, mousePos.y) && sv_6 != 0) {

                Text_NewGame_Button_6.setFillColor(sf::Color::Black); sv_6 = 0;
            }
        }
             
    }
    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:
    sf::RenderWindow& window; // Ссылка на окно

    //переменные обьектов
    sf::RectangleShape PlayButton;        sf::Texture PlayButton_Texture;        sf::Text Text_PlayButton;
    sf::RectangleShape LoadgameButton;    sf::Texture LoadgameButton_Texture;    sf::Text Text_LoadgameButton;
    sf::RectangleShape SettingsButton;    sf::Texture SettingsButton_Texture;    sf::Text Text_SettingsButton;
    sf::RectangleShape ExitButton;        sf::Texture ExitButton_Texture;        sf::Text Text_ExitButton;
     
    sf::RectangleShape SettingsPanel;     sf::Texture SettingsPanel_Texture;     sf::Text Text_SETTINGS;
                                                                                 
                                                                                 
    sf::RectangleShape SettingsPanel_Global;                                     
    sf::RectangleShape SettingsPanel_Graphics;   
    sf::RectangleShape SettingsPanel_Controls;
                                                                                 
    sf::Texture SettingsPanel_Global_Texture;                                    sf::Text Text_Changes;
    sf::Texture SettingsPanel_Graphics_Texture;
    sf::Texture SettingsPanel_Controls_Texture;
                                                     sf::Text Text_Sound_FX;     sf::RectangleShape Fon_Sound_FX_down;          sf::Text Text_Sound_FX_down;
                                                                                 sf::RectangleShape Fon_Sound_FX_up;            sf::Text Text_Sound_FX_up;
                                                                                 sf::RectangleShape Fon_Sound_FX_num;           sf::Text Text_Sound_FX_num;

                                                     sf::Text Text_Music;        sf::RectangleShape Fon_Music_down;             sf::Text Text_Music_down;
                                                                                 sf::RectangleShape Fon_Music_up;               sf::Text Text_Music_up;
                                                                                 sf::RectangleShape Fon_Music_num;              sf::Text Text_Music_num;
                                                                                   
                                                     sf::Text Text_Vignette;     sf::RectangleShape Fon_Vignette_down;          sf::Text Text_Vignette_down;
                                                                                 sf::RectangleShape Fon_Vignette_up;            sf::Text Text_Vignette_up;
                                                                                 sf::RectangleShape Fon_Vignette_num;           sf::Text Text_Vignette_num;

                                                     sf::Text Text_Fullscreen;   sf::RectangleShape Fon_Fullscreen;             sf::Text Text_Fullscreen_num;
                                                                                 
                                                     sf::Text Text_Resolution;   sf::RectangleShape Fon_Resolution_down;        sf::Text Text_Resolution_down;
                                                                                 sf::RectangleShape Fon_Resolution_up;          sf::Text Text_Resolution_up;
                                                                                 sf::RectangleShape Fon_Resolution_num;         sf::Text Text_Resolution_num;
                                                                                                                                sf::Text Text_Your_Resolution;

                                          sf::Texture Button_S_Texture;          sf::RectangleShape SaveButton;                 sf::Text Text_SaveButton;
                                          sf::Texture Button_X_Texture;          sf::RectangleShape CancelButton;               sf::Text Text_CancelButton;
                                          sf::Texture Button_NUM_Texture;
                                                                                
                                          sf::Texture ENG_LenguageButton_Texture; sf::RectangleShape ENG_LenguageButton;
                                          sf::Texture RUS_LenguageButton_Texture; sf::RectangleShape RUS_LenguageButton;
                                          sf::Texture TUR_LenguageButton_Texture; sf::RectangleShape TUR_LenguageButton;
                                          sf::Texture UKR_LenguageButton_Texture; sf::RectangleShape UKR_LenguageButton;
                                          sf::Texture SPA_LenguageButton_Texture; sf::RectangleShape SPA_LenguageButton;
                                          sf::Texture POR_LenguageButton_Texture; sf::RectangleShape POR_LenguageButton;
                                          sf::Texture POL_LenguageButton_Texture; sf::RectangleShape POL_LenguageButton;
                                          sf::Texture KOR_LenguageButton_Texture; sf::RectangleShape KOR_LenguageButton;
                                          sf::Texture JPN_LenguageButton_Texture; sf::RectangleShape JPN_LenguageButton;
                                          sf::Texture FRA_LenguageButton_Texture; sf::RectangleShape FRA_LenguageButton;
                                          sf::Texture DEU_LenguageButton_Texture; sf::RectangleShape DEU_LenguageButton;
                                          sf::Texture CHS_LenguageButton_Texture; sf::RectangleShape CHS_LenguageButton;

                                          

              sf::Text Text_Vsync;        sf::RectangleShape Fon_Vsync;       sf::Text Text_Vsync_num;

              sf::Text Text_MaxFPS;       sf::RectangleShape Fon_MaxFPS_down;            sf::Text Text_MaxFPS_down;
                                          sf::RectangleShape Fon_MaxFPS_up;               sf::Text Text_MaxFPS_up;
                                          sf::RectangleShape Fon_MaxFPS_num;              

              sf::Text Text_FPS_visibility;        sf::RectangleShape Fon_FPS_visibility;       sf::Text Text_FPS_visibility_num;
                                          








  sf::RectangleShape SavePanel;     sf::Texture SavePanel_Texture;             
                  sf::Text Text_SAVE_1;  sf::RectangleShape LoadGame_Button_1; sf::Text Text_LoadGame_Button_1; 
                  sf::Text Text_SAVE_2;  sf::RectangleShape LoadGame_Button_2; sf::Text Text_LoadGame_Button_2;
                  sf::Text Text_SAVE_3;  sf::RectangleShape LoadGame_Button_3; sf::Text Text_LoadGame_Button_3;
                  sf::Text Text_SAVE_4;  sf::RectangleShape LoadGame_Button_4; sf::Text Text_LoadGame_Button_4;
                  sf::Text Text_SAVE_5;  sf::RectangleShape LoadGame_Button_5; sf::Text Text_LoadGame_Button_5;
                  sf::Text Text_SAVE_6;  sf::RectangleShape LoadGame_Button_6; sf::Text Text_LoadGame_Button_6;

                                         sf::RectangleShape NewGame_Button_1;  sf::Text Text_NewGame_Button_1;
                                         sf::RectangleShape NewGame_Button_2;  sf::Text Text_NewGame_Button_2;
                                         sf::RectangleShape NewGame_Button_3;  sf::Text Text_NewGame_Button_3;
                                         sf::RectangleShape NewGame_Button_4;  sf::Text Text_NewGame_Button_4;
                                         sf::RectangleShape NewGame_Button_5;  sf::Text Text_NewGame_Button_5;
                                         sf::RectangleShape NewGame_Button_6;  sf::Text Text_NewGame_Button_6;

                                         sf::RectangleShape DelGame_Button_1;  sf::Text Text_DelGame_Button_1;
                                         sf::RectangleShape DelGame_Button_2;  sf::Text Text_DelGame_Button_2;
                                         sf::RectangleShape DelGame_Button_3;  sf::Text Text_DelGame_Button_3;
                                         sf::RectangleShape DelGame_Button_4;  sf::Text Text_DelGame_Button_4;
                                         sf::RectangleShape DelGame_Button_5;  sf::Text Text_DelGame_Button_5;
                                         sf::RectangleShape DelGame_Button_6;  sf::Text Text_DelGame_Button_6;
              sf::RectangleShape ConfirmPanel;     sf::Texture ConfirmPanel_Texture;
                                                      sf::Text Text_DeleteSave;
                      sf::RectangleShape Yes_Button;  sf::Text Text_Yes_Button;
                      sf::RectangleShape No_Button;   sf::Text Text_No_Button;


 
    sf::RectangleShape Fon_menu; sf::Texture Fon_menu_Texture;
    sf::RectangleShape Fon_menu_foreground; sf::Texture Fon_menu_Foreground;
    

    sf::RectangleShape Clouds_1; vector<sf::Texture> MASIV_Clouds_Texture;
    sf::RectangleShape Clouds_2;    
    sf::RectangleShape Clouds_3;  
    sf::RectangleShape Clouds_4;
    

         sf::Texture Vignette_Texture;

    sf::Texture EmptyTexture;






    
    
//////////////////////////////   динамичные лёгкие параметры настроек  ////////////////////////
    void UI_dynamic_text()
    {

        Refresh_Sound_FX_Num(); // Sound_FX
        Refresh_Music_Num(); // Music
        Refresh_Resolutione_Num(); //  Resolution
        Refresh_Vignette_Num(); //  Vignette
        

        Refresh_FPS_Num();
     

    }    
    // функции обновления отображения чисел в настройках
    void Refresh_Sound_FX_Num()
    {
        string s_Sound_FX_num = to_string(i_Sound_FX_num);
        Text_Sound_FX_num.setString(s_Sound_FX_num);

        Text_Sound_FX_num.setOrigin(Text_Sound_FX_num.getLocalBounds().width / 2, Text_Sound_FX_num.getLocalBounds().height / 2);
        sf::FloatRect fonBounds = Fon_Sound_FX_num.getGlobalBounds();
        Text_Sound_FX_num.setPosition(fonBounds.left + fonBounds.width / 2.2, fonBounds.top + fonBounds.height / 2);
    
    }
    void Refresh_Music_Num()
    {
        string s_Music_num = to_string(i_Music_num);
        Text_Music_num.setString(s_Music_num);

        Text_Music_num.setOrigin(Text_Music_num.getLocalBounds().width / 2, Text_Music_num.getLocalBounds().height / 2);
        sf::FloatRect fonBounds = Fon_Music_num.getGlobalBounds();
        Text_Music_num.setPosition(fonBounds.left + fonBounds.width / 2.2, fonBounds.top + fonBounds.height / 2);
    
    }
    void Refresh_Vignette_Num()
    {
        string s_Vignette_num = to_string(i_Vignette_num);
        Text_Vignette_num.setString(s_Vignette_num);

        Text_Vignette_num.setOrigin(Text_Vignette_num.getLocalBounds().width / 2, Text_Vignette_num.getLocalBounds().height / 2);
        sf::FloatRect fonBounds = Fon_Vignette_num.getGlobalBounds();
        Text_Vignette_num.setPosition(fonBounds.left + fonBounds.width / 2.2, fonBounds.top + fonBounds.height / 2);
    }
    void Vignette_setting()
    {
        int vignetteStrength = static_cast<int>(i_Vignette_num * 2.55); // Пересчёт в 0-255
        Vignette.setFillColor(sf::Color(0, 0, 0, vignetteStrength));
    }

    void Refresh_Resolutione_Num()
    {
        if (i_Resolution_num == 1)
        {
            resolutionX = 800; resolutionY = 600;
            Text_Resolution_num.setString("800 x 600");
        }
        if (i_Resolution_num == 2)
        {
            resolutionX = 1280; resolutionY = 720;
            Text_Resolution_num.setString("1280 x 720");
        }
        if (i_Resolution_num == 3)
        {
            resolutionX = 1366; resolutionY = 768;
            Text_Resolution_num.setString("1366 x 768");
        }
        if (i_Resolution_num == 4)
        {
            resolutionX = 1920; resolutionY = 1080;
            Text_Resolution_num.setString("1920 x 1080");
        }
        if (i_Resolution_num == 0)
        {
            sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode(); sf::Vector2u Desktop_Size(desktopMode.width, desktopMode.height);
            string s_Resolution = to_string(Desktop_Size.x) + " x " + to_string(Desktop_Size.y);
            resolutionX = Desktop_Size.x; resolutionY = Desktop_Size.y;
            Text_Resolution_num.setString(s_Resolution);
        }

        Text_Resolution_num.setOrigin(Text_Resolution_num.getLocalBounds().width / 2, Text_Resolution_num.getLocalBounds().height / 2);
        sf::FloatRect fonBounds = Fon_Resolution_num.getGlobalBounds();
        Text_Resolution_num.setPosition(fonBounds.left + fonBounds.width / 2, fonBounds.top + fonBounds.height / 2);

    }

    
    void Refresh_FPS_Num()
    {      
        string s_MAXFPS = to_string(MAXFPS);
        Text_MaxFPS_num.setString(s_MAXFPS);

        Text_MaxFPS_num.setOrigin(Text_MaxFPS_num.getLocalBounds().width / 2, Text_MaxFPS_num.getLocalBounds().height / 2);
        sf::FloatRect fonBounds = Fon_MaxFPS_num.getGlobalBounds();
        Text_MaxFPS_num.setPosition(fonBounds.left + fonBounds.width / 2.2, fonBounds.top + fonBounds.height / 2);
    }

    
    

///////////////////////////////   загрузка параметров обьектов \ ЯЗЫК  ////////////////////////

    //задаётся текст   ЯЗЫК
    void UI_load_text()
    {
        load_text_no_translate();
        // Английский (ENG)
        if (Lenguage == 0)
        {
            // Меню
            {
                Text_PlayButton.setString(L"Play");
                Text_LoadgameButton.setString(L"Load Game");
                Text_SettingsButton.setString(L"Settings");
                Text_ExitButton.setString(L"Exit");

                Text_SETTINGS.setString(L"SETTINGS");
                Text_Resolution.setString(L"Resolution");
                Text_Sound_FX.setString(L"Sound FX");
                Text_Music.setString(L"Music");
                Text_Vignette.setString(L"Vignette");
                Text_Fullscreen.setString(L"Fullscreen");

                Text_Your_Resolution.setString(L"your resolution");
                Text_Changes.setString(L"If global changes were made, restart the game");

                Text_SaveButton.setString(L"Save");
                Text_CancelButton.setString(L"Cancel");


                Text_SAVE_1.setString(L"SAVE 1"); Text_LoadGame_Button_1.setString(L"  Load  ");
                Text_SAVE_2.setString(L"SAVE 2"); Text_LoadGame_Button_2.setString(L"  Load  ");
                Text_SAVE_3.setString(L"SAVE 3"); Text_LoadGame_Button_3.setString(L"  Load  ");
                Text_SAVE_4.setString(L"SAVE 4"); Text_LoadGame_Button_4.setString(L"  Load  ");
                Text_SAVE_5.setString(L"SAVE 5"); Text_LoadGame_Button_5.setString(L"  Load  ");
                Text_SAVE_6.setString(L"SAVE 6"); Text_LoadGame_Button_6.setString(L"  Load  ");

                Text_NewGame_Button_1.setString(L"New Game");   Text_DeleteSave.setString(L"Delete Save?");
                Text_NewGame_Button_2.setString(L"New Game");   Text_Yes_Button.setString(L"  Yes  ");
                Text_NewGame_Button_3.setString(L"New Game");   Text_No_Button.setString(L"   No   ");
                Text_NewGame_Button_4.setString(L"New Game");
                Text_NewGame_Button_5.setString(L"New Game");
                Text_NewGame_Button_6.setString(L"New Game");



                Text_Vsync.setString(L"Vsync    ");
                Text_MaxFPS.setString(L"Max FPS");
            }

            // Game_UI
            {
                Text_PAUSE.setString(L"PAUSE");
                Text_SaveGame.setString(L" Save Game ");
                Text_Exit.setString(L"Exit To Menu");

                Text_FPS_visibility.setString(L"Vis. FPS");
            }
            
            
        }
        // Русский (RUS)
        if (Lenguage == 1)
        {
            Text_PlayButton.setString(L"Играть");
            Text_SettingsButton.setString(L"Настройки");
            Text_ExitButton.setString(L"Выход");

            Text_SETTINGS.setString(L"НАСТРОЙКИ");
            Text_Resolution.setString(L"Разрешение");
            Text_Sound_FX.setString(L"Эффекты");
            Text_Music.setString(L"Музыка");
            Text_Vignette.setString(L"Виньетка");
            Text_Fullscreen.setString(L"Полноэкранный");

            Text_Your_Resolution.setString(L"ваше разрешение");
            Text_Changes.setString(L"Если были внесены глобальные изменения, перезапустите игру");

            Text_SaveButton.setString(L"Сохранить");
            Text_CancelButton.setString(L"Отменить");
        }
        // Турецкий (TUR)
        if (Lenguage == 2)
        {
            Text_PlayButton.setString(L"Oyna");
            Text_SettingsButton.setString(L"Ayarlar");
            Text_ExitButton.setString(L"Çıkış");

            Text_SETTINGS.setString(L"AYARLAR");
            Text_Resolution.setString(L"Çözünürlük");
            Text_Sound_FX.setString(L"Etkileri");
            Text_Music.setString(L"Müzik");
            Text_Vignette.setString(L"Vinyet");
            Text_Fullscreen.setString(L"Tam ekran");

            Text_Your_Resolution.setString(L"Çözünürlüğünüz");
            Text_Changes.setString(L"Eğer global değişiklikler yapıldıysa oyunu yeniden başlatın");
            
            Text_SaveButton.setString(L"Kaydet");
            Text_CancelButton.setString(L"İptal");
        }
        // Украинский (UKR)
        if (Lenguage == 3)
        {
            Text_PlayButton.setString(L"Грати");
            Text_SettingsButton.setString(L"Налаштування");
            Text_ExitButton.setString(L"Вихід");

            Text_SETTINGS.setString(L"НАЛАШТУВАННЯ");
            Text_Resolution.setString(L"Роздільна здатність");
            Text_Sound_FX.setString(L"Ефекти");
            Text_Music.setString(L"Музика");
            Text_Vignette.setString(L"Віньєтка");
            Text_Fullscreen.setString(L"Повний екран");

            Text_Your_Resolution.setString(L"Ваша роздільна здатність");
            Text_Changes.setString(L"Якщо були внесені глобальні зміни, перезапустіть гру");
            
            Text_SaveButton.setString(L"Зберегти");
            Text_CancelButton.setString(L"Скасувати");
        }
        // Испанский (SPA)
        if (Lenguage == 4)
        {
            Text_PlayButton.setString(L"Jugar");
            Text_SettingsButton.setString(L"Configuración");
            Text_ExitButton.setString(L"Salir");

            Text_SETTINGS.setString(L"CONFIGURACIÓN");
            Text_Resolution.setString(L"Resolución");
            Text_Sound_FX.setString(L"Efectos");
            Text_Music.setString(L"Música");
            Text_Vignette.setString(L"Viñeta");
            Text_Fullscreen.setString(L"Pantalla completa");

            Text_Your_Resolution.setString(L"Tu resolución");
            Text_Changes.setString(L"Si se realizaron cambios globales, reinicie el juego");

            Text_SaveButton.setString(L"Guardar");
            Text_CancelButton.setString(L"Cancelar");
        }
        // Португальский (POR) бразилия
        if (Lenguage == 5)
        {
            Text_PlayButton.setString(L"Jogar");
            Text_SettingsButton.setString(L"Configurações");
            Text_ExitButton.setString(L"Sair");

            Text_SETTINGS.setString(L"CONFIGURAÇÕES");
            Text_Resolution.setString(L"Resolução");
            Text_Sound_FX.setString(L"Efeitos");
            Text_Music.setString(L"Música");
            Text_Vignette.setString(L"Vinheta");
            Text_Fullscreen.setString(L"Tela cheia");

            Text_Your_Resolution.setString(L"Sua resolução");
            Text_Changes.setString(L"Se alterações globais foram feitas, reinicie o jogo");

            Text_SaveButton.setString(L"Salvar");
            Text_CancelButton.setString(L"Cancelar");
        }
        // Польский (POL)
        if (Lenguage == 6)
        {
            Text_PlayButton.setString(L"Graj");
            Text_SettingsButton.setString(L"Ustawienia");
            Text_ExitButton.setString(L"Wyjdź");

            Text_SETTINGS.setString(L"USTAWIENIA");
            Text_Resolution.setString(L"Rozdzielczość");
            Text_Sound_FX.setString(L"Efekty");
            Text_Music.setString(L"Muzyka");
            Text_Vignette.setString(L"Winetka");
            Text_Fullscreen.setString(L"Pełny ekran");

            Text_Your_Resolution.setString(L"Twoja rozdzielczość");
            Text_Changes.setString(L"Jeśli zostały wprowadzone zmiany globalne, uruchom grę ponownie");

            Text_SaveButton.setString(L"Zapisz");
            Text_CancelButton.setString(L"Anuluj");
        }
        // Корейский (KOR)
        if (Lenguage == 7)
        {
            Text_PlayButton.setString(L"  시작  ");
            Text_SettingsButton.setString(L"   설정   ");
            Text_ExitButton.setString(L"  종료  ");

            Text_SETTINGS.setString(L"    설정    ");

            Text_Resolution.setString(L"해상도       ");
            Text_Sound_FX.setString(L"효과      ");
            Text_Music.setString(L"음악    ");
            Text_Vignette.setString(L"비네트    ");
            Text_Fullscreen.setString(L"전체 화면    ");

            Text_Your_Resolution.setString(L"   현재 해상도   ");
            Text_Changes.setString(L"글로벌 변경 사항이 적용된 경우 게임을 다시 시작하세요");

            Text_SaveButton.setString(L"  저장  ");
            Text_CancelButton.setString(L"  취소  ");
        }
        // Японский (JPN)
        if (Lenguage == 8)
        {
            Text_PlayButton.setString(L"プレイ");
            Text_SettingsButton.setString(L"    設定    ");
            Text_ExitButton.setString(L"  終了  ");

            Text_SETTINGS.setString(L"    設定    ");

            Text_Resolution.setString(L"解像度       ");
            Text_Sound_FX.setString(L"効果      ");
            Text_Music.setString(L"音楽   ");
            Text_Vignette.setString(L"ビネット   ");
            Text_Fullscreen.setString(L"フルスクリーン  ");

            Text_Your_Resolution.setString(L"    現在の解像度    ");
            Text_Changes.setString(L"グローバルな変更が行われた場合は、ゲームを再起動してください");

            Text_SaveButton.setString(L"  保存  ");
            Text_CancelButton.setString(L"キャンセル");
        }
        // Французский (FRA)
        if (Lenguage == 9)
        {
            Text_PlayButton.setString(L"Jouer");
            Text_SettingsButton.setString(L"Paramètres");
            Text_ExitButton.setString(L"Quitter");

            Text_SETTINGS.setString(L"PARAMÈTRES");

            Text_Resolution.setString(L"Résolution");
            Text_Sound_FX.setString(L"Effets");
            Text_Music.setString(L"Musique");
            Text_Vignette.setString(L"Vignette");
            Text_Fullscreen.setString(L"Plein écran");

            Text_Your_Resolution.setString(L"Votre résolution");
            Text_Changes.setString(L"Si des modifications globales ont été apportées, redémarrez le jeu");

            Text_SaveButton.setString(L"Sauvegarder");
            Text_CancelButton.setString(L"Annuler");
        }
        // Немецкий (DEU)
        if (Lenguage == 10)
        {
            Text_PlayButton.setString(L"Spielen");
            Text_SettingsButton.setString(L"Einstellungen");
            Text_ExitButton.setString(L"Beenden");

            Text_SETTINGS.setString(L"  EINSTELLUNGEN  ");

            Text_Resolution.setString(L"Auflösung");
            Text_Sound_FX.setString(L"Effekte");
            Text_Music.setString(L"Musik");
            Text_Vignette.setString(L"Vignette");
            Text_Fullscreen.setString(L"Vollbild");

            Text_Your_Resolution.setString(L"Ihre Auflösung ");
            Text_Changes.setString(L"Wenn globale Änderungen vorgenommen wurden, starten Sie das Spiel neu");

            Text_SaveButton.setString(L"Speichern");
            Text_CancelButton.setString(L"Abbrechen");
        }
        // Китайский (CHS)
        if (Lenguage == 11)
        {
            Text_PlayButton.setString(L" 开始 ");           
            Text_SettingsButton.setString(L"    设置    ");
            Text_ExitButton.setString(L"  退出  ");

            Text_SETTINGS.setString(L"    设置    ");

            Text_Resolution.setString(L"分辨率    ");
            Text_Sound_FX.setString(L"效果    ");
            Text_Music.setString(L"音乐   ");
            Text_Vignette.setString(L"暗角      ");
            Text_Fullscreen.setString(L"全屏         ");

            Text_Your_Resolution.setString(L"   您的分辨率   ");
            Text_Changes.setString(L"如果进行了全局更改，请重新启动游戏       ");

            Text_SaveButton.setString(L"  保存  ");
            Text_CancelButton.setString(L"  取消  ");
        }


    }
    void Move_text_CHS()
    {
        Text_PlayButton.      setPosition(Text_PlayButton.getPosition().x, Text_PlayButton.getPosition().y - 12);
        Text_SettingsButton.  setPosition(Text_SettingsButton.getPosition().x, Text_SettingsButton.getPosition().y - 12);
        Text_ExitButton.      setPosition(Text_ExitButton.getPosition().x, Text_ExitButton.getPosition().y - 12);
        Text_SETTINGS.        setPosition(Text_SETTINGS.getPosition().x, Text_SETTINGS.getPosition().y - 10);
        Text_Resolution.      setPosition(Text_Resolution.getPosition().x, Text_Resolution.getPosition().y - 10);
        Text_Sound_FX.        setPosition(Text_Sound_FX.getPosition().x, Text_Sound_FX.getPosition().y - 10);
        Text_Music.           setPosition(Text_Music.getPosition().x, Text_Music.getPosition().y - 10);
        Text_Vignette.        setPosition(Text_Vignette.getPosition().x, Text_Vignette.getPosition().y - 10);
        Text_Fullscreen.      setPosition(Text_Fullscreen.getPosition().x, Text_Fullscreen.getPosition().y - 10);
        Text_Your_Resolution. setPosition(Text_Your_Resolution.getPosition().x, Text_Your_Resolution.getPosition().y - 2);
        Text_SaveButton.      setPosition(Text_SaveButton.getPosition().x, Text_SaveButton.getPosition().y - 10);
        Text_CancelButton.    setPosition(Text_CancelButton.getPosition().x, Text_CancelButton.getPosition().y - 10);
    
    }
    void Move_text_JPN()
    {
        Text_PlayButton.setPosition(Text_PlayButton.getPosition().x, Text_PlayButton.getPosition().y - 10);
        Text_SettingsButton.setPosition(Text_SettingsButton.getPosition().x, Text_SettingsButton.getPosition().y - 10);
        Text_ExitButton.setPosition(Text_ExitButton.getPosition().x, Text_ExitButton.getPosition().y - 10);
        Text_SETTINGS.setPosition(Text_SETTINGS.getPosition().x, Text_SETTINGS.getPosition().y - 10);
        Text_Resolution.setPosition(Text_Resolution.getPosition().x, Text_Resolution.getPosition().y - 10);
        Text_Sound_FX.setPosition(Text_Sound_FX.getPosition().x, Text_Sound_FX.getPosition().y - 10);
        Text_Music.setPosition(Text_Music.getPosition().x, Text_Music.getPosition().y - 10);
        Text_Vignette.setPosition(Text_Vignette.getPosition().x, Text_Vignette.getPosition().y - 10);
        Text_Fullscreen.setPosition(Text_Fullscreen.getPosition().x, Text_Fullscreen.getPosition().y - 10);
        Text_Your_Resolution.setPosition(Text_Your_Resolution.getPosition().x, Text_Your_Resolution.getPosition().y - 2);
        Text_SaveButton.setPosition(Text_SaveButton.getPosition().x, Text_SaveButton.getPosition().y - 5);
        Text_CancelButton.setPosition(Text_CancelButton.getPosition().x, Text_CancelButton.getPosition().y - 4);

    }
    void Move_text_KOR()
    {
        Text_PlayButton.setPosition(Text_PlayButton.getPosition().x, Text_PlayButton.getPosition().y - 10);
        Text_SettingsButton.setPosition(Text_SettingsButton.getPosition().x, Text_SettingsButton.getPosition().y - 10);
        Text_ExitButton.setPosition(Text_ExitButton.getPosition().x, Text_ExitButton.getPosition().y - 10);
        Text_SETTINGS.setPosition(Text_SETTINGS.getPosition().x, Text_SETTINGS.getPosition().y - 10);
        Text_Resolution.setPosition(Text_Resolution.getPosition().x, Text_Resolution.getPosition().y - 10);
        Text_Sound_FX.setPosition(Text_Sound_FX.getPosition().x, Text_Sound_FX.getPosition().y - 10);
        Text_Music.setPosition(Text_Music.getPosition().x, Text_Music.getPosition().y - 10);
        Text_Vignette.setPosition(Text_Vignette.getPosition().x, Text_Vignette.getPosition().y - 10);
        Text_Fullscreen.setPosition(Text_Fullscreen.getPosition().x, Text_Fullscreen.getPosition().y - 10);
        Text_Your_Resolution.setPosition(Text_Your_Resolution.getPosition().x, Text_Your_Resolution.getPosition().y - 2);
        Text_SaveButton.setPosition(Text_SaveButton.getPosition().x, Text_SaveButton.getPosition().y - 10);
        Text_CancelButton.setPosition(Text_CancelButton.getPosition().x, Text_CancelButton.getPosition().y - 10);

    }

    void load_text_no_translate()
    {
        Text_Sound_FX_down.setString("<");  Text_Sound_FX_up.setString(">");
        Text_Music_down.setString("<");     Text_Music_up.setString(">");
        Text_Vignette_down.setString("<");  Text_Vignette_up.setString(">");

        Text_MaxFPS_down.setString("<");     Text_MaxFPS_up.setString(">");
        

        Text_Vsync_num.setString("X");
        Text_FPS_visibility_num.setString("X");
        

        Text_Fullscreen_num.setString("X");


        Text_DelGame_Button_1.setString("X");
        Text_DelGame_Button_2.setString("X");
        Text_DelGame_Button_3.setString("X");
        Text_DelGame_Button_4.setString("X");
        Text_DelGame_Button_5.setString("X");
        Text_DelGame_Button_6.setString("X");

        Text_Resolution_down.setString("<");
        Text_Resolution_up.setString(">");

        
    }


    //базовые настроки надписей 
    void Basic_text_setting()
    {
        //меню
        {
            Text_PlayButton.setFont(Font_words);     Text_PlayButton.setFillColor(sf::Color::Black);      Text_PlayButton.setCharacterSize(288);
            Text_LoadgameButton.setFont(Font_words); Text_LoadgameButton.setFillColor(sf::Color::Black);  Text_LoadgameButton.setCharacterSize(288);
            Text_SettingsButton.setFont(Font_words); Text_SettingsButton.setFillColor(sf::Color::Black);  Text_SettingsButton.setCharacterSize(288);
            Text_ExitButton.setFont(Font_words);     Text_ExitButton.setFillColor(sf::Color::Black);      Text_ExitButton.setCharacterSize(288);

            Text_Your_Resolution.setFont(Font_words);        Text_Your_Resolution.setFillColor(sf::Color::Black);      Text_Your_Resolution.setCharacterSize(288);

            Text_Resolution_num.setFont(Font_no_words);      Text_Resolution_num.setFillColor(sf::Color::Black);       Text_Resolution_num.setCharacterSize(288);
            Text_Resolution_down.setFont(Font_no_words);     Text_Resolution_down.setFillColor(sf::Color::Black);      Text_Resolution_down.setCharacterSize(288);
            Text_Resolution_up.setFont(Font_no_words);       Text_Resolution_up.setFillColor(sf::Color::Black);        Text_Resolution_up.setCharacterSize(288);

            Text_SETTINGS.setFont(Font_words);       Text_SETTINGS.setFillColor(sf::Color::Black);        Text_SETTINGS.setCharacterSize(288);
            Text_Resolution.setFont(Font_words);     Text_Resolution.setFillColor(sf::Color::Black);      Text_Resolution.setCharacterSize(288);
            Text_Sound_FX.setFont(Font_words);       Text_Sound_FX.setFillColor(sf::Color::Black);        Text_Sound_FX.setCharacterSize(288);
            Text_Music.setFont(Font_words);          Text_Music.setFillColor(sf::Color::Black);           Text_Music.setCharacterSize(288);
            Text_Vignette.setFont(Font_words);       Text_Vignette.setFillColor(sf::Color::Black);        Text_Vignette.setCharacterSize(288);
            Text_Fullscreen.setFont(Font_words);     Text_Fullscreen.setFillColor(sf::Color::Black);      Text_Fullscreen.setCharacterSize(288);
            Text_Changes.setFont(Font_words);        Text_Changes.setFillColor(sf::Color::Red);           Text_Changes.setCharacterSize(288);
            Text_Vsync.setFont(Font_words);    Text_Vsync.setFillColor(sf::Color::Black);    Text_Vsync.setCharacterSize(288);
            Text_MaxFPS.setFont(Font_words);   Text_MaxFPS.setFillColor(sf::Color::Black);   Text_MaxFPS.setCharacterSize(288);

            Text_Sound_FX_num.setFont(Font_no_words);   Text_Sound_FX_num.setFillColor(sf::Color::Black);    Text_Sound_FX_num.setCharacterSize(288);
            Text_Sound_FX_down.setFont(Font_no_words);  Text_Sound_FX_down.setFillColor(sf::Color::Black);   Text_Sound_FX_down.setCharacterSize(288);
            Text_Sound_FX_up.setFont(Font_no_words);    Text_Sound_FX_up.setFillColor(sf::Color::Black);     Text_Sound_FX_up.setCharacterSize(288);

            Text_Music_num.setFont(Font_no_words);      Text_Music_num.setFillColor(sf::Color::Black);       Text_Music_num.setCharacterSize(288);
            Text_Music_down.setFont(Font_no_words);     Text_Music_down.setFillColor(sf::Color::Black);      Text_Music_down.setCharacterSize(288);
            Text_Music_up.setFont(Font_no_words);       Text_Music_up.setFillColor(sf::Color::Black);        Text_Music_up.setCharacterSize(288);

            Text_Vignette_num.setFont(Font_no_words);   Text_Vignette_num.setFillColor(sf::Color::Black);    Text_Vignette_num.setCharacterSize(288);
            Text_Vignette_down.setFont(Font_no_words);  Text_Vignette_down.setFillColor(sf::Color::Black);   Text_Vignette_down.setCharacterSize(288);
            Text_Vignette_up.setFont(Font_no_words);    Text_Vignette_up.setFillColor(sf::Color::Black);     Text_Vignette_up.setCharacterSize(288);

            Text_Fullscreen_num.setFont(Font_no_words); Text_Fullscreen_num.setFillColor(sf::Color::Black);  Text_Fullscreen_num.setCharacterSize(288);
            Text_Vsync_num.setFont(Font_no_words);   Text_Vsync_num.setFillColor(sf::Color::Black);   Text_Vsync_num.setCharacterSize(288);
            Text_FPS_visibility_num.setFont(Font_no_words);   Text_FPS_visibility_num.setFillColor(sf::Color::Black);   Text_FPS_visibility_num.setCharacterSize(288);
            Text_MaxFPS_num.setFont(Font_no_words);       Text_MaxFPS_num.setCharacterSize(288);
            

            Text_MaxFPS_down.setFont(Font_no_words);  Text_MaxFPS_down.setFillColor(sf::Color::Black);   Text_MaxFPS_down.setCharacterSize(288);
            Text_MaxFPS_up.setFont(Font_no_words);    Text_MaxFPS_up.setFillColor(sf::Color::Black);     Text_MaxFPS_up.setCharacterSize(288);

            Text_FPS_visibility.setFont(Font_no_words);       Text_FPS_visibility.setFillColor(sf::Color::Black);        Text_FPS_visibility.setCharacterSize(288);

            Text_SaveButton.setFont(Font_words);          Text_SaveButton.setFillColor(sf::Color::Black);           Text_SaveButton.setCharacterSize(288);
            Text_CancelButton.setFont(Font_words);        Text_CancelButton.setFillColor(sf::Color::Black);         Text_CancelButton.setCharacterSize(288);

            Text_SAVE_1.setFont(Font_words);  Text_SAVE_1.setFillColor(sf::Color::Black);  Text_SAVE_1.setCharacterSize(288);
            Text_SAVE_2.setFont(Font_words);  Text_SAVE_2.setFillColor(sf::Color::Black);  Text_SAVE_2.setCharacterSize(288);
            Text_SAVE_3.setFont(Font_words);  Text_SAVE_3.setFillColor(sf::Color::Black);  Text_SAVE_3.setCharacterSize(288);
            Text_SAVE_4.setFont(Font_words);  Text_SAVE_4.setFillColor(sf::Color::Black);  Text_SAVE_4.setCharacterSize(288);
            Text_SAVE_5.setFont(Font_words);  Text_SAVE_5.setFillColor(sf::Color::Black);  Text_SAVE_5.setCharacterSize(288);
            Text_SAVE_6.setFont(Font_words);  Text_SAVE_6.setFillColor(sf::Color::Black);  Text_SAVE_6.setCharacterSize(288);

            Text_LoadGame_Button_1.setFont(Font_words);  Text_LoadGame_Button_1.setFillColor(sf::Color::Black);  Text_LoadGame_Button_1.setCharacterSize(288);
            Text_LoadGame_Button_2.setFont(Font_words);  Text_LoadGame_Button_2.setFillColor(sf::Color::Black);  Text_LoadGame_Button_2.setCharacterSize(288);
            Text_LoadGame_Button_3.setFont(Font_words);  Text_LoadGame_Button_3.setFillColor(sf::Color::Black);  Text_LoadGame_Button_3.setCharacterSize(288);
            Text_LoadGame_Button_4.setFont(Font_words);  Text_LoadGame_Button_4.setFillColor(sf::Color::Black);  Text_LoadGame_Button_4.setCharacterSize(288);
            Text_LoadGame_Button_5.setFont(Font_words);  Text_LoadGame_Button_5.setFillColor(sf::Color::Black);  Text_LoadGame_Button_5.setCharacterSize(288);
            Text_LoadGame_Button_6.setFont(Font_words);  Text_LoadGame_Button_6.setFillColor(sf::Color::Black);  Text_LoadGame_Button_6.setCharacterSize(288);

            Text_NewGame_Button_1.setFont(Font_words);  Text_NewGame_Button_1.setFillColor(sf::Color::Black);  Text_NewGame_Button_1.setCharacterSize(288);
            Text_NewGame_Button_2.setFont(Font_words);  Text_NewGame_Button_2.setFillColor(sf::Color::Black);  Text_NewGame_Button_2.setCharacterSize(288);
            Text_NewGame_Button_3.setFont(Font_words);  Text_NewGame_Button_3.setFillColor(sf::Color::Black);  Text_NewGame_Button_3.setCharacterSize(288);
            Text_NewGame_Button_4.setFont(Font_words);  Text_NewGame_Button_4.setFillColor(sf::Color::Black);  Text_NewGame_Button_4.setCharacterSize(288);
            Text_NewGame_Button_5.setFont(Font_words);  Text_NewGame_Button_5.setFillColor(sf::Color::Black);  Text_NewGame_Button_5.setCharacterSize(288);
            Text_NewGame_Button_6.setFont(Font_words);  Text_NewGame_Button_6.setFillColor(sf::Color::Black);  Text_NewGame_Button_6.setCharacterSize(288);

            Text_DelGame_Button_1.setFont(Font_no_words);  Text_DelGame_Button_1.setFillColor(sf::Color::Black);  Text_DelGame_Button_1.setCharacterSize(288);
            Text_DelGame_Button_2.setFont(Font_no_words);  Text_DelGame_Button_2.setFillColor(sf::Color::Black);  Text_DelGame_Button_2.setCharacterSize(288);
            Text_DelGame_Button_3.setFont(Font_no_words);  Text_DelGame_Button_3.setFillColor(sf::Color::Black);  Text_DelGame_Button_3.setCharacterSize(288);
            Text_DelGame_Button_4.setFont(Font_no_words);  Text_DelGame_Button_4.setFillColor(sf::Color::Black);  Text_DelGame_Button_4.setCharacterSize(288);
            Text_DelGame_Button_5.setFont(Font_no_words);  Text_DelGame_Button_5.setFillColor(sf::Color::Black);  Text_DelGame_Button_5.setCharacterSize(288);
            Text_DelGame_Button_6.setFont(Font_no_words);  Text_DelGame_Button_6.setFillColor(sf::Color::Black);  Text_DelGame_Button_6.setCharacterSize(288);

            Text_DeleteSave.setFont(Font_words);  Text_DeleteSave.setFillColor(sf::Color::Black);  Text_DeleteSave.setCharacterSize(288);
            Text_Yes_Button.setFont(Font_words);  Text_Yes_Button.setFillColor(sf::Color::Black);  Text_Yes_Button.setCharacterSize(288);
            Text_No_Button.setFont(Font_words);   Text_No_Button.setFillColor(sf::Color::Black);   Text_No_Button.setCharacterSize(288);

            
        }

        //Game_UI
        {
            Text_PAUSE.setFont(Font_words);       Text_PAUSE.setFillColor(sf::Color::Black);        Text_PAUSE.setCharacterSize(288);
            Text_SaveGame.setFont(Font_words);       Text_SaveGame.setFillColor(sf::Color::Black);        Text_SaveGame.setCharacterSize(288);
            Text_Exit.setFont(Font_words);       Text_Exit.setFillColor(sf::Color::Black);        Text_Exit.setCharacterSize(288);
            
            

        }

        
    }


    ////////////////////////////// общие настройки интерфейса которые могут быть динамичными
    void UI_load_setting(sf::Vector2u Menu_Size)
    {
         
        // параметры   PlayButton 
        {
            PlayButton.setPosition(sf::Vector2f(Menu_Size.x * 0.02, Menu_Size.y * 0.36));
            PlayButton.setSize(sf::Vector2f(Menu_Size.x * 0.12, Menu_Size.y * 0.1));
            
            sf::FloatRect Bounds = Text_PlayButton.getLocalBounds();
            float Width = Bounds.width;
            float targetWidth = PlayButton.getSize().x * 0.7f;
            float scaleX = targetWidth / Width;
            float targetHeight = PlayButton.getSize().y * 0.5f; 
            float Height = Bounds.height;
            float scaleY = targetHeight / Height;
            // Устанавливаем масштаб
            Text_PlayButton.setScale(scaleX, scaleY);


            Text_PlayButton.setOrigin(Bounds.width / 2, Bounds.height / 2);
            Text_PlayButton.setPosition
            (
                PlayButton.getPosition().x + PlayButton.getSize().x / 2, // Центр по X
                PlayButton.getPosition().y + PlayButton.getSize().y / 2  // Центр по Y
            );
        }
        // параметры   LoadgameButton 
        {
            if (isActive_Save == false) { LoadgameButton.setFillColor(sf::Color(100, 100, 100)); }
            LoadgameButton.setPosition(sf::Vector2f(Menu_Size.x * 0.02, Menu_Size.y * 0.54));
            LoadgameButton.setSize(sf::Vector2f(Menu_Size.x * 0.2, Menu_Size.y * 0.1));

            sf::FloatRect Bounds = Text_LoadgameButton.getLocalBounds();
            float Width = Bounds.width;
            float targetWidth = LoadgameButton.getSize().x * 0.8f;
            float scaleX = targetWidth / Width;
            float targetHeight = LoadgameButton.getSize().y * 0.5f;
            float Height = Bounds.height;
            float scaleY = targetHeight / Height;
            // Устанавливаем масштаб
            Text_LoadgameButton.setScale(scaleX, scaleY);




            Text_LoadgameButton.setOrigin(Bounds.width / 2, Bounds.height / 2);
            Text_LoadgameButton.setPosition
            (
                LoadgameButton.getPosition().x + LoadgameButton.getSize().x / 2, // Центр по X
                LoadgameButton.getPosition().y + LoadgameButton.getSize().y / 2  // Центр по Y
            );

        }
        // параметры   SettingsButton 
        {
            SettingsButton.setPosition(sf::Vector2f(Menu_Size.x * 0.02, Menu_Size.y * 0.72));
            SettingsButton.setSize(sf::Vector2f(Menu_Size.x * 0.2, Menu_Size.y * 0.1));

            sf::FloatRect Bounds = Text_SettingsButton.getLocalBounds();
            float Width = Bounds.width;
            float targetWidth = SettingsButton.getSize().x * 0.8f;
            float scaleX = targetWidth / Width;
            float targetHeight = SettingsButton.getSize().y * 0.5f;
            float Height = Bounds.height;
            float scaleY = targetHeight / Height;
            // Устанавливаем масштаб
            Text_SettingsButton.setScale(scaleX, scaleY);




            Text_SettingsButton.setOrigin(Bounds.width / 2, Bounds.height / 2);
            Text_SettingsButton.setPosition
            (
                SettingsButton.getPosition().x + SettingsButton.getSize().x / 2, // Центр по X
                SettingsButton.getPosition().y + SettingsButton.getSize().y / 2  // Центр по Y
            );

        }
        // параметры   ExitButton 
        {

            ExitButton.setPosition(sf::Vector2f(Menu_Size.x * 0.02, Menu_Size.y * 0.90));
            ExitButton.setSize(sf::Vector2f(Menu_Size.x * 0.12, Menu_Size.y * 0.1));

            
            sf::FloatRect Bounds = Text_ExitButton.getLocalBounds();
            float Width = Bounds.width;
            float targetWidth = ExitButton.getSize().x * 0.7f;
            float scaleX = targetWidth / Width;
            float targetHeight = ExitButton.getSize().y * 0.5f;
            float Height = Bounds.height;
            float scaleY = targetHeight / Height;
            // Устанавливаем масштаб
            Text_ExitButton.setScale(scaleX, scaleY);




            Text_ExitButton.setOrigin(Bounds.width / 2, Bounds.height / 2);
            Text_ExitButton.setPosition
            (
                ExitButton.getPosition().x + ExitButton.getSize().x / 2, // Центр по X
                ExitButton.getPosition().y + ExitButton.getSize().y / 2  // Центр по Y
            );
        }


        // параметры   SettingsPanel
        {
            //////////////// Global   ////////////////
                   
            
            //Text_SETTINGS 
            {
                sf::FloatRect Bounds = Text_SETTINGS.getLocalBounds();

                float Width = Bounds.width;
                float targetWidth = SettingsPanel.getSize().x * 0.7;
                float scaleX = targetWidth / Width;
                float targetHeight = SettingsPanel.getSize().y * 0.07f;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_SETTINGS.setScale(scaleX, scaleY);

                Text_SETTINGS.setOrigin(Bounds.width / 2, Bounds.height / 2);
                Text_SETTINGS.setPosition
                (
                    SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 2, // Центр по X
                    SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 7.7  // Центр по Y
                );
            }
            //Text_Resolution 
            {
                sf::FloatRect Bounds = Text_Resolution.getLocalBounds();
                
                float Width = Bounds.width;
                float targetWidth = SettingsPanel.getSize().x * 0.4;
                float scaleX = targetWidth / Width;
                float targetHeight = SettingsPanel.getSize().y * 0.04;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_Resolution.setScale(scaleX, scaleY);

                Text_Resolution.setOrigin(0, Bounds.height / 2);
                Text_Resolution.setPosition
                (
                    SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 9, // Центр по X
                    SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 4.5  // Центр по Y     
                );
            }
            //Text_Sound_FX 
            {
                sf::FloatRect Bounds = Text_Sound_FX.getLocalBounds();

                float Width = Bounds.width;
                float targetWidth = SettingsPanel.getSize().x * 0.3;
                float scaleX = targetWidth / Width;
                float targetHeight = SettingsPanel.getSize().y * 0.04;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_Sound_FX.setScale(scaleX, scaleY);

                Text_Sound_FX.setOrigin(0, Bounds.height / 2);
                Text_Sound_FX.setPosition
                (
                    SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 9, // Центр по X
                    SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 3.3  // Центр по Y
                );
            }
            //Text_Music 
            {
                sf::FloatRect Bounds = Text_Music.getLocalBounds();

                float Width = Bounds.width;
                float targetWidth = SettingsPanel.getSize().x * 0.2;
                float scaleX = targetWidth / Width;
                float targetHeight = SettingsPanel.getSize().y * 0.04;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_Music.setScale(scaleX, scaleY);

                Text_Music.setOrigin(0, Bounds.height / 2);
                Text_Music.setPosition
                (
                    SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 9, // Центр по X
                    SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 2.65  // Центр по Y
                );
            }
            //Text_Vignette 
            {
                sf::FloatRect Bounds = Text_Vignette.getLocalBounds();

                float Width = Bounds.width;
                float targetWidth = SettingsPanel.getSize().x * 0.3;
                float scaleX = targetWidth / Width;
                float targetHeight = SettingsPanel.getSize().y * 0.04;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_Vignette.setScale(scaleX, scaleY);

                Text_Vignette.setOrigin(0, Bounds.height / 2);
                Text_Vignette.setPosition
                (
                    SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 9, // Центр по X
                    SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 2.17  // Центр по Y
                );
            }
            //Text_Fullscreen 
            {
                sf::FloatRect Bounds = Text_Fullscreen.getLocalBounds();

                float Width = Bounds.width;
                float targetWidth = SettingsPanel.getSize().x * 0.4;
                float scaleX = targetWidth / Width;
                float targetHeight = SettingsPanel.getSize().y * 0.04;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_Fullscreen.setScale(scaleX, scaleY);

                Text_Fullscreen.setOrigin(0, Bounds.height / 2);
                Text_Fullscreen.setPosition
                (
                    SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 9, // Центр по X
                    SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.85  // Центр по Y
                );
            }
            //Text_Changes 
            {
                sf::FloatRect Bounds = Text_Changes.getLocalBounds();

                float Width = Bounds.width;
                float targetWidth = SettingsPanel.getSize().x * 0.77;
                float scaleX = targetWidth / Width;
                float targetHeight = SettingsPanel.getSize().y * 0.02;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_Changes.setScale(scaleX, scaleY);

                Text_Changes.setOrigin(0, Bounds.height / 2);
                Text_Changes.setPosition
                (
                    SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 9, // Центр по X
                    SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.27  // Центр по Y
                );
            }


            ////////  регуляторы  
            
            // Sound_FX
            {
                //  0 - 100
                {
                    // параметры фона 
                    Fon_Sound_FX_num.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.10, SettingsPanel.getSize().y * 0.028));

                    

                    sf::FloatRect Bounds = Fon_Sound_FX_num.getLocalBounds();
                    // устанавливаем отчёт от центра фона
                    Fon_Sound_FX_num.setOrigin(Bounds.width / 2, Bounds.height / 2);
                    //размещаем относительно панели настроек
                    Fon_Sound_FX_num.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.38, // Центр по X
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 3.3  // Центр по Y
                    );

                    // настройки текста
                    Bounds = Text_Sound_FX_num.getLocalBounds();
                    float Width = Bounds.width;
                    float targetWidth = Fon_Sound_FX_num.getSize().x * 0.75; // размер относительно фона
                    float scaleX = targetWidth / Width;
                    float targetHeight = Fon_Sound_FX_num.getSize().y * 0.75;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_Sound_FX_num.setScale(scaleX, scaleY); //задаём размер

                    // устанавливаем отчёт от центра текста

                    Text_Sound_FX_num.setOrigin(Bounds.width / 2, Bounds.height / 2);

                    sf::FloatRect fonBounds = Fon_Sound_FX_num.getGlobalBounds(); // Берём границы фона

                    Text_Sound_FX_num.setPosition(
                        fonBounds.left + fonBounds.width / 2.2, // Центрируем по X
                        fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                    );
                }
                // <
                {

                    Fon_Sound_FX_down.setSize(sf::Vector2f(Menu_Size.x * 0.013, Menu_Size.y * 0.025));


                    sf::FloatRect Bounds = Fon_Sound_FX_down.getLocalBounds();
                    // устанавливаем отчёт от центра фона
                    Fon_Sound_FX_down.setOrigin(Bounds.width, Bounds.height - Bounds.height);  // ширина - высота

                    //размещаем относительно фона цифер который показывает значения
                    Fon_Sound_FX_down.setPosition
                    (
                        Fon_Sound_FX_num.getPosition().x - Fon_Sound_FX_num.getSize().x / 2, // Центр по X
                        Fon_Sound_FX_num.getPosition().y - Fon_Sound_FX_num.getSize().y / 2  // Центр по Y
                    );


                    // настройки текста

                    Bounds = Text_Sound_FX_down.getLocalBounds();
                    float Width = Bounds.width;
                    float targetWidth = Fon_Sound_FX_down.getSize().x * 0.75; // размер относительно фона
                    float scaleX = targetWidth / Width;
                    float targetHeight = Fon_Sound_FX_down.getSize().y * 0.75;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_Sound_FX_down.setScale(scaleX, scaleY); //задаём размер


                    // устанавливаем отчёт от центра текста и задаём позицию

                    Text_Sound_FX_down.setOrigin(Bounds.width / 2, Bounds.height / 2);

                    sf::FloatRect fonBounds = Fon_Sound_FX_down.getGlobalBounds(); // Берём границы фона

                    Text_Sound_FX_down.setPosition(
                        fonBounds.left + fonBounds.width / 3, // Центрируем по X
                        fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                    );

                }
                // >
                {

                    Fon_Sound_FX_up.setSize(sf::Vector2f(Menu_Size.x * 0.013, Menu_Size.y * 0.025));

                    sf::FloatRect Bounds = Fon_Sound_FX_up.getLocalBounds();
                    // устанавливаем отчёт от центра фона
                    Fon_Sound_FX_up.setOrigin(0, Bounds.height);

                    //размещаем относительно фона цифер который показывает значения
                    Fon_Sound_FX_up.setPosition
                    (
                        Fon_Sound_FX_num.getPosition().x + Fon_Sound_FX_num.getSize().x / 2, // Центр по X
                        Fon_Sound_FX_num.getPosition().y + Fon_Sound_FX_num.getSize().y / 2  // Центр по Y
                    );


                    // настройки текста

                    Bounds = Text_Sound_FX_up.getLocalBounds();
                    float Width = Bounds.width;
                    float targetWidth = Fon_Sound_FX_up.getSize().x * 0.75; // размер относительно фона
                    float scaleX = targetWidth / Width;
                    float targetHeight = Fon_Sound_FX_up.getSize().y * 0.75;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_Sound_FX_up.setScale(scaleX, scaleY); //задаём размер


                    // устанавливаем отчёт от центра текста и задаём позицию
                    Text_Sound_FX_up.setOrigin(Bounds.width / 2, Bounds.height / 2);

                    sf::FloatRect fonBounds = Fon_Sound_FX_up.getGlobalBounds(); // Берём границы фона

                    Text_Sound_FX_up.setPosition(
                        fonBounds.left + fonBounds.width / 3, // Центрируем по X
                        fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                    );
                }

            }
            //Music
   if (true)
            {
                //  0 - 100
                {
                    // параметры фона 
                    Fon_Music_num.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.10, SettingsPanel.getSize().y * 0.028));

                    sf::FloatRect Bounds = Fon_Music_num.getLocalBounds();
                    // устанавливаем отчёт от центра фона
                    Fon_Music_num.setOrigin(Bounds.width / 2, Bounds.height / 2);
                    //размещаем относительно панели настроек
                    Fon_Music_num.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.38, // Центр по X
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 2.65  // Центр по Y
                    );

                    // настройки текста
                    Bounds = Text_Music_num.getLocalBounds();
                    float Width = Bounds.width;
                    float targetWidth = Fon_Music_num.getSize().x * 0.75; // размер относительно фона
                    float scaleX = targetWidth / Width;
                    float targetHeight = Fon_Music_num.getSize().y * 0.75;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_Music_num.setScale(scaleX, scaleY); //задаём размер

                    // устанавливаем отчёт от центра текста

                    Text_Music_num.setOrigin(Bounds.width / 2, Bounds.height / 2);

                    sf::FloatRect fonBounds = Fon_Music_num.getGlobalBounds(); // Берём границы фона

                    Text_Music_num.setPosition(
                        fonBounds.left + fonBounds.width / 2.2, // Центрируем по X
                        fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                    );
                }       
                // <
                {

                    Fon_Music_down.setSize(sf::Vector2f(Menu_Size.x * 0.013, Menu_Size.y * 0.025));


                    sf::FloatRect Bounds = Fon_Music_down.getLocalBounds();
                    // устанавливаем отчёт от центра фона
                    Fon_Music_down.setOrigin(Bounds.width, Bounds.height - Bounds.height);  // ширина - высота

                    //размещаем относительно фона цифер который показывает значения
                    Fon_Music_down.setPosition
                    (
                        Fon_Music_num.getPosition().x - Fon_Music_num.getSize().x / 2, // Центр по X
                        Fon_Music_num.getPosition().y - Fon_Music_num.getSize().y / 2  // Центр по Y
                    );


                    // настройки текста

                    Bounds = Text_Music_down.getLocalBounds();
                    float Width = Bounds.width;
                    float targetWidth = Fon_Music_down.getSize().x * 0.75; // размер относительно фона
                    float scaleX = targetWidth / Width;
                    float targetHeight = Fon_Music_down.getSize().y * 0.75;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_Music_down.setScale(scaleX, scaleY); //задаём размер


                    // устанавливаем отчёт от центра текста и задаём позицию

                    Text_Music_down.setOrigin(Bounds.width / 2, Bounds.height / 2);

                    sf::FloatRect fonBounds = Fon_Music_down.getGlobalBounds(); // Берём границы фона

                    Text_Music_down.setPosition(
                        fonBounds.left + fonBounds.width / 3, // Центрируем по X
                        fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                    );

                }
                // >
                {

                    Fon_Music_up.setSize(sf::Vector2f(Menu_Size.x * 0.013, Menu_Size.y * 0.025));

                    sf::FloatRect Bounds = Fon_Music_up.getLocalBounds();
                    // устанавливаем отчёт от центра фона
                    Fon_Music_up.setOrigin(0, Bounds.height);

                    //размещаем относительно фона цифер который показывает значения
                    Fon_Music_up.setPosition
                    (
                        Fon_Music_num.getPosition().x + Fon_Music_num.getSize().x / 2, // Центр по X
                        Fon_Music_num.getPosition().y + Fon_Music_num.getSize().y / 2  // Центр по Y
                    );


                    // настройки текста

                    Bounds = Text_Music_up.getLocalBounds();
                    float Width = Bounds.width;
                    float targetWidth = Fon_Music_up.getSize().x * 0.75; // размер относительно фона
                    float scaleX = targetWidth / Width;
                    float targetHeight = Fon_Music_up.getSize().y * 0.75;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_Music_up.setScale(scaleX, scaleY); //задаём размер


                    // устанавливаем отчёт от центра текста и задаём позицию
                    Text_Music_up.setOrigin(Bounds.width / 2, Bounds.height / 2);

                    sf::FloatRect fonBounds = Fon_Music_up.getGlobalBounds(); // Берём границы фона

                    Text_Music_up.setPosition(
                        fonBounds.left + fonBounds.width / 3, // Центрируем по X
                        fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                    );
                }

            }
            // Vignette
   if (true)
            {
                //  0 - 100
                {
                    // параметры фона 
                    Fon_Vignette_num.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.10, SettingsPanel.getSize().y * 0.028));

                    sf::FloatRect Bounds = Fon_Vignette_num.getLocalBounds();
                    // устанавливаем отчёт от центра фона
                    Fon_Vignette_num.setOrigin(Bounds.width / 2, Bounds.height / 2);
                    //размещаем относительно панели настроек
                    Fon_Vignette_num.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.38, // Центр по X
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 2.17  // Центр по Y
                    );

                    // настройки текста
                    Bounds = Text_Vignette_num.getLocalBounds();
                    float Width = Bounds.width;
                    float targetWidth = Fon_Vignette_num.getSize().x * 0.75; // размер относительно фона
                    float scaleX = targetWidth / Width;
                    float targetHeight = Fon_Vignette_num.getSize().y * 0.75;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_Vignette_num.setScale(scaleX, scaleY); //задаём размер

                    // устанавливаем отчёт от центра текста

                    Text_Vignette_num.setOrigin(Bounds.width / 2, Bounds.height / 2);

                    sf::FloatRect fonBounds = Fon_Vignette_num.getGlobalBounds(); // Берём границы фона

                    Text_Vignette_num.setPosition(
                        fonBounds.left + fonBounds.width / 2.2, // Центрируем по X
                        fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                    );
                }
                // <
                {

                    Fon_Vignette_down.setSize(sf::Vector2f(Menu_Size.x * 0.013, Menu_Size.y * 0.025));


                    sf::FloatRect Bounds = Fon_Vignette_down.getLocalBounds();
                    // устанавливаем отчёт от центра фона
                    Fon_Vignette_down.setOrigin(Bounds.width, Bounds.height - Bounds.height);  // ширина - высота

                    //размещаем относительно фона цифер который показывает значения
                    Fon_Vignette_down.setPosition
                    (
                        Fon_Vignette_num.getPosition().x - Fon_Vignette_num.getSize().x / 2, // Центр по X
                        Fon_Vignette_num.getPosition().y - Fon_Vignette_num.getSize().y / 2  // Центр по Y
                    );


                    // настройки текста

                    Bounds = Text_Vignette_down.getLocalBounds();
                    float Width = Bounds.width;
                    float targetWidth = Fon_Vignette_down.getSize().x * 0.75; // размер относительно фона
                    float scaleX = targetWidth / Width;
                    float targetHeight = Fon_Vignette_down.getSize().y * 0.75;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_Vignette_down.setScale(scaleX, scaleY); //задаём размер


                    // устанавливаем отчёт от центра текста и задаём позицию

                    Text_Vignette_down.setOrigin(Bounds.width / 2, Bounds.height / 2);

                    sf::FloatRect fonBounds = Fon_Vignette_down.getGlobalBounds(); // Берём границы фона

                    Text_Vignette_down.setPosition(
                        fonBounds.left + fonBounds.width / 3, // Центрируем по X
                        fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                    );

                }
                // >
                {

                    Fon_Vignette_up.setSize(sf::Vector2f(Menu_Size.x * 0.013, Menu_Size.y * 0.025));

                    sf::FloatRect Bounds = Fon_Vignette_up.getLocalBounds();
                    // устанавливаем отчёт от центра фона
                    Fon_Vignette_up.setOrigin(0, Bounds.height);

                    //размещаем относительно фона цифер который показывает значения
                    Fon_Vignette_up.setPosition
                    (
                        Fon_Vignette_num.getPosition().x + Fon_Vignette_num.getSize().x / 2, // Центр по X
                        Fon_Vignette_num.getPosition().y + Fon_Vignette_num.getSize().y / 2  // Центр по Y
                    );


                    // настройки текста

                    Bounds = Text_Vignette_up.getLocalBounds();
                    float Width = Bounds.width;
                    float targetWidth = Fon_Vignette_up.getSize().x * 0.75; // размер относительно фона
                    float scaleX = targetWidth / Width;
                    float targetHeight = Fon_Vignette_up.getSize().y * 0.75;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_Vignette_up.setScale(scaleX, scaleY); //задаём размер


                    // устанавливаем отчёт от центра текста и задаём позицию
                    Text_Vignette_up.setOrigin(Bounds.width / 2, Bounds.height / 2);

                    sf::FloatRect fonBounds = Fon_Vignette_up.getGlobalBounds(); // Берём границы фона

                    Text_Vignette_up.setPosition(
                        fonBounds.left + fonBounds.width / 3, // Центрируем по X
                        fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                    );
                }

            }
            // Fullscreen     
   if (true) {
       // параметры фона 
       Fon_Fullscreen.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.080, SettingsPanel.getSize().y * 0.050));
       //Fon_Fullscreen.setFillColor(sf::Color(190, 190, 190));

       sf::FloatRect Bounds = Fon_Fullscreen.getLocalBounds();
       // устанавливаем отчёт от центра фона
       Fon_Fullscreen.setOrigin(Bounds.width / 2, Bounds.height / 2);
       //размещаем относительно панели настроек
       Fon_Fullscreen.setPosition
       (
           SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.38, // Центр по X
           SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.85  // Центр по Y
       );

       // настройки текста
       Bounds = Text_Fullscreen_num.getLocalBounds();
       float Width = Bounds.width;
       float targetWidth = Fon_Fullscreen.getSize().x * 0.5; // размер относительно фона
       float scaleX = targetWidth / Width;
       float targetHeight = Fon_Fullscreen.getSize().y * 0.5;
       float Height = Bounds.height;
       float scaleY = targetHeight / Height;
       Text_Fullscreen_num.setScale(scaleX, scaleY); //задаём размер


       // устанавливаем отчёт от центра текста и задаём позицию


       Text_Fullscreen_num.setOrigin(Text_Fullscreen_num.getLocalBounds().width / 2, Text_Fullscreen_num.getLocalBounds().height / 2);
       Text_Fullscreen_num.setPosition
       (
           SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.38, // Центр по X
           SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.85  // Центр по Y
       );
   }
            //Resolution
            {
                //  центр 
                {
                    // параметры фона 
                    Fon_Resolution_num.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.23, SettingsPanel.getSize().y * 0.033));

                    sf::FloatRect Bounds = Fon_Resolution_num.getLocalBounds();
                    // устанавливаем отчёт от центра фона
                    Fon_Resolution_num.setOrigin(Bounds.width / 2, Bounds.height / 2);
                    //размещаем относительно панели настроек
                    Fon_Resolution_num.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.38, // Центр по X
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 4.5  // Центр по Y
                    );

                    // настройки текста 1
                    Bounds = Text_Resolution_num.getLocalBounds();
                    float Width = Bounds.width;
                    float targetWidth = Fon_Resolution_num.getSize().x * 0.85; // размер относительно фона
                    float scaleX = targetWidth / Width;
                    float targetHeight = Fon_Resolution_num.getSize().y * 0.75;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_Resolution_num.setScale(scaleX, scaleY); //задаём размер

                    // устанавливаем отчёт от центра текста

                    Text_Resolution_num.setOrigin(Bounds.width / 2, Bounds.height / 2);

                    sf::FloatRect fonBounds = Fon_Resolution_num.getGlobalBounds(); // Берём границы фона

                    Text_Resolution_num.setPosition(
                        fonBounds.left + fonBounds.width / 2.2, // Центрируем по X
                        fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                    );

                    // настройки текста 2
                    Bounds = Text_Your_Resolution.getLocalBounds();
                     Width = Bounds.width;
                     targetWidth = Fon_Resolution_num.getSize().x * 0.90; // размер относительно фона
                     scaleX = targetWidth / Width;
                     targetHeight = Fon_Resolution_num.getSize().y * 0.40;
                     Height = Bounds.height;
                     scaleY = targetHeight / Height;
                    Text_Your_Resolution.setScale(scaleX, scaleY); //задаём размер


                    Text_Your_Resolution.setOrigin(Bounds.width / 2, Bounds.height / 2);
                    Text_Your_Resolution.setPosition(
                        fonBounds.left + fonBounds.width / 2, // Центрируем по X
                        fonBounds.top + fonBounds.height * 1.3 // Центрируем по Y
                    );




                }
                // <
                {

                    Fon_Resolution_down.setSize(sf::Vector2f(Menu_Size.x * 0.018, Menu_Size.y * 0.030)); 


                    sf::FloatRect Bounds = Fon_Resolution_down.getLocalBounds();
                    // устанавливаем отчёт от центра фона
                    Fon_Resolution_down.setOrigin(Bounds.width, Bounds.height - Bounds.height);  // ширина - высота

                    //размещаем относительно фона цифер который показывает значения
                    Fon_Resolution_down.setPosition
                    (
                        Fon_Resolution_num.getPosition().x - Fon_Resolution_num.getSize().x / 2, // Центр по X
                        Fon_Resolution_num.getPosition().y - Fon_Resolution_num.getSize().y / 2  // Центр по Y
                    );


                    // настройки текста

                    Bounds = Text_Resolution_down.getLocalBounds();
                    float Width = Bounds.width;
                    float targetWidth = Fon_Resolution_down.getSize().x * 0.75; // размер относительно фона
                    float scaleX = targetWidth / Width;
                    float targetHeight = Fon_Resolution_down.getSize().y * 0.75;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_Resolution_down.setScale(scaleX, scaleY); //задаём размер


                    // устанавливаем отчёт от центра текста и задаём позицию

                    Text_Resolution_down.setOrigin(Bounds.width / 2, Bounds.height / 2);

                    sf::FloatRect fonBounds = Fon_Resolution_down.getGlobalBounds(); // Берём границы фона

                    Text_Resolution_down.setPosition(
                        fonBounds.left + fonBounds.width / 3, // Центрируем по X
                        fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                    );

                }
                // >
                {

                    Fon_Resolution_up.setSize(sf::Vector2f(Menu_Size.x * 0.018, Menu_Size.y * 0.030));

                    sf::FloatRect Bounds = Fon_Resolution_up.getLocalBounds();
                    // устанавливаем отчёт от центра фона
                    Fon_Resolution_up.setOrigin(0, Bounds.height);

                    //размещаем относительно фона цифер который показывает значения
                    Fon_Resolution_up.setPosition
                    (
                        Fon_Resolution_num.getPosition().x + Fon_Resolution_num.getSize().x / 2, // Центр по X
                        Fon_Resolution_num.getPosition().y + Fon_Resolution_num.getSize().y / 2  // Центр по Y
                    );


                    // настройки текста

                    Bounds = Text_Resolution_up.getLocalBounds();
                    float Width = Bounds.width;
                    float targetWidth = Fon_Resolution_up.getSize().x * 0.75; // размер относительно фона
                    float scaleX = targetWidth / Width;
                    float targetHeight = Fon_Resolution_up.getSize().y * 0.75;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_Resolution_up.setScale(scaleX, scaleY); //задаём размер


                    // устанавливаем отчёт от центра текста и задаём позицию
                    Text_Resolution_up.setOrigin(Bounds.width / 2, Bounds.height / 2);

                    sf::FloatRect fonBounds = Fon_Resolution_up.getGlobalBounds(); // Берём границы фона

                    Text_Resolution_up.setPosition(
                        fonBounds.left + fonBounds.width / 3, // Центрируем по X
                        fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                    );
                }

            }


            //////// флаги языков
            if (true)
            {
                // ENG
                if (true)
                {
                    ENG_LenguageButton.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.090, SettingsPanel.getSize().y * 0.040));
                    sf::FloatRect Bounds = ENG_LenguageButton.getLocalBounds();
                    ENG_LenguageButton.setOrigin(0, Bounds.height / 2);

                    //размещаем относительно панели настроек
                    ENG_LenguageButton.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 9, // Центр по X
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.45  // Центр по Y
                    );

                }
                // RUS
                if (true)
                {
                    RUS_LenguageButton.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.090, SettingsPanel.getSize().y * 0.040));
                    sf::FloatRect Bounds = RUS_LenguageButton.getLocalBounds();
                    RUS_LenguageButton.setOrigin(0, Bounds.height / 2);

                    //размещаем относительно панели настроек
                    RUS_LenguageButton.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 4.5, // Центр по X
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.45  // Центр по Y
                    );

                }
                // TUR
                if (true)
                {
                    TUR_LenguageButton.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.090, SettingsPanel.getSize().y * 0.040));
                    sf::FloatRect Bounds = TUR_LenguageButton.getLocalBounds();
                    TUR_LenguageButton.setOrigin(0, Bounds.height / 2);

                    TUR_LenguageButton.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 3,
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.45
                    );
                }
                // UKR
                if (true)
                {
                    UKR_LenguageButton.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.090, SettingsPanel.getSize().y * 0.040));
                    sf::FloatRect Bounds = UKR_LenguageButton.getLocalBounds();
                    UKR_LenguageButton.setOrigin(0, Bounds.height / 2);

                    UKR_LenguageButton.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 2.25,
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.45
                    );
                }
                // SPA
                if (true)
                {
                    SPA_LenguageButton.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.090, SettingsPanel.getSize().y * 0.040));
                    sf::FloatRect Bounds = SPA_LenguageButton.getLocalBounds();
                    SPA_LenguageButton.setOrigin(0, Bounds.height / 2);

                    SPA_LenguageButton.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.8,
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.45
                    );
                }
                // POR
                if (true)
                {
                    POR_LenguageButton.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.090, SettingsPanel.getSize().y * 0.040));
                    sf::FloatRect Bounds = POR_LenguageButton.getLocalBounds();
                    POR_LenguageButton.setOrigin(0, Bounds.height / 2);

                    POR_LenguageButton.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.5,
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.45
                    );
                }
                // POL
                if (true)
                {
                    POL_LenguageButton.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.090, SettingsPanel.getSize().y * 0.040));
                    sf::FloatRect Bounds = POL_LenguageButton.getLocalBounds();
                    POL_LenguageButton.setOrigin(0, Bounds.height / 2);

                    POL_LenguageButton.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.285,
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.45
                    );
                }
                // KOR
                if (true)
                {
                    KOR_LenguageButton.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.090, SettingsPanel.getSize().y * 0.040));
                    sf::FloatRect Bounds = KOR_LenguageButton.getLocalBounds();
                    KOR_LenguageButton.setOrigin(0, Bounds.height / 2);

                    KOR_LenguageButton.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 9,
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.34
                    );
                }
                // JPN
                if (true)
                {
                    JPN_LenguageButton.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.090, SettingsPanel.getSize().y * 0.040));
                    sf::FloatRect Bounds = JPN_LenguageButton.getLocalBounds();
                    JPN_LenguageButton.setOrigin(0, Bounds.height / 2);

                    JPN_LenguageButton.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 4.5,
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.34
                    );
                }
                // FRA
                if (true)
                {
                    FRA_LenguageButton.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.090, SettingsPanel.getSize().y * 0.040));
                    sf::FloatRect Bounds = FRA_LenguageButton.getLocalBounds();
                    FRA_LenguageButton.setOrigin(0, Bounds.height / 2);

                    FRA_LenguageButton.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 3,
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.34
                    );
                }
                // DEU
                if (true)
                {
                    DEU_LenguageButton.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.090, SettingsPanel.getSize().y * 0.040));
                    sf::FloatRect Bounds = DEU_LenguageButton.getLocalBounds();
                    DEU_LenguageButton.setOrigin(0, Bounds.height / 2);

                    DEU_LenguageButton.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 2.25,
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.34
                    );
                }
                // CHS (упрощённый китайский)
                if (true)
                {
                    CHS_LenguageButton.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.090, SettingsPanel.getSize().y * 0.040));
                    sf::FloatRect Bounds = CHS_LenguageButton.getLocalBounds();
                    CHS_LenguageButton.setOrigin(0, Bounds.height / 2);

                    CHS_LenguageButton.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.8,
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.34
                    );
                }
            }



            ////////////////  Graphics  //////////////// 
  
            //Text_Vsync 
            if(true) 
            {
                    sf::FloatRect Bounds = Text_Vsync.getLocalBounds();

                    float Width = Bounds.width;
                    float targetWidth = SettingsPanel.getSize().x * 0.4;
                    float scaleX = targetWidth / Width;
                    float targetHeight = SettingsPanel.getSize().y * 0.04;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_Vsync.setScale(scaleX, scaleY);

                    Text_Vsync.setOrigin(0, Bounds.height / 2);
                    Text_Vsync.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 9, // Центр по X
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 4.5  // Центр по Y     
                    );
            }           
            //Vsync_Num
            {

                // параметры фона 
                Fon_Vsync.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.080, SettingsPanel.getSize().y * 0.050));

                sf::FloatRect Bounds = Fon_Vsync.getLocalBounds();
                // устанавливаем отчёт от центра фона
                Fon_Vsync.setOrigin(Bounds.width / 2, Bounds.height / 2);
                //размещаем относительно панели настроек
                Fon_Vsync.setPosition
                (
                    SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.38, // Центр по X
                    SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 4.5  // Центр по Y
                );

                // настройки текста
                Bounds = Text_Vsync_num.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = Fon_Vsync.getSize().x * 0.5; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = Fon_Vsync.getSize().y * 0.5;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_Vsync_num.setScale(scaleX, scaleY); //задаём размер


                // устанавливаем отчёт от центра текста и задаём позицию


                Text_Vsync_num.setOrigin(Text_Vsync_num.getLocalBounds().width / 2, Text_Vsync_num.getLocalBounds().height / 2);
                Text_Vsync_num.setPosition
                (
                    SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.38, // Центр по X
                    SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 4.5  // Центр по Y
                );


            }

            //Text_MaxFPS 
            {
                sf::FloatRect Bounds = Text_MaxFPS.getLocalBounds();

                float Width = Bounds.width;
                float targetWidth = SettingsPanel.getSize().x * 0.3;
                float scaleX = targetWidth / Width;
                float targetHeight = SettingsPanel.getSize().y * 0.04;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_MaxFPS.setScale(scaleX, scaleY);

                Text_MaxFPS.setOrigin(0, Bounds.height / 2);
                Text_MaxFPS.setPosition
                (
                    SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 9, // Центр по X
                    SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 3.3  // Центр по Y
                );
            }
            // MaxFPS
            {
                //  0 - 100
                {
                    // параметры фона 
                    Fon_MaxFPS_num.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.10, SettingsPanel.getSize().y * 0.028));



                    sf::FloatRect Bounds = Fon_MaxFPS_num.getLocalBounds();
                    // устанавливаем отчёт от центра фона
                    Fon_MaxFPS_num.setOrigin(Bounds.width / 2, Bounds.height / 2);
                    //размещаем относительно панели настроек
                    Fon_MaxFPS_num.setPosition
                    (
                        SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.38, // Центр по X
                        SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 3.3  // Центр по Y
                    );

                    // настройки текста
                    Bounds = Text_MaxFPS_num.getLocalBounds();
                    float Width = Bounds.width;
                    float targetWidth = Fon_MaxFPS_num.getSize().x * 0.75; // размер относительно фона
                    float scaleX = targetWidth / Width;
                    float targetHeight = Fon_MaxFPS_num.getSize().y * 0.75;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_MaxFPS_num.setScale(scaleX, scaleY); //задаём размер

                    // устанавливаем отчёт от центра текста

                    Text_MaxFPS_num.setOrigin(Bounds.width / 2, Bounds.height / 2);

                    sf::FloatRect fonBounds = Fon_MaxFPS_num.getGlobalBounds(); // Берём границы фона

                    Text_MaxFPS_num.setPosition(
                        fonBounds.left + fonBounds.width / 2.2, // Центрируем по X
                        fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                    );
                }
                // <
                {

                    Fon_MaxFPS_down.setSize(sf::Vector2f(Menu_Size.x * 0.013, Menu_Size.y * 0.025));


                    sf::FloatRect Bounds = Fon_MaxFPS_down.getLocalBounds();
                    // устанавливаем отчёт от центра фона
                    Fon_MaxFPS_down.setOrigin(Bounds.width, Bounds.height - Bounds.height);  // ширина - высота

                    //размещаем относительно фона цифер который показывает значения
                    Fon_MaxFPS_down.setPosition
                    (
                        Fon_MaxFPS_num.getPosition().x - Fon_MaxFPS_num.getSize().x / 2, // Центр по X
                        Fon_MaxFPS_num.getPosition().y - Fon_MaxFPS_num.getSize().y / 2  // Центр по Y
                    );


                    // настройки текста

                    Bounds = Text_MaxFPS_down.getLocalBounds();
                    float Width = Bounds.width;
                    float targetWidth = Fon_MaxFPS_down.getSize().x * 0.75; // размер относительно фона
                    float scaleX = targetWidth / Width;
                    float targetHeight = Fon_MaxFPS_down.getSize().y * 0.75;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_MaxFPS_down.setScale(scaleX, scaleY); //задаём размер


                    // устанавливаем отчёт от центра текста и задаём позицию

                    Text_MaxFPS_down.setOrigin(Bounds.width / 2, Bounds.height / 2);

                    sf::FloatRect fonBounds = Fon_MaxFPS_down.getGlobalBounds(); // Берём границы фона

                    Text_MaxFPS_down.setPosition(
                        fonBounds.left + fonBounds.width / 3, // Центрируем по X
                        fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                    );

                }
                // >
                {

                    Fon_MaxFPS_up.setSize(sf::Vector2f(Menu_Size.x * 0.013, Menu_Size.y * 0.025));

                    sf::FloatRect Bounds = Fon_MaxFPS_up.getLocalBounds();
                    // устанавливаем отчёт от центра фона
                    Fon_MaxFPS_up.setOrigin(0, Bounds.height);

                    //размещаем относительно фона цифер который показывает значения
                    Fon_MaxFPS_up.setPosition
                    (
                        Fon_MaxFPS_num.getPosition().x + Fon_MaxFPS_num.getSize().x / 2, // Центр по X
                        Fon_MaxFPS_num.getPosition().y + Fon_MaxFPS_num.getSize().y / 2  // Центр по Y
                    );


                    // настройки текста

                    Bounds = Text_MaxFPS_up.getLocalBounds();
                    float Width = Bounds.width;
                    float targetWidth = Fon_MaxFPS_up.getSize().x * 0.75; // размер относительно фона
                    float scaleX = targetWidth / Width;
                    float targetHeight = Fon_MaxFPS_up.getSize().y * 0.75;
                    float Height = Bounds.height;
                    float scaleY = targetHeight / Height;
                    Text_MaxFPS_up.setScale(scaleX, scaleY); //задаём размер


                    // устанавливаем отчёт от центра текста и задаём позицию
                    Text_MaxFPS_up.setOrigin(Bounds.width / 2, Bounds.height / 2);

                    sf::FloatRect fonBounds = Fon_MaxFPS_up.getGlobalBounds(); // Берём границы фона

                    Text_MaxFPS_up.setPosition(
                        fonBounds.left + fonBounds.width / 3, // Центрируем по X
                        fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                    );
                }

            }


            //  Text_FPS_visibility                 
            if (true)
            {
                sf::FloatRect Bounds = Text_FPS_visibility.getLocalBounds();

                float Width = Bounds.width;
                float targetWidth = SettingsPanel.getSize().x * 0.4;
                float scaleX = targetWidth / Width;
                float targetHeight = SettingsPanel.getSize().y * 0.04;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_FPS_visibility.setScale(scaleX, scaleY);

                Text_FPS_visibility.setOrigin(0, Bounds.height / 2);
                Text_FPS_visibility.setPosition
                (
                    SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 9, // Центр по X
                    SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 2.65  // Центр по Y     
                );

            }
            //  Text_FPS_visibility_num   
            {

                // параметры фона 
                Fon_FPS_visibility.setSize(sf::Vector2f(SettingsPanel.getSize().x * 0.080, SettingsPanel.getSize().y * 0.050));

                sf::FloatRect Bounds = Fon_FPS_visibility.getLocalBounds();
                // устанавливаем отчёт от центра фона
                Fon_FPS_visibility.setOrigin(Bounds.width / 2, Bounds.height / 2);
                //размещаем относительно панели настроек
                Fon_FPS_visibility.setPosition
                (
                    SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.38, // Центр по X
                    SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 2.65  // Центр по Y
                );

                // настройки текста
                Bounds = Text_FPS_visibility_num.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = Fon_FPS_visibility.getSize().x * 0.5; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = Fon_FPS_visibility.getSize().y * 0.5;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_FPS_visibility_num.setScale(scaleX, scaleY); //задаём размер


                // устанавливаем отчёт от центра текста и задаём позицию


                Text_FPS_visibility_num.setOrigin(Text_FPS_visibility_num.getLocalBounds().width / 2, Text_FPS_visibility_num.getLocalBounds().height / 2);
                Text_FPS_visibility_num.setPosition
                (
                    SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.38, // Центр по X
                    SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 2.65  // Центр по Y
                );


            }


        }

        // параметры   SavePanel
        if(true)
        {
            ////////  TEXT 
            
             
            //Text_SAVE_1
            if (true)
            {
                // размер текста
                sf::FloatRect Bounds = Text_SAVE_1.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = SavePanel.getSize().x * 0.20;
                float scaleX = targetWidth / Width;
                float targetHeight = SavePanel.getSize().y * 0.06;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_SAVE_1.setScale(scaleX, scaleY);


                Text_SAVE_1.setOrigin(Bounds.width / 2, Bounds.height / 2);
                Text_SAVE_1.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / -9.69697, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -1.85976   // Центр по Y     
                );
            }           
            //Text_SAVE_2
            if (true)
            {
                // размер текста
                sf::FloatRect Bounds = Text_SAVE_2.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = SavePanel.getSize().x * 0.20;
                float scaleX = targetWidth / Width;
                float targetHeight = SavePanel.getSize().y * 0.06;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_SAVE_2.setScale(scaleX, scaleY);


                Text_SAVE_2.setOrigin(Bounds.width / 2, Bounds.height / 2);
                Text_SAVE_2.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 6.66667, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -1.85976  // Центр по Y     
                );
            }
            //Text_SAVE_3
            if (true)
            {
                // размер текста
                sf::FloatRect Bounds = Text_SAVE_3.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = SavePanel.getSize().x * 0.20;
                float scaleX = targetWidth / Width;
                float targetHeight = SavePanel.getSize().y * 0.06;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_SAVE_3.setScale(scaleX, scaleY);


                Text_SAVE_3.setOrigin(Bounds.width / 2, Bounds.height / 2);
                Text_SAVE_3.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 2.48062, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -1.85976   // Центр по Y     
                );
            }
            //Text_SAVE_4
            if (true)
            {
                // размер текста
                sf::FloatRect Bounds = Text_SAVE_4.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = SavePanel.getSize().x * 0.20;
                float scaleX = targetWidth / Width;
                float targetHeight = SavePanel.getSize().y * 0.06;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_SAVE_4.setScale(scaleX, scaleY);


                Text_SAVE_4.setOrigin(Bounds.width / 2, Bounds.height / 2);
                Text_SAVE_4.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / -9.69697, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -4.42029   // Центр по Y     
                );
            }
            //Text_SAVE_5
            if (true)
            {
                // размер текста
                sf::FloatRect Bounds = Text_SAVE_5.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = SavePanel.getSize().x * 0.20;
                float scaleX = targetWidth / Width;
                float targetHeight = SavePanel.getSize().y * 0.06;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_SAVE_5.setScale(scaleX, scaleY);


                Text_SAVE_5.setOrigin(Bounds.width / 2, Bounds.height / 2);
                Text_SAVE_5.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 6.66667, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -4.42029   // Центр по Y     
                );
            }
            //Text_SAVE_6
            if (true)
            {
                // размер текста
                sf::FloatRect Bounds = Text_SAVE_6.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = SavePanel.getSize().x * 0.20;
                float scaleX = targetWidth / Width;
                float targetHeight = SavePanel.getSize().y * 0.06;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_SAVE_6.setScale(scaleX, scaleY);


                Text_SAVE_6.setOrigin(Bounds.width / 2, Bounds.height / 2);
                Text_SAVE_6.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 2.48062, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -4.42029   // Центр по Y     
                );
            }

            ////////  кнопки
            
            // LoadGame_Button_1
            if (true)
            {
                LoadGame_Button_1.setSize(sf::Vector2f(SavePanel.getSize().x * 0.150, SavePanel.getSize().y * 0.070));

                sf::FloatRect Bounds = LoadGame_Button_1.getLocalBounds();
                // устанавливаем отчёт от центра фона
                LoadGame_Button_1.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                LoadGame_Button_1.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / -9.69697, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -2.79817  // Центр по Y
                );
                
                // настройки текста
                Bounds = Text_LoadGame_Button_1.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = LoadGame_Button_1.getSize().x * 0.90; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = LoadGame_Button_1.getSize().y * 0.70;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_LoadGame_Button_1.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_LoadGame_Button_1.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = LoadGame_Button_1.getGlobalBounds(); // Берём границы фона

                Text_LoadGame_Button_1.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }
            // LoadGame_Button_2
            if (true)
            {
                LoadGame_Button_2.setSize(sf::Vector2f(SavePanel.getSize().x * 0.150, SavePanel.getSize().y * 0.070));

                sf::FloatRect Bounds = LoadGame_Button_2.getLocalBounds();
                // устанавливаем отчёт от центра фона
                LoadGame_Button_2.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                LoadGame_Button_2.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 6.66667, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -2.79817  // Центр по Y
                );

                // настройки текста
                Bounds = Text_LoadGame_Button_2.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = LoadGame_Button_2.getSize().x * 0.90; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = LoadGame_Button_2.getSize().y * 0.70;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_LoadGame_Button_2.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_LoadGame_Button_2.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = LoadGame_Button_2.getGlobalBounds(); // Берём границы фона

                Text_LoadGame_Button_2.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }
            // LoadGame_Button_3
            if (true)
            {
                LoadGame_Button_3.setSize(sf::Vector2f(SavePanel.getSize().x * 0.150, SavePanel.getSize().y * 0.070));

                sf::FloatRect Bounds = LoadGame_Button_3.getLocalBounds();
                // устанавливаем отчёт от центра фона
                LoadGame_Button_3.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                LoadGame_Button_3.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 2.48062, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -2.79817  // Центр по Y
                );

                // настройки текста
                Bounds = Text_LoadGame_Button_3.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = LoadGame_Button_3.getSize().x * 0.90; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = LoadGame_Button_3.getSize().y * 0.70;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_LoadGame_Button_3.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_LoadGame_Button_3.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = LoadGame_Button_3.getGlobalBounds(); // Берём границы фона

                Text_LoadGame_Button_3.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }
            // LoadGame_Button_4
            if (true)
            {
                LoadGame_Button_4.setSize(sf::Vector2f(SavePanel.getSize().x * 0.150, SavePanel.getSize().y * 0.070));

                sf::FloatRect Bounds = LoadGame_Button_4.getLocalBounds();
                // устанавливаем отчёт от центра фона
                LoadGame_Button_4.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                LoadGame_Button_4.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / -9.69697, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -21.0345  // Центр по Y
                );

                // настройки текста
                Bounds = Text_LoadGame_Button_4.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = LoadGame_Button_4.getSize().x * 0.90; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = LoadGame_Button_4.getSize().y * 0.70;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_LoadGame_Button_4.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_LoadGame_Button_4.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = LoadGame_Button_4.getGlobalBounds(); // Берём границы фона

                Text_LoadGame_Button_4.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }
            // LoadGame_Button_5
            if (true)
            {
                LoadGame_Button_5.setSize(sf::Vector2f(SavePanel.getSize().x * 0.150, SavePanel.getSize().y * 0.070));

                sf::FloatRect Bounds = LoadGame_Button_5.getLocalBounds();
                // устанавливаем отчёт от центра фона
                LoadGame_Button_5.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                LoadGame_Button_5.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 6.66667, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -21.0345  // Центр по Y
                );

                // настройки текста
                Bounds = Text_LoadGame_Button_5.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = LoadGame_Button_5.getSize().x * 0.90; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = LoadGame_Button_5.getSize().y * 0.70;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_LoadGame_Button_5.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_LoadGame_Button_5.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = LoadGame_Button_5.getGlobalBounds(); // Берём границы фона

                Text_LoadGame_Button_5.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }
            // LoadGame_Button_6
            if (true)
            {
                LoadGame_Button_6.setSize(sf::Vector2f(SavePanel.getSize().x * 0.150, SavePanel.getSize().y * 0.070));

                sf::FloatRect Bounds = LoadGame_Button_6.getLocalBounds();
                // устанавливаем отчёт от центра фона
                LoadGame_Button_6.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                LoadGame_Button_6.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 2.48062, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -21.0345  // Центр по Y
                );

                // настройки текста
                Bounds = Text_LoadGame_Button_6.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = LoadGame_Button_6.getSize().x * 0.90; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = LoadGame_Button_6.getSize().y * 0.70;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_LoadGame_Button_6.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_LoadGame_Button_6.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = LoadGame_Button_6.getGlobalBounds(); // Берём границы фона

                Text_LoadGame_Button_6.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }

            Color_LoadGame_Button();

            //NewGame_Button_1
            if (true)
            {
                
                NewGame_Button_1.setSize(sf::Vector2f(SavePanel.getSize().x * 0.150, SavePanel.getSize().y * 0.070));

                sf::FloatRect Bounds = NewGame_Button_1.getLocalBounds();
                // устанавливаем отчёт от центра фона
                NewGame_Button_1.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                NewGame_Button_1.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / -9.69697, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -2.15548  // Центр по Y
                );

                // настройки текста
                Bounds = Text_NewGame_Button_1.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = NewGame_Button_1.getSize().x * 0.90; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = NewGame_Button_1.getSize().y * 0.70;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_NewGame_Button_1.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_NewGame_Button_1.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = NewGame_Button_1.getGlobalBounds(); // Берём границы фона

                Text_NewGame_Button_1.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }
            //NewGame_Button_2
            if (true)
            {
                NewGame_Button_2.setSize(sf::Vector2f(SavePanel.getSize().x * 0.150, SavePanel.getSize().y * 0.070));

                sf::FloatRect Bounds = NewGame_Button_2.getLocalBounds();
                // устанавливаем отчёт от центра фона
                NewGame_Button_2.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                NewGame_Button_2.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 6.66667, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -2.15548  // Центр по Y
                );

                // настройки текста
                Bounds = Text_NewGame_Button_2.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = NewGame_Button_2.getSize().x * 0.90; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = NewGame_Button_2.getSize().y * 0.70;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_NewGame_Button_2.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_NewGame_Button_2.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = NewGame_Button_2.getGlobalBounds(); // Берём границы фона

                Text_NewGame_Button_2.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }
            //NewGame_Button_3
            if (true)
            {

                NewGame_Button_3.setSize(sf::Vector2f(SavePanel.getSize().x * 0.150, SavePanel.getSize().y * 0.070));

                sf::FloatRect Bounds = NewGame_Button_3.getLocalBounds();
                // устанавливаем отчёт от центра фона
                NewGame_Button_3.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                NewGame_Button_3.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 2.48062, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -2.15548  // Центр по Y
                );

                // настройки текста
                Bounds = Text_NewGame_Button_3.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = NewGame_Button_3.getSize().x * 0.90; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = NewGame_Button_3.getSize().y * 0.70;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_NewGame_Button_3.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_NewGame_Button_3.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = NewGame_Button_3.getGlobalBounds(); // Берём границы фона

                Text_NewGame_Button_3.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }
            //NewGame_Button_4
            if (true)
            {

                NewGame_Button_4.setSize(sf::Vector2f(SavePanel.getSize().x * 0.150, SavePanel.getSize().y * 0.070));

                sf::FloatRect Bounds = NewGame_Button_4.getLocalBounds();
                // устанавливаем отчёт от центра фона
                NewGame_Button_4.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                NewGame_Button_4.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / -9.69697, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -6.55914  // Центр по Y
                );

                // настройки текста
                Bounds = Text_NewGame_Button_4.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = NewGame_Button_4.getSize().x * 0.90; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = NewGame_Button_4.getSize().y * 0.70;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_NewGame_Button_4.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_NewGame_Button_4.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = NewGame_Button_4.getGlobalBounds(); // Берём границы фона

                Text_NewGame_Button_4.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }
            //NewGame_Button_5
            if (true)
            {

                NewGame_Button_5.setSize(sf::Vector2f(SavePanel.getSize().x * 0.150, SavePanel.getSize().y * 0.070));

                sf::FloatRect Bounds = NewGame_Button_5.getLocalBounds();
                // устанавливаем отчёт от центра фона
                NewGame_Button_5.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                NewGame_Button_5.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 6.66667, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -6.55914  // Центр по Y
                );

                // настройки текста
                Bounds = Text_NewGame_Button_5.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = NewGame_Button_5.getSize().x * 0.90; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = NewGame_Button_5.getSize().y * 0.70;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_NewGame_Button_5.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_NewGame_Button_5.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = NewGame_Button_5.getGlobalBounds(); // Берём границы фона

                Text_NewGame_Button_5.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }
            //NewGame_Button_6
            if (true)
            {

                NewGame_Button_6.setSize(sf::Vector2f(SavePanel.getSize().x * 0.150, SavePanel.getSize().y * 0.070));

                sf::FloatRect Bounds = NewGame_Button_6.getLocalBounds();
                // устанавливаем отчёт от центра фона
                NewGame_Button_6.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                NewGame_Button_6.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 2.48062, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -6.55914  // Центр по Y
                );

                // настройки текста
                Bounds = Text_NewGame_Button_6.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = NewGame_Button_6.getSize().x * 0.90; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = NewGame_Button_6.getSize().y * 0.70;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_NewGame_Button_6.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_NewGame_Button_6.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = NewGame_Button_6.getGlobalBounds(); // Берём границы фона

                Text_NewGame_Button_6.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }


            //DelGame_Button_1
            if (true)
            {

                DelGame_Button_1.setSize(sf::Vector2f(SavePanel.getSize().x * 0.051, SavePanel.getSize().y * 0.07));

                sf::FloatRect Bounds = DelGame_Button_1.getLocalBounds();
                // устанавливаем отчёт от центра фона
                DelGame_Button_1.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                DelGame_Button_1.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / -9.69697, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -2.15548  // Центр по Y
                );

                // настройки текста
                Bounds = Text_DelGame_Button_1.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = DelGame_Button_1.getSize().x * 0.50; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = DelGame_Button_1.getSize().y * 0.50;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_DelGame_Button_1.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_DelGame_Button_1.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = DelGame_Button_1.getGlobalBounds(); // Берём границы фона

                Text_DelGame_Button_1.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }
            //DelGame_Button_2
            if (true)
            {

                DelGame_Button_2.setSize(sf::Vector2f(SavePanel.getSize().x * 0.051, SavePanel.getSize().y * 0.07));

                sf::FloatRect Bounds = DelGame_Button_2.getLocalBounds();
                // устанавливаем отчёт от центра фона
                DelGame_Button_2.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                DelGame_Button_2.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 6.66667, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -2.15548  // Центр по Y
                );

                // настройки текста
                Bounds = Text_DelGame_Button_2.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = DelGame_Button_2.getSize().x * 0.50; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = DelGame_Button_2.getSize().y * 0.50;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_DelGame_Button_2.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_DelGame_Button_2.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = DelGame_Button_2.getGlobalBounds(); // Берём границы фона

                Text_DelGame_Button_2.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }
            //DelGame_Button_3
            if (true)
            {

                DelGame_Button_3.setSize(sf::Vector2f(SavePanel.getSize().x * 0.051, SavePanel.getSize().y * 0.07));

                sf::FloatRect Bounds = DelGame_Button_3.getLocalBounds();
                // устанавливаем отчёт от центра фона
                DelGame_Button_3.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                DelGame_Button_3.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 2.48062, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -2.15548  // Центр по Y
                );

                // настройки текста
                Bounds = Text_DelGame_Button_3.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = DelGame_Button_3.getSize().x * 0.50; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = DelGame_Button_3.getSize().y * 0.50;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_DelGame_Button_3.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_DelGame_Button_3.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = DelGame_Button_3.getGlobalBounds(); // Берём границы фона

                Text_DelGame_Button_3.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }          
            //DelGame_Button_4
            if (true)
            {

                DelGame_Button_4.setSize(sf::Vector2f(SavePanel.getSize().x * 0.051, SavePanel.getSize().y * 0.07));

                sf::FloatRect Bounds = DelGame_Button_4.getLocalBounds();
                // устанавливаем отчёт от центра фона
                DelGame_Button_4.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                DelGame_Button_4.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / -9.69697, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -6.63043  // Центр по Y
                );

                // настройки текста
                Bounds = Text_DelGame_Button_4.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = DelGame_Button_4.getSize().x * 0.50; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = DelGame_Button_4.getSize().y * 0.50;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_DelGame_Button_4.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_DelGame_Button_4.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = DelGame_Button_4.getGlobalBounds(); // Берём границы фона

                Text_DelGame_Button_4.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }
            //DelGame_Button_5
            if (true)
            {

                DelGame_Button_5.setSize(sf::Vector2f(SavePanel.getSize().x * 0.051, SavePanel.getSize().y * 0.07));

                sf::FloatRect Bounds = DelGame_Button_5.getLocalBounds();
                // устанавливаем отчёт от центра фона
                DelGame_Button_5.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                DelGame_Button_5.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 6.66667, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -6.63043  // Центр по Y
                );

                // настройки текста
                Bounds = Text_DelGame_Button_5.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = DelGame_Button_5.getSize().x * 0.50; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = DelGame_Button_5.getSize().y * 0.50;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_DelGame_Button_5.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_DelGame_Button_5.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = DelGame_Button_5.getGlobalBounds(); // Берём границы фона

                Text_DelGame_Button_5.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }
            //DelGame_Button_6
            if (true)
            {

                DelGame_Button_6.setSize(sf::Vector2f(SavePanel.getSize().x * 0.051, SavePanel.getSize().y * 0.07));

                sf::FloatRect Bounds = DelGame_Button_6.getLocalBounds();
                // устанавливаем отчёт от центра фона
                DelGame_Button_6.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                DelGame_Button_6.setPosition
                (
                    SavePanel.getPosition().x + SavePanel.getSize().x / 2.48062, // Центр по X
                    SavePanel.getPosition().y + SavePanel.getSize().y / -6.63043  // Центр по Y
                );

                // настройки текста
                Bounds = Text_DelGame_Button_6.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = DelGame_Button_6.getSize().x * 0.50; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = DelGame_Button_6.getSize().y * 0.50;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_DelGame_Button_6.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_DelGame_Button_6.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = DelGame_Button_6.getGlobalBounds(); // Берём границы фона

                Text_DelGame_Button_6.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }

            //////   ConfirmPanel
            
            //Text_DeleteSave
            if (true)
            {
                // размер текста
                sf::FloatRect Bounds = Text_DeleteSave.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = ConfirmPanel.getSize().x * 0.70;
                float scaleX = targetWidth / Width;
                float targetHeight = ConfirmPanel.getSize().y * 0.20;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_DeleteSave.setScale(scaleX, scaleY);


                Text_DeleteSave.setOrigin(Bounds.width / 2, Bounds.height / 2);
                Text_DeleteSave.setPosition
                (
                    ConfirmPanel.getPosition().x + ConfirmPanel.getSize().x / 426.667, // Центр по X
                    ConfirmPanel.getPosition().y + ConfirmPanel.getSize().y / -4.63721   // Центр по Y     
                );
            }
            // Yes_Button
            if (true)
            {
                Yes_Button.setSize(sf::Vector2f(ConfirmPanel.getSize().x * 0.35, ConfirmPanel.getSize().y * 0.25));

                sf::FloatRect Bounds = Yes_Button.getLocalBounds();
                // устанавливаем отчёт от центра фона
                Yes_Button.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                Yes_Button.setPosition
                (
                    ConfirmPanel.getPosition().x + ConfirmPanel.getSize().x / -4.35374, // Центр по X
                    ConfirmPanel.getPosition().y + ConfirmPanel.getSize().y / 6.55601  // Центр по Y
                );

                // настройки текста
                Bounds = Text_Yes_Button.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = Yes_Button.getSize().x * 0.90; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = Yes_Button.getSize().y * 0.70;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_Yes_Button.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_Yes_Button.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = Yes_Button.getGlobalBounds(); // Берём границы фона

                Text_Yes_Button.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }
            // No_Button
            if (true)
            {
                No_Button.setSize(sf::Vector2f(ConfirmPanel.getSize().x * 0.35, ConfirmPanel.getSize().y * 0.25));

                sf::FloatRect Bounds = No_Button.getLocalBounds();
                // устанавливаем отчёт от центра фона
                No_Button.setOrigin((Bounds.width / 2), Bounds.height / 2);
                //размещаем относительно панели настроек
                No_Button.setPosition
                (
                    ConfirmPanel.getPosition().x + ConfirmPanel.getSize().x / 4.30976, // Центр по X
                    ConfirmPanel.getPosition().y + ConfirmPanel.getSize().y / 6.55601  // Центр по Y
                );

                // настройки текста
                Bounds = Text_No_Button.getLocalBounds();
                float Width = Bounds.width;
                float targetWidth = No_Button.getSize().x * 0.90; // размер относительно фона
                float scaleX = targetWidth / Width;
                float targetHeight = No_Button.getSize().y * 0.70;
                float Height = Bounds.height;
                float scaleY = targetHeight / Height;
                Text_No_Button.setScale(scaleX, scaleY); //задаём размер

                // устанавливаем отчёт от центра текста

                Text_No_Button.setOrigin(Bounds.width / 2, Bounds.height / 2);

                sf::FloatRect fonBounds = No_Button.getGlobalBounds(); // Берём границы фона

                Text_No_Button.setPosition(
                    fonBounds.left + fonBounds.width / 2, // Центрируем по X
                    fonBounds.top + fonBounds.height / 2  // Центрируем по Y
                );

            }


        }

        // параметры SaveButton 
        if (true)
        {

            SaveButton.setSize(sf::Vector2f(Menu_Size.x * 0.110, Menu_Size.y * 0.050));

            sf::FloatRect Bounds = SaveButton.getLocalBounds();
            // устанавливаем отчёт от центра фона
            SaveButton.setOrigin(0, Bounds.height / 2);
            //размещаем относительно панели настроек
            SaveButton.setPosition
            (
                SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 9, // Центр по X
                SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.17232  // Центр по Y
            );

            // настройки текста
            Bounds = Text_SaveButton.getLocalBounds();
            float Width = Bounds.width;
            float targetWidth = SaveButton.getSize().x * 0.55; // размер относительно фона
            float scaleX = targetWidth / Width;
            float targetHeight = SaveButton.getSize().y * 0.70;
            float Height = Bounds.height;
            float scaleY = targetHeight / Height;
            Text_SaveButton.setScale(scaleX, scaleY); //задаём размер

            // устанавливаем отчёт от центра текста

            Text_SaveButton.setOrigin(Bounds.width / 2, Bounds.height / 2);

            sf::FloatRect fonBounds = SaveButton.getGlobalBounds(); // Берём границы фона

            Text_SaveButton.setPosition(
                fonBounds.left + fonBounds.width / 2, // Центрируем по X
                fonBounds.top + fonBounds.height / 2  // Центрируем по Y
            );
        }
        // параметры CancelButton 
        if (true)
        {

            CancelButton.setSize(sf::Vector2f(Menu_Size.x * 0.110, Menu_Size.y * 0.050));
          

            sf::FloatRect Bounds = CancelButton.getLocalBounds();
            // устанавливаем отчёт от центра фона
            CancelButton.setOrigin(Bounds.width / 2, Bounds.height / 2);
            //размещаем относительно панели настроек
            CancelButton.setPosition
            (
                SettingsPanel.getPosition().x + SettingsPanel.getSize().x / 1.38, // Центр по X
                SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.17232  // Центр по Y
            );

            // настройки текста
            Bounds = Text_CancelButton.getLocalBounds();
            float Width = Bounds.width;
            float targetWidth = CancelButton.getSize().x * 0.70; // размер относительно фона
            float scaleX = targetWidth / Width;
            float targetHeight = CancelButton.getSize().y * 0.70;
            float Height = Bounds.height;
            float scaleY = targetHeight / Height;
            Text_CancelButton.setScale(scaleX, scaleY); //задаём размер

            // устанавливаем отчёт от центра текста

            Text_CancelButton.setOrigin(Bounds.width / 2, Bounds.height / 2);

            sf::FloatRect fonBounds = CancelButton.getGlobalBounds(); // Берём границы фона

            Text_CancelButton.setPosition(
                fonBounds.left + fonBounds.width / 2, // Центрируем по X
                fonBounds.top + fonBounds.height / 2  // Центрируем по Y
            );
        }


    }
    void Color_LoadGame_Button()
    {
        if (is_Save_1 == false) { LoadGame_Button_1.setFillColor(sf::Color(100, 100, 100)); } else { LoadGame_Button_1.setFillColor(sf::Color(255, 255, 255)); } // добавляем \ Убираем цвет
        if (is_Save_2 == false) { LoadGame_Button_2.setFillColor(sf::Color(100, 100, 100)); } else { LoadGame_Button_2.setFillColor(sf::Color(255, 255, 255)); } 
        if (is_Save_3 == false) { LoadGame_Button_3.setFillColor(sf::Color(100, 100, 100)); } else { LoadGame_Button_3.setFillColor(sf::Color(255, 255, 255)); } 
        if (is_Save_4 == false) { LoadGame_Button_4.setFillColor(sf::Color(100, 100, 100)); } else { LoadGame_Button_4.setFillColor(sf::Color(255, 255, 255)); } 
        if (is_Save_5 == false) { LoadGame_Button_5.setFillColor(sf::Color(100, 100, 100)); } else { LoadGame_Button_5.setFillColor(sf::Color(255, 255, 255)); } 
        if (is_Save_6 == false) { LoadGame_Button_6.setFillColor(sf::Color(100, 100, 100)); } else { LoadGame_Button_6.setFillColor(sf::Color(255, 255, 255)); } 

    }
    // панели \\ обьекты которым задаётся позиция 0, 0,
    bool lod_v = true;
    void UI_menu_load(sf::Vector2u Menu_Size)
    {
        Vignette.setSize(sf::Vector2f(Menu_Size.x, Menu_Size.y)); Vignette.setPosition(0, 0);
        
        Fon_menu.setSize(sf::Vector2f(Menu_Size.x, Menu_Size.y)); Fon_menu.setPosition(0, 0);
        Fon_menu_foreground.setSize(sf::Vector2f(Menu_Size.x, Menu_Size.y)); Fon_menu_foreground.setPosition(0, 0);

        // облака
         {
            Clouds_1.setSize(sf::Vector2f(Menu_Size.x * 0.17708333333, Menu_Size.y * 0.06481481481)); Clouds_1.setPosition(Random_Number(0, Menu_Size.x / 1.1), Random_Number(-10, Menu_Size.y / 2 ) + 100);
            Clouds_2.setSize(sf::Vector2f(Menu_Size.x * 0.21354166666, Menu_Size.y * 0.09259259259)); Clouds_2.setPosition(Random_Number(0, Menu_Size.x / 1.1), Random_Number(-10, Menu_Size.y / 2 ) + 200);
            Clouds_3.setSize(sf::Vector2f(Menu_Size.x * 0.08854166666, Menu_Size.y * 0.08333333333)); Clouds_3.setPosition(Random_Number(0, Menu_Size.x / 1.1), Random_Number(-10, Menu_Size.y / 2 ) - 200);
            Clouds_4.setSize(sf::Vector2f(1500,100));                                                  Clouds_4.setPosition(Random_Number(0, Menu_Size.x / 1.1), Random_Number(-10, Menu_Size.y / 2));
         }
         
                   SettingsPanel.setSize(sf::Vector2f(Menu_Size.x * 0.35, Menu_Size.y * 0.9));
                   SettingsPanel.setPosition(Menu_Size.x - SettingsPanel.getSize().x, (Menu_Size.y - SettingsPanel.getSize().y) / 2);
                   // доп панели настроек
                   {
                       SettingsPanel_Global.setSize(sf::Vector2f(Menu_Size.x * 0.0625, Menu_Size.y * 0.27777777777));
                       SettingsPanel_Global.setPosition
                       (
                           SettingsPanel.getPosition().x + SettingsPanel.getSize().x / -5.63133, // -5.63133 открытая позиция
                           SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 14.8326  
                       );
                       

                       SettingsPanel_Graphics.setSize(sf::Vector2f(Menu_Size.x * 0.0625, Menu_Size.y * 0.27777777777));
                       SettingsPanel_Graphics.setPosition
                       (
                           SettingsPanel.getPosition().x + SettingsPanel.getSize().x / -10.4161,  // -10.4161 закрыт позиция
                           SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 2.73034 
                       );
                       SettingsPanel_Graphics.setFillColor(sf::Color(255, 255, 255));
                      
                       if(lod_v == true)
                       {
                           Open_Pos = SettingsPanel_Global.getPosition().x;
                           Hide_Pos = SettingsPanel_Graphics.getPosition().x;
                           lod_v = false;
                       }

                       SettingsPanel_Controls.setSize(sf::Vector2f(Menu_Size.x * 0.0625, Menu_Size.y * 0.23888888888));
                       SettingsPanel_Controls.setPosition
                       (
                           SettingsPanel.getPosition().x + SettingsPanel.getSize().x / -10.4161,
                           SettingsPanel.getPosition().y + SettingsPanel.getSize().y / 1.50392
                       );
                       SettingsPanel_Controls.setFillColor(sf::Color(255, 255, 255));

                       
                   }

                   // панель сохранения и всё что связанно
                   {
                       SavePanel.setSize(sf::Vector2f(Menu_Size.x * 0.5, Menu_Size.y * 0.56481481481));
                       SavePanel.setOrigin(SettingsPanel.getLocalBounds().width / 2, SettingsPanel.getLocalBounds().height / 2);
                       SavePanel.setPosition(Menu_Size.x / 2, (Menu_Size.y / 2));

                       ConfirmPanel.setSize(sf::Vector2f(Menu_Size.x * 0.22222222222, Menu_Size.y * 0.17604166666));
                       ConfirmPanel.setOrigin(ConfirmPanel.getSize().x / 2, ConfirmPanel.getSize().y / 2);
                       ConfirmPanel.setPosition
                       (
                           SavePanel.getPosition().x + SavePanel.getSize().x / 6.95652, // Центр по X
                           SavePanel.getPosition().y + SavePanel.getSize().y / -3.52601  // Центр по Y
                       );
                   }


                   
                   
    }

    
    // настройки громкости звуков 
    void Settings_FX()
    {
        Confirm_play.setVolume(i_Sound_FX_num);
        Confirm_settings.setVolume(i_Sound_FX_num);
        Confirm_SettingsButton.setVolume(i_Sound_FX_num);

            Move_batton_1.setVolume(i_Sound_FX_num);
            Move_batton_2.setVolume(i_Sound_FX_num);
            Move_batton_3.setVolume(i_Sound_FX_num);

            Move_batton_down.setVolume(i_Sound_FX_num);
            Move_batton_up.setVolume(i_Sound_FX_num);
            Lenguage_select.setVolume(i_Sound_FX_num);

            CH_Sound.setVolume(i_Sound_FX_num);
            NewGame_Sound.setVolume(i_Sound_FX_num);
            LoadGame_Sound.setVolume(i_Sound_FX_num);
            DelGame_Sound.setVolume(i_Sound_FX_num);
            DelGame_Yes_Sound.setVolume(i_Sound_FX_num);
            DelGame_No_Sound.setVolume(i_Sound_FX_num);


    }

    // настройки громкости музыки
    void Settings_music()
    {
        // Устанавливаем громкость
        Menu_music_1.setVolume(i_Music_num);    
        Load_sound.setVolume(i_Music_num);
    }

//////////////////////////    загрузка файлов   //////////////////////////////




    //////////////////      загрузка текстур     ////
    void UI_load_texture()
    {
        // текстуры кнопок
        if (true) 
        {
            // текстуры кнопок меню
            if (!PlayButton_Texture.loadFromFile(Path_directory_game + "Texture\\Menu\\Button.png"))
            {
                cout << "Ошибка - Button.png" << endl;
                cout << "путь:" << Path_directory_game << endl;
            }

            PlayButton.setTexture(&PlayButton_Texture);
            SettingsButton.setTexture(&PlayButton_Texture);
            ExitButton.setTexture(&PlayButton_Texture);
            LoadgameButton.setTexture(&PlayButton_Texture);

            // текстуры кнопок сохранить и отменить
            if (!Button_S_Texture.loadFromFile(Path_directory_game + "Texture\\Menu\\Button_S.png"))
            {
                cout << "Ошибка - Button_S.png" << endl;
                cout << "путь:" << Path_directory_game << endl;
            }

            SaveButton.setTexture(&Button_S_Texture);
            CancelButton.setTexture(&Button_S_Texture);

            LoadGame_Button_1.setTexture(&Button_S_Texture);
            LoadGame_Button_2.setTexture(&Button_S_Texture);
            LoadGame_Button_3.setTexture(&Button_S_Texture);
            LoadGame_Button_4.setTexture(&Button_S_Texture);
            LoadGame_Button_5.setTexture(&Button_S_Texture);
            LoadGame_Button_6.setTexture(&Button_S_Texture);

            NewGame_Button_1.setTexture(&Button_S_Texture);
            NewGame_Button_2.setTexture(&Button_S_Texture);
            NewGame_Button_3.setTexture(&Button_S_Texture);
            NewGame_Button_4.setTexture(&Button_S_Texture);
            NewGame_Button_5.setTexture(&Button_S_Texture);
            NewGame_Button_6.setTexture(&Button_S_Texture);

            Yes_Button.setTexture(&Button_S_Texture);
            No_Button.setTexture(&Button_S_Texture);


            // текстура переключателей
            if (!Button_X_Texture.loadFromFile(Path_directory_game + "Texture\\Menu\\Button_X.png"))
            {
                cout << "Ошибка - Button_X.png" << endl;
                cout << "путь:" << Path_directory_game << endl;
            }
            DelGame_Button_1.setTexture(&Button_X_Texture);
            DelGame_Button_2.setTexture(&Button_X_Texture);
            DelGame_Button_3.setTexture(&Button_X_Texture);
            DelGame_Button_4.setTexture(&Button_X_Texture);
            DelGame_Button_5.setTexture(&Button_X_Texture);
            DelGame_Button_6.setTexture(&Button_X_Texture);

            Fon_Vsync.setTexture(&Button_X_Texture);
            Fon_FPS_visibility.setTexture(&Button_X_Texture);
            Fon_Fullscreen.setTexture(&Button_X_Texture);
            Fon_Sound_FX_down.setTexture(&Button_X_Texture);
            Fon_Sound_FX_up.setTexture(&Button_X_Texture);
            Fon_Music_down.setTexture(&Button_X_Texture);
            Fon_Music_up.setTexture(&Button_X_Texture);
            Fon_Vignette_down.setTexture(&Button_X_Texture);
            Fon_Vignette_up.setTexture(&Button_X_Texture);
            Fon_Resolution_down.setTexture(&Button_X_Texture);
            Fon_Resolution_up.setTexture(&Button_X_Texture);

            Fon_MaxFPS_down.setTexture(&Button_X_Texture);
            Fon_MaxFPS_up.setTexture(&Button_X_Texture);

            if (!Button_NUM_Texture.loadFromFile(Path_directory_game + "Texture\\Menu\\Button_NUM.png"))
            {
                cout << "Ошибка - Button_NUM.png" << endl;
                cout << "путь:" << Path_directory_game << endl;
            }
            Fon_Sound_FX_num.setTexture(&Button_NUM_Texture);
            Fon_Vignette_num.setTexture(&Button_NUM_Texture);
            Fon_Music_num.setTexture(&Button_NUM_Texture);
            Fon_Resolution_num.setTexture(&Button_NUM_Texture);

            Fon_MaxFPS_num.setTexture(&Button_NUM_Texture);


        }

        if (!EmptyTexture.loadFromFile(Path_directory_game + "Texture\\EmptyTexture.png"))
        {
            cout << "Ошибка - EmptyTexture.png" << endl;
            cout << "путь:" << Path_directory_game << endl;
        }


        // текстура фон меню
        if (!Fon_menu_Texture.loadFromFile(Path_directory_game + "Texture\\Menu\\Fon_menu.png"))
        {
            cout << "Ошибка - Fon_menu.png" << endl;
            cout << "путь:" << Path_directory_game << endl;
        }
        Fon_menu.setTexture(&Fon_menu_Texture);
        if (!Fon_menu_Foreground.loadFromFile(Path_directory_game + "Texture\\Menu\\Fon_menu_Foreground.png"))
        {
            cout << "Ошибка - Fon_menu_Foreground.png" << endl;
            cout << "путь:" << Path_directory_game << endl;
        }
        Fon_menu_foreground.setTexture(&Fon_menu_Foreground);
        
        // текстура флагов стран
        {

            std::string path = Path_directory_game + "Texture\\Menu\\Language\\";

            if (!ENG_LenguageButton_Texture.loadFromFile(path + "ENG.png")) {
                std::cout << "Ошибка - ENG.png\n";            
            }
            ENG_LenguageButton.setTexture(&ENG_LenguageButton_Texture);

            if (!RUS_LenguageButton_Texture.loadFromFile(path + "RUS.png")) {
                std::cout << "Ошибка - RUS.png\n";
            }
            RUS_LenguageButton.setTexture(&RUS_LenguageButton_Texture);

            if (!TUR_LenguageButton_Texture.loadFromFile(path + "TUR.png")) {
                std::cout << "Ошибка - TUR.png\n";
               
            }
            TUR_LenguageButton.setTexture(&TUR_LenguageButton_Texture);

            if (!UKR_LenguageButton_Texture.loadFromFile(path + "UKR.png")) {
                std::cout << "Ошибка - UKR.png\n";
                
            }
            UKR_LenguageButton.setTexture(&UKR_LenguageButton_Texture);

            if (!SPA_LenguageButton_Texture.loadFromFile(path + "SPA.png")) {
                std::cout << "Ошибка - SPA.png\n";
                
            }
            SPA_LenguageButton.setTexture(&SPA_LenguageButton_Texture);

            if (!POR_LenguageButton_Texture.loadFromFile(path + "POR.png")) {
                std::cout << "Ошибка - POR.png\n";
                
            }
            POR_LenguageButton.setTexture(&POR_LenguageButton_Texture);

            if (!POL_LenguageButton_Texture.loadFromFile(path + "POL.png")) {
                std::cout << "Ошибка - POL.png\n";
                
            }
            POL_LenguageButton.setTexture(&POL_LenguageButton_Texture);

            if (!KOR_LenguageButton_Texture.loadFromFile(path + "KOR.png")) {
                std::cout << "Ошибка - KOR.png\n";
               
            }
            KOR_LenguageButton.setTexture(&KOR_LenguageButton_Texture);

            if (!JPN_LenguageButton_Texture.loadFromFile(path + "JPN.png")) {
                std::cout << "Ошибка - JPN.png\n";
               
            }
            JPN_LenguageButton.setTexture(&JPN_LenguageButton_Texture);

            if (!FRA_LenguageButton_Texture.loadFromFile(path + "FRA.png")) {
                std::cout << "Ошибка - FRA.png\n";
                
            }
            FRA_LenguageButton.setTexture(&FRA_LenguageButton_Texture);

            if (!DEU_LenguageButton_Texture.loadFromFile(path + "DEU.png")) {
                std::cout << "Ошибка - DEU.png\n";
                
            }
            DEU_LenguageButton.setTexture(&DEU_LenguageButton_Texture);

            if (!CHS_LenguageButton_Texture.loadFromFile(path + "CHS.png")) {
                std::cout << "Ошибка - CHS.png\n";
               
            }
            CHS_LenguageButton.setTexture(&CHS_LenguageButton_Texture);
        }


        // текстура облака меню    load_frame <= 6   изменить цифру если больше текстур
            sf::Texture CloudsTexture;
            for (int load_frame = 1; load_frame <= 6; ++load_frame)
            {

                if (!CloudsTexture.loadFromFile(Path_directory_game + "Texture\\Clouds\\Clouds_" + to_string(load_frame) + ".png"))
                {
                    cout << "Ошибка -  Clouds_" + to_string(load_frame) + ".png" << "\n";
                }
                MASIV_Clouds_Texture.push_back(CloudsTexture);
            }

Clouds_1.setTexture(&MASIV_Clouds_Texture[0]);  
Clouds_2.setTexture(&MASIV_Clouds_Texture[1]);
Clouds_3.setTexture(&MASIV_Clouds_Texture[2]); 
Clouds_4.setTexture(&EmptyTexture);
   

         
         sf::Texture CursorTexture;
         for (int load_frame = 1; load_frame <= 1; ++load_frame)
         {
             if (!CursorTexture.loadFromFile(Path_directory_game + "Texture\\Cursor\\Cursor_" + to_string(load_frame) + ".png"))
             {
                 cout << "Ошибка -  Cursor_" + to_string(load_frame) + ".png" << "\n";
             }
             MASIV_Cursor_Texture.push_back(CursorTexture);
         }
        Cursor.setTexture(&MASIV_Cursor_Texture[0]);
  




        // текстура виньетки
        if (!Vignette_Texture.loadFromFile(Path_directory_game + "Texture\\Vignette.png"))
        {
            cout << "Ошибка - Vignette.png" << endl;
            cout << "путь:" << Path_directory_game << endl;
        }
        Vignette.setTexture(&Vignette_Texture);

        // текстуры панели 
        if (!SettingsPanel_Texture.loadFromFile(Path_directory_game + "Texture\\Menu\\Setting_panel.png"))
        {
            cout << "Ошибка - Setting_panel.png" << endl;
            cout << "путь:" << Path_directory_game << endl;
        }
        SettingsPanel.setTexture(&SettingsPanel_Texture);

        if (!SavePanel_Texture.loadFromFile(Path_directory_game + "Texture\\Menu\\Save_panel.png"))
        {
            cout << "Ошибка - Save_panel.png" << endl;
            cout << "путь:" << Path_directory_game << endl;
        }
        SavePanel.setTexture(&SavePanel_Texture);

        if (!ConfirmPanel_Texture.loadFromFile(Path_directory_game + "Texture\\Menu\\Confirm_panel.png"))
        {
            cout << "Ошибка - Confirm_panel.png" << endl;
            cout << "путь:" << Path_directory_game << endl;
        }
        ConfirmPanel.setTexture(&ConfirmPanel_Texture);


        // текстуры доп панелей 
        if (!SettingsPanel_Global_Texture.loadFromFile(Path_directory_game + "Texture\\Menu\\Setting_panel_Global.png"))
        {
            cout << "Ошибка - Setting_panel_Global.png" << endl;
            cout << "путь:" << Path_directory_game << endl;
        }
        SettingsPanel_Global.setTexture(&SettingsPanel_Global_Texture);

        if (!SettingsPanel_Graphics_Texture.loadFromFile(Path_directory_game + "Texture\\Menu\\Setting_panel_Graphics.png"))
        {
            cout << "Ошибка - Setting_panel_Graphics.png" << endl;
            cout << "путь:" << Path_directory_game << endl;
        }
        SettingsPanel_Graphics.setTexture(&SettingsPanel_Graphics_Texture);
        
        if (!SettingsPanel_Controls_Texture.loadFromFile(Path_directory_game + "Texture\\Menu\\Setting_panel_Controls.png"))
        {
            cout << "Ошибка - SettingsPanel_Controls.png" << endl;
            cout << "путь:" << Path_directory_game << endl;
        }
        SettingsPanel_Controls.setTexture(&SettingsPanel_Controls_Texture);

        

    }







    //////////////////        загрузка звуков        ////
    sf::Music Menu_music_1;
    sf::Sound Load_sound;  sf::SoundBuffer Load_sound_buf;
    
        
    sf::Sound Confirm_play;            sf::SoundBuffer Confirm_play_buf;
    sf::Sound Confirm_SettingsButton;  sf::SoundBuffer Confirm_SettingsButton_buf;

    sf::Sound Confirm_settings;   sf::SoundBuffer Confirm_settings_buf;
    sf::Sound Lenguage_select;    sf::SoundBuffer Lenguage_select_buf;

         sf::Sound Move_batton_1;  sf::SoundBuffer Move_batton_1_buf;
         sf::Sound Move_batton_2;  sf::SoundBuffer Move_batton_2_buf;
         sf::Sound Move_batton_3;  sf::SoundBuffer Move_batton_3_buf;

         sf::Sound Move_batton_down;  sf::SoundBuffer Move_batton_down_buf;
         sf::Sound Move_batton_up;    sf::SoundBuffer Move_batton_up_buf;

         sf::Sound CH_Sound;  sf::SoundBuffer CH_Sound_buf;

         sf::Sound NewGame_Sound;  sf::SoundBuffer NewGame_Sound_buf;
         sf::Sound LoadGame_Sound;  sf::SoundBuffer LoadGame_Sound_buf;
         sf::Sound DelGame_Sound;  sf::SoundBuffer DelGame_Sound_buf;

         sf::Sound DelGame_Yes_Sound;  sf::SoundBuffer DelGame_Yes_Sound_buf;
         sf::Sound DelGame_No_Sound;  sf::SoundBuffer DelGame_No_Sound_buf;

        
    void load_sounds()
    {
        // в основном музыка
        if (true) 
        {
            if (!Menu_music_1.openFromFile(Path_directory_game + "Music\\Menu_music_1.wav"))
            {
                std::cout << "Ошибка загрузки музыки!" << std::endl;
                return;
            }
            Menu_music_1.setLoop(true);    // Зацикливаем музыку

            if (!Load_sound_buf.loadFromFile(Path_directory_game + "Music\\Load_sound.wav"))
            {
                cout << "Ошибка загрузки звука:Load_sound.wav" << endl;
            }
            Load_sound.setBuffer(Load_sound_buf);  // лого саунд
        }

            // нажатие кнопок
        if (true) {
            if (!Confirm_play_buf.loadFromFile(Path_directory_game + "Sounds\\Confirm_play.wav"))
            {
                cout << "Ошибка загрузки звука:Confirm_play.wav" << endl;
            }
            if (!Confirm_settings_buf.loadFromFile(Path_directory_game + "Sounds\\Confirm_settings.wav"))
            {
                cout << "Ошибка загрузки звука:Confirm_settings.wav" << endl;
            }
            if (!Confirm_SettingsButton_buf.loadFromFile(Path_directory_game + "Sounds\\Confirm_SettingsButton.wav"))
            {
                cout << "Ошибка загрузки звука:Confirm_SettingsButton.wav" << endl;
            }

            Confirm_play.setBuffer(Confirm_play_buf);
            Confirm_settings.setBuffer(Confirm_settings_buf);
            Confirm_SettingsButton.setBuffer(Confirm_SettingsButton_buf);


            if (!Lenguage_select_buf.loadFromFile(Path_directory_game + "Sounds\\Lenguage_select.wav"))
            {
                cout << "Ошибка загрузки звука:Lenguage_select.wav" << endl;
            }
            Lenguage_select.setBuffer(Lenguage_select_buf);

            if (!NewGame_Sound_buf.loadFromFile(Path_directory_game + "Sounds\\NewGame.wav"))
            {
                cout << "Ошибка загрузки звука:NewGame.wav" << endl;
            }
            if (!LoadGame_Sound_buf.loadFromFile(Path_directory_game + "Sounds\\LoadGame.wav"))
            {
                cout << "Ошибка загрузки звука:LoadGame.wav" << endl;
            }
            if (!DelGame_Sound_buf.loadFromFile(Path_directory_game + "Sounds\\DelGame.wav"))
            {
                cout << "Ошибка загрузки звука:DelGame.wav" << endl;
            }
            if (!DelGame_Yes_Sound_buf.loadFromFile(Path_directory_game + "Sounds\\DelGame_Yes.wav"))
            {
                cout << "Ошибка загрузки звука:DelGame_Yes.wav" << endl;
            }
            if (!DelGame_No_Sound_buf.loadFromFile(Path_directory_game + "Sounds\\DelGame_No.wav"))
            {
                cout << "Ошибка загрузки звука:DelGame_No.wav" << endl;
            }

            NewGame_Sound.setBuffer(NewGame_Sound_buf);
            LoadGame_Sound.setBuffer(LoadGame_Sound_buf);
            DelGame_Sound.setBuffer(DelGame_Sound_buf);

            DelGame_Yes_Sound.setBuffer(DelGame_Yes_Sound_buf);
            DelGame_No_Sound.setBuffer(DelGame_No_Sound_buf);
        }

            // перемещение по кнопкам
        if (true) 
        {
            if (!Move_batton_1_buf.loadFromFile(Path_directory_game + "Sounds\\Move_batton_1.wav"))
            {
                cout << "Ошибка загрузки звука:Move_batton_1.wav" << endl;
            }
            if (!Move_batton_2_buf.loadFromFile(Path_directory_game + "Sounds\\Move_batton_2.wav"))
            {
                cout << "Ошибка загрузки звука:Move_batton_2.wav" << endl;
            }
            if (!Move_batton_3_buf.loadFromFile(Path_directory_game + "Sounds\\Move_batton_3.wav"))
            {
                cout << "Ошибка загрузки звука:Move_batton_3.wav" << endl;
            }

            Move_batton_1.setBuffer(Move_batton_1_buf);
            Move_batton_2.setBuffer(Move_batton_2_buf);
            Move_batton_3.setBuffer(Move_batton_3_buf);

            if (!Move_batton_down_buf.loadFromFile(Path_directory_game + "Sounds\\Move_batton_down.wav"))
            {
                cout << "Ошибка загрузки звука:Move_batton_down.wav" << endl;
            }
            if (!Move_batton_up_buf.loadFromFile(Path_directory_game + "Sounds\\Move_batton_up.wav"))
            {
                cout << "Ошибка загрузки звука:Move_batton_up.wav" << endl;
            }

            Move_batton_down.setBuffer(Move_batton_down_buf);
            Move_batton_up.setBuffer(Move_batton_up_buf);

            if (!CH_Sound_buf.loadFromFile(Path_directory_game + "Sounds\\CH_Sound.wav"))
            {
                cout << "Ошибка загрузки звука:CH_Sound.wav" << endl;
            }
            CH_Sound.setBuffer(CH_Sound_buf);
        }
    }



    
        

    
    
//////////////////////////////
    
};