#include "Header.h"

int main() {
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    SongManager manager("songs.txt");
    manager.menu();
    return 0;
}
