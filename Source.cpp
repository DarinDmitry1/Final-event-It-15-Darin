#include "Header.h"

using namespace std;

Comment::Comment(const string& txt) : text(txt), likes(0) {}

void Comment::like() {
    ++likes;
}

string Comment::str() const {
    return text + " (Likes: " + to_string(likes) + ")";
}

Song::Song(const string& grp, const string& ttl, int bpmVal, const string& k)
    : group(grp), title(ttl), bpm(bpmVal), key(k) {}

void Song::addComment(const string& text) {
    comments.emplace_back(text);
}

void Song::likeComment(int index) {
    if (index >= 0 && index < comments.size()) {
        comments[index].like();
    }
    else {
        cout << "Неверный индекс комментария.\n";
    }
}

void Song::displayComments() const {
    vector<Comment> sorted = comments;
    sort(sorted.begin(), sorted.end(), [](const Comment& a, const Comment& b) {
        return a.likes > b.likes;
        });

    for (int i = 0; i < sorted.size(); ++i) {
        cout << i << ". " << sorted[i].str() << endl;
    }
}

string Song::str() const {
    stringstream ss;
    ss << group << " | " << title << " | BPM: " << bpm << " | Key: " << key;
    return ss.str();
}

SongManager::SongManager(const string& file) : filename(file) {
    loadFromFile();
}

void SongManager::loadFromFile() {
    ifstream fin(filename);
    if (!fin) {
        cout << "Файл не найден. Создается новый при сохранении.\n";
        return;
    }

    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string group, title, bpmStr, key;

        if (getline(ss, group, ';') &&
            getline(ss, title, ';') &&
            getline(ss, bpmStr, ';') &&
            getline(ss, key, ';')) {
            try {
                int bpm = stoi(bpmStr);
                songs.emplace_back(group, title, bpm, key);
            }
            catch (...) {
                cout << "Ошибка чтения строки: " << line << endl;
            }
        }
    }
}

void SongManager::saveToFile() {
    ofstream fout(filename);
    for (const auto& song : songs) {
        fout << song.group << ";" << song.title << ";" << song.bpm << ";" << song.key << ";\n";
    }
}

void SongManager::removeSong() {
    int id;
    cout << "Введите ID песни для удаления: ";
    cin >> id;

    if (id < 0 || id >= songs.size()) {
        cout << "Неверный ID.\n";
        return;
    }

    cout << "Вы уверены, что хотите удалить песню \"" << songs[id].title << "\"? (y/n): ";
    char ch;
    cin >> ch;
    if (ch == 'y' || ch == 'Y') {
        songs.erase(songs.begin() + id);
        cout << "Песня удалена.\n";
    }
    else {
        cout << "Удаление отменено.\n";
    }

    cin.ignore();
}


void SongManager::displaySongs() const {
    cout << "\n"
        << setw(4) << "ID" << " | "
        << setw(20) << left << "Группа"
        << " | " << setw(30) << left << "Название песни"
        << " | " << setw(5) << left << "BPM"
        << " | " << setw(6) << left << "Тональность" << endl;

    cout << string(75, '-') << endl;

    for (size_t i = 0; i < songs.size(); ++i) {
        cout << setw(4) << i << " | "
            << setw(20) << left << songs[i].group
            << " | " << setw(30) << left << songs[i].title
            << " | " << setw(5) << left << songs[i].bpm
            << " | " << setw(6) << left << songs[i].key << endl;
    }
}


void SongManager::editSong() {
    int id;
    cout << "Введите ID песни для редактирования: ";
    cin >> id;
    if (id < 0 || id >= songs.size()) {
        cout << "Неверный ID.\n";
        return;
    }

    int newBPM;
    string newKey;
    cout << "Новый BPM (или -1, чтобы не менять): ";
    cin >> newBPM;
    cin.ignore();
    cout << "Новая тональность (оставьте пустым, чтобы не менять): ";
    getline(cin, newKey);

    if (newBPM > 0) songs[id].bpm = newBPM;
    if (!newKey.empty()) songs[id].key = newKey;

    cout << "Песня обновлена.\n";
}

void SongManager::commentMenu() {
    int id;
    cout << "Введите ID песни для просмотра комментариев: ";
    cin >> id;
    cin.ignore();

    if (id < 0 || id >= songs.size()) {
        cout << "Неверный ID.\n";
        return;
    }

    Song& song = songs[id];
    int choice;
    do {
        clearScreen();
        cout << "Комментарии к песне: " << song.title << "\n";
        song.displayComments();
        cout << "\n1. Добавить комментарий\n2. Лайкнуть комментарий\n0. Назад\nВыбор: ";
        cin >> choice;
        cin.ignore();

      switch(choice){
        case 1:{
            string text;
            cout << "Введите комментарий: ";
            getline(cin, text);
            song.addComment(text);
            break;
        }
        case 2: {
            int idx;
            cout << "Введите индекс комментария: ";
            cin >> idx;
            song.likeComment(idx);
            break;
        }
        case 3: {

        }
        default: {
            cout << "Ошибка, неверный выбор" << endl;
        }
      }
    } while (choice != 0);
}

void SongManager::menu() {
    int choice;
    do {
        cout << "1. Показать все песни\n"
            << "2. Редактировать песню\n"
            << "3. Комментарии\n"
            << "4. Сохранить\n"
            << "5. Добавить новую песню\n"
            << "6. Удалить песню\n"
            << "0. Выход\n"
            << "Выбор: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: clearScreen(); displaySongs(); break;
        case 2: clearScreen(); editSong(); break;
        case 3: clearScreen(); commentMenu(); break;
        case 4: clearScreen(); saveToFile(); cout << "Сохранено.\n"; break;
        case 5: clearScreen(); addNewSong(); break;
        case 6: clearScreen(); removeSong(); break;
        case 0: break;
        default: cout << "Неверный выбор.\n"; break;
        }
    } while (choice != 0);
}


    void SongManager::addNewSong() {
    string group, title, key;
    int bpm;

    cout << "Введите название группы/исполнителя: ";
    getline(cin, group);

    cout << "Введите название песни: ";
    getline(cin, title);

    cout << "Введите BPM: ";
    cin >> bpm;
    cin.ignore();

    cout << "Введите тональность: ";
    getline(cin, key);

    songs.emplace_back(group, title, bpm, key);
    cout << "Песня добавлена!\n";
}

    void SongManager::clearScreen() {
#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif
    }
