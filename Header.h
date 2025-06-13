#pragma once
#include <string>
#include <Windows.h>
#include <iomanip>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cctype>
using namespace std;
class Comment {
public:
    string text;
    int likes;//Создаёт комментарий с заданным текстом txt и начальным количеством лайков 0
    Comment(const string& txt);//Увеличивает количество лайков комментария на 1
    void like();//возвращает кол-во лайков
    string str() const;
};

class Song {
public:
    string group;
    string title;
    int bpm;
    string key;
    vector<Comment> comments;//список комментариев к песне
    
    Song(const string& grp, const string& ttl, int bpm, const string& key);//конструктор
    void addComment(const string& text);//добавить комментарии
    void likeComment(int index);//увеличевает лайки указанного комментария 
    void displayComments() const;//отображает все комментарии к песне, отсортированные по количеству лайков
    string str() const;//возвращает строчку с описанием песни(группа название бпм тональность) 
};
//класс, управляющий списком песен и работой с файлом
class SongManager {
private:
    vector<Song> songs;//все песни
    string filename;//имя файла

public:
    SongManager(const string& file);//конструктор(сохраняет имя файла и загружает песни)
    void loadFromFile();//загружает список песен из файла
    void saveToFile();//сохраняет текущий список песен в фаил и комментарии в фаил
    void displaySongs() const;//выводит таблицу на экран
    void editSong();//редактирование песни(изменение бпм и тональности)
    void commentMenu();//меню комментариев
    void menu();//главный интерфейс
    void addNewSong();//добавить новую песню в список(обязательно надо сохранить)
    void clearScreen();//очистить консоль(для удобства)
    void removeSong();//удалить песню
};

int getSimpleInt(const string& prompt);//валидация
