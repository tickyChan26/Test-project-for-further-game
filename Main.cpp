
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
#include "tinyxml2.h"

#pragma comment(lib, "Shell32.lib")


#include "Menu.cpp"
#include "SaveData.cpp"
#include "Gamelocation.cpp"
#include "GameControl.cpp"
#include "Game_UI.cpp"


using namespace std;




// отдача //
string Path_directory_game;// путь в директорию
sf::RenderWindow window;

static bool Refresh_VSYNC = false;  
static bool Refresh_MAXFPS = false; static int MAXFPS = 100;

static bool Load_Game_Data = false;
static bool Load_Menu_Data = false;

// приём //
extern bool startGame;
extern bool press_NewGame;
extern bool isESC_press;
extern sf::RectangleShape Cursor;
extern sf::Text FpsText;
extern bool VSYNC; 
extern bool Move_Left_Press, Move_Right_Press, Move_Up_Press, Move_Down_Press;

extern sf::Text Text_MaxFPS_num;

int resolutionX, resolutionY, i_Sound_FX_num = 100, i_Music_num = 100, i_Vignette_num = 100, i_Resolution_num, Lenguage = 0;
extern bool FPS_ON;



////////     воиды и переменные    ////////


sf::Vector2u Win_size;  // размер игры
bool Fullscreen;



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
                
                if (key == "ResolutionX")      resolutionX = stoi(value);
                else if (key == "ResolutionY") resolutionY = stoi(value);
                else if (key == "Fullscreen")  i_Resolution_num = stoi(value);
                else if (key == "SoundVolume") i_Sound_FX_num = stoi(value);
                else if (key == "MusicVolume") i_Music_num = stoi(value);
                else if (key == "Vignette") i_Vignette_num = stoi(value);
                else if (key == "Lenguage") Lenguage = stoi(value);
                else if (key == "Vsync") VSYNC = stoi(value);
                else if (key == "MaxFPS") MAXFPS = stoi(value);
                else if (key == "FPS_ON") FPS_ON = stoi(value);
            }
        }
    }


    if (i_Resolution_num == 0) { Fullscreen = true; }
    else if (i_Resolution_num != 0) { Fullscreen = false; }
    
    
    file.close();

}

void settings_window(sf::RenderWindow& window)
{
    if(Fullscreen == false)
    {
        Win_size.x = resolutionX;
        Win_size.y = resolutionY;

        window.create(sf::VideoMode(Win_size.x, Win_size.y), "Mushroom vs Plumber", sf::Style::Default);
    }

    if (Fullscreen == true)
    {
        window.create(sf::VideoMode::getDesktopMode(), "Mushroom vs Plumber", sf::Style::Fullscreen); 
    }

    
    if (VSYNC == true) { window.setVerticalSyncEnabled(true); Text_MaxFPS_num.setFillColor(sf::Color::Red); }
    else if (VSYNC == false) { window.setVerticalSyncEnabled(false); window.setFramerateLimit(MAXFPS); Text_MaxFPS_num.setFillColor(sf::Color::Black);}
  
    window.setMouseCursorVisible(false);
}

sf::Clock Clock_Timer;
sf::Clock Clock_Time; 
sf::Clock Clock_Load_Locatoin;

const float FixDeltaTime = 1.0f / 244.0f; // Примерно 0.0041 секунды на кадр
float Accumulator = 0.0f;


////////////  основное выполнение всего   ////////////
int main(int argc, char** argv)
{
    setlocale(LC_ALL, "RU");
    //Path_directory_game
    {
        std::filesystem::path Path_directory = std::filesystem::canonical(std::filesystem::path(argv[0])).remove_filename();
        Path_directory_game = Path_directory.string(); // Записываем путь
        cout << "\nPath_directory_game: " << Path_directory_game << " [ путь в директорию игры  ,Main.cpp, ]\n\n";
    }
    
    loadSettings(); //загружаем настройки 
    settings_window(window); //задаём настройки окна
  
    Menu menu(window);           //     Всё меню, текстуры, действия, анимации настройки, всё тут.   (   menu.MENU_STARDANT_LOAD();   )

    SaveData savedata(window);       savedata.SAVEDATA_STARDANT_LOAD(); //отвечает за сохранение игры во время игрового про

    Game_UI game_ui(window);           game_ui.GAME_UI_STARDANT_LOAD(); //интерфейс во время игры, его отображение и функционал

    GameLocation gamelocation(window);    gamelocation.GAMELOCATION_STARDANT_LOAD(); //лоация игры,отображение мобов, игрока  и т.п.

    GameControl gamecontrol(window);        gamecontrol.GAMECONTROL_STARDANT_LOAD(); //управление, физика, логика управление игроком

    
 


    int One_load = 0, lod = 0;
    while (window.isOpen()) // Главный цикл
    {
        float deltaTime = Clock_Time.restart().asSeconds(); // Время между кадрами      
        if (deltaTime > 0.04f) { deltaTime = 0.04f; }  Accumulator += deltaTime; /* Ограничиваем `deltaTime`, если игра лаганула */
        sf::Event event;


        window.clear();
        

            //////////////// обработка меню //////////////// (при запуске игры или когда нажата кнопки Exit в игре)
            if (startGame == false)
            {
               
                //Загрузка  доп параметров
                if (Load_Menu_Data == false)
                {
                    Load_Menu_Data = true; Load_Game_Data = false;

                    menu.MENU_STARDANT_LOAD();

                    menu.Load_Additional_Settings_Menu();
                }

               
                // Обрабатываем события             
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    { // Если игрок закрыл окно
                        window.close();
                    }

                    if (event.type == sf::Event::MouseMoved)
                    {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        Cursor.setPosition(mousePos.x, mousePos.y);
                    }

                    menu.handleEvents_menu(event);
                    menu.moveEvents(event);
                   
                }

                // запуск музыки
                if (One_load == 0)
                {
                    if (Clock_Timer.getElapsedTime().asSeconds() >= 3) // Ждём 3 секунд
                    {
                        One_load = 1;
                        menu.play_music();
                    }
                }

    
                // Фиксированные обновления логики
                while (Accumulator >= FixDeltaTime)
                {
                    menu.update(FixDeltaTime);

                    Accumulator -= FixDeltaTime;
                }


                // Рендер
                {
                    menu.render(); // Отрисовываем меню
                }

                //если нажата кнопка НОВАЯ ИГРА
                if (press_NewGame == true)
                {
                    cout << "> NewGame Процесс...\n";
                    savedata.Detect_ID_Saves();
                    savedata.Load_Saves_Data();
                    press_NewGame = false;
                    cout << "Новый сейв создан \n";
                }

            }
   
            //////////////// обработка игры //////////////// (Нажата кнопка Play)
            if (startGame == true)
            {
               

                    //Загрузка сохранения \ доп параметров
                if (Load_Game_Data == false)
                {
                    
                    
                    game_ui.Load_Additional_Settings_UI();

                    cout << ">>  Загрузка сохранения\n";
                    savedata.Detect_ID_Saves();
                    savedata.Load_Saves_Data();
                    menu.saveSettings_Last_Save_ID(); // сохраняем ID сохранение как последнее загруженное
                    cout << ">>  Загрузка сейва завершена\n\n";

                    gamelocation.Load_Texture();
                    game_ui.Load_Texture();
                    
                    cout << ">>  Загрузка текстур завершена\n\n";

                    
                    gamelocation.Loading_Unit_Parameters();
                    cout << ">> Loading_Unit_Parameters завершена\n\n";


                    gamelocation.load_Vectors();


                    Load_Game_Data = true; Load_Menu_Data = false;
                }


                // Обрабатываем события
                while (window.pollEvent(event))
                {
                    gamecontrol.Processing_Key(event);

                    game_ui.handleEvents_UI(event);

                    if (event.type == sf::Event::KeyPressed &&
                        (event.key.code == sf::Keyboard::Escape))
                    {

                        Move_Left_Press = false; Move_Right_Press = false; Move_Up_Press = false; Move_Down_Press = false;

                        game_ui.Resize();
                        if (isESC_press == true) { isESC_press = false; }
                        else if (isESC_press == false) { isESC_press = true; }
                    }


                    //  //  // Временный код  //  //  // 
                    if (event.key.code == sf::Keyboard::RBracket) // Правая квадратная скобка
                    {
                        gamelocation.ClearPlatforms();
                        
                    }
                    else if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        game_ui.chekpositioninmausetext();
                    }
                    //  //  //  //  //  //  //  //  // 
                }

                // Фиксированные обновления логики
                while (Accumulator >= FixDeltaTime)
                {
                    
                    game_ui.Update();
                    gamecontrol.Update(FixDeltaTime);  
                    
                    Accumulator -= FixDeltaTime;
                }

                //анимация если нет паузы
                if (isESC_press == false) 
                { 
                    gamelocation.Animation_Update();
                }

                // Рендер
                {
                    gamelocation.Render();
                    game_ui.Render();
                }

                //если запускает сохранение игры (авто сохранение или сохранение в ручную)
                if (false)
                {
                    //снало берем id что бы узнать какой сейв сохранять

                    // потом сохраняем
                }

            }

            

            //обработка общих настроек
            {
                if (Refresh_VSYNC == true) 
                {
                    if (VSYNC == true) { window.setVerticalSyncEnabled(true); cout << "vsync = true\n"; window.setFramerateLimit(0); }
                    else if (VSYNC == false) { window.setVerticalSyncEnabled(false); cout << "vsync = false\n"; window.setFramerateLimit(MAXFPS);}        
                    Refresh_VSYNC = false;
                }

                if (Refresh_MAXFPS == true && VSYNC == false)
                {             
                    Refresh_MAXFPS = false;
                    window.setFramerateLimit(MAXFPS);
                }

                if (FPS_ON == true)
                {
                    game_ui.FPS();
                    window.draw(FpsText);
                }

            }

 
            window.draw(Cursor);

            window.display();
    }


    return 0;
}


