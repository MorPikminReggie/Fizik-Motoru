#include <iostream>
#include <cstdlib>
#include <windows.h>
using namespace std;

int genislik, yukseklik, uzunluk;
bool gameLoop = true;

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
void ClearScreen()
{
    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

class Ekran {
public:
    Ekran(int x, int y) {
        ShowConsoleCursor(false);
        screenx = x;
        screeny = y;
        for (int i = 0; i < screeny; i++) {
            for (int j = 0; j < screenx; j++) {
                if (j == 0 || j == screenx - 1) {
                    sScreen[j][i] = 'I';
                }
                else if (i == 0 || i == screeny - 1) {
                    sScreen[j][i] = '-';
                }
                else {
                    sScreen[j][i] = ' ';
                }
                dScreen[j][i] = ' ';
            }
            cout << endl;
        }
    }

    void sAddChar(int x, int y, char c) {
        if (x != 0 && x != screenx - 1 && y != 0 && y != screeny - 1) {
            sScreen[x][y] = c;
        }
    }
    void dAddChar(int x, int y, char c) {
        if (x != 0 && x != screenx - 1 && y != 0 && y != screeny - 1) {
            dScreen[x][y] = c;
        }
    }
    void moveChar(int x, int y, char c, int r, int l) {
        if (x != 0 && x != screenx - 1 && y != 0 && y != screeny - 1 && x + r != 0 && x + r != screenx - 1 && y + l != 0 && y + l != screeny - 1) {
            dScreen[x][y] = ' ';
            dScreen[x + r][y + l] = c;
        }
    }

    void drawScreen() {
        x = "";
        for (int i = 0; i < screeny; i++) {
            for (int j = 0; j < screenx; j++) {
                if (dScreen[j][i] != ' ') {
                    x += dScreen[j][i];
                    x += " ";
                }
                else {
                    x += sScreen[j][i];
                    x += " ";
                }
            }
            x += "\n";
        }
        Sleep(50);
        ClearScreen();
        cout << x;
    }

    int screenx;
    int screeny;
    char sScreen[100][100];
    char dScreen[100][100];
private:

    string x;
};

class Dinamik {
public:
    Dinamik() {
        karakterx = 0;
        karaktery = 0;
        karakters = ' ';
        ekran = NULL;
    }

protected:
    int karakterx, karaktery;
    char karakters;
    Ekran* ekran;
};

class Player : public Dinamik {
public:
    int xForce=0;
    int yForce = 0;
    int xSpeed = 0;
    int ySpeed = 0;
    bool hareketli = false;
    bool spaceFree = false;
    Player(int x, int y, char sembol, Ekran& e) {
        karakterx = x;
        karaktery = y;
        karakters = sembol;
        ekran = &e;
        ekran->dAddChar(karakterx, karaktery, karakters);
    }
    void Atis() {
        karaktery = yukseklik - 2;
        while (!spaceFree) {
            ekran->dScreen[karakterx][karaktery] = karakters;
            ekran->drawScreen();
            if (GetAsyncKeyState(VK_LEFT)) {
                if (xForce > 0) {
                    xForce = 0;
                }
                xForce -= 10;
                Sleep(50);
            }
            else if (GetAsyncKeyState(VK_RIGHT)) {
                if (xForce < 0) {
                    xForce = 0;
                }
                xForce += 10;
                Sleep(50);
            }
            if (GetAsyncKeyState(VK_SPACE)) {
                while (GetAsyncKeyState(VK_SPACE) && karaktery != 1) {
                    yForce += 10;
                    ekran->dScreen[karakterx][karaktery] = karakters;
                    ekran->drawScreen();
                    Sleep(50);
                }
                spaceFree = true;
            }   
        }
    }
    void Gravity() {
        if (karaktery < yukseklik - 2) {
            yForce -= 10;
        }
    }
    void Carpismalar() {
        if ((karakterx == 1&&xSpeed<0) || (karakterx == genislik-1&&xSpeed>0)) {
            xSpeed = xSpeed * -1;
            xForce = xForce * -1;
            xForce -= (xForce / 2);
        }
        if ((karaktery == 1 && ySpeed > 0) || (karaktery == yukseklik - 1 && ySpeed < 0)) {
            ySpeed = ySpeed * -1;
            yForce = yForce * -1;
            yForce -= floor(yForce/2);
            if (abs(yForce) < 10) {
                yForce = 0;
            }
        }
    }
    void Speed() {
        if (xForce <= 40 && xForce >=-40) {
            xSpeed += xForce;
        }
        else if (xForce > 40) {
            xSpeed += 40;
        }
        else if (xForce < -40) {
            xSpeed -= 40;
        }
        if (yForce <= 40 && yForce >= -40) {
            ySpeed += yForce;
        }
        else if (yForce > 40) {
            ySpeed += 40;
        }
        else if (yForce < -40) {
            ySpeed -= 40;
        }
    }
    void move()
    {
        Carpismalar();
        hareketli = false;
        if (xSpeed > 0)
        {
            while (xSpeed > 0) {
                ekran->dScreen[karakterx][karaktery] = ' ';
                karakterx++;
                xSpeed -= 10;
                Carpismalar();
                hareketli = true;
            }
        }
        Carpismalar();
        if (xSpeed < 0)
        {
            while (xSpeed < 0) {
                ekran->dScreen[karakterx][karaktery] = ' ';
                karakterx--;
                xSpeed += 10;
                Carpismalar();
                hareketli = true;
            }
        }
        Carpismalar();
        if (ySpeed < 0)
        {
            while (ySpeed < 0) {
                ekran->dScreen[karakterx][karaktery] = ' ';
                karaktery++;
                ySpeed += 10;
                Carpismalar();
                hareketli = true;
            }
        }
        Carpismalar();
        if (ySpeed > 0)
        {
            while (ySpeed > 0) {
                ekran->dScreen[karakterx][karaktery] = ' ';
                karaktery--;
                ySpeed -= 10;
                Carpismalar();
                hareketli = true;
            }
        }
        if (!hareketli&&karaktery==yukseklik-2) {
            spaceFree = false;
        }if (xForce != 0) {
            xForce -= xForce/abs(xForce);
        }
        ekran->dScreen[karakterx][karaktery] = karakters;
        ekran->drawScreen();
        
    }
};
int main()
{
    cin >> genislik >> yukseklik;
    Ekran ekran(genislik, yukseklik);
    Player player(genislik/2, yukseklik-2, 'O', ekran);

    while (gameLoop) {
        if (!player.spaceFree) {
            player.Atis();
        }
        player.Gravity();
        player.Speed();
        player.move();
        ekran.drawScreen();
        Sleep(20);
    }

    ekran.drawScreen();
    cout << "Game Over" << endl;
    system("pause");
    return 0;
}