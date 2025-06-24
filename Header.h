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

struct CommentNode {
    string text;
    int likes;
    CommentNode* prev;
    CommentNode* next;

    CommentNode(const string& txt,
        CommentNode* p = nullptr,
        CommentNode* n = nullptr);
};

class CommentList {
private:
    CommentNode* head;
    CommentNode* tail;
    int size;

    void checkIntegrity() const;//проверка целостности узла

public:
    CommentList();//конструктор
    ~CommentList();//дестркутор

    void addComment(const string& text);//добавить коммент
    void likeComment(int index);//лайк коммента
    //геттеры
    vector<CommentNode*> getSortedComments() const;
    int getSize() const;
};

class Song {
public:
    string group;
    string title;
    int bpm;
    string key;
    CommentList comments;

    Song(const string& grp, const string& ttl, int bpm, const string& key);//конструктор
    void addComment(const string& text);//добавить коммент
    void likeComment(int index);//лайкнуть коммент с комментлиста(потому что мы не берем в SongManager CommentList)
    void displayComments() const;//вывод всех комментариев
    string str() const;//работа с меню
};
//все опции работы с песней и комментариями
class SongManager {
private:
    vector<Song> songs;
    string filename;

public:
    SongManager(const string& file);//конструктоор
    void loadFromFile();//выгрузка информации из фаила
    void saveToFile();//сохранение
    void displaySongs() const;//вывод всех песен
    void editSong();//редактировать песню
    void commentMenu();//меню комметов
    void menu();//основное меню
    void addNewSong();//добавить новую песню
    void clearScreen();//очистить консоль
    void removeSong();//удалить песню
};

int getSimpleInt(const string& prompt);//валидация
