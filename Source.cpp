#include "Header.h"

using namespace std;

Comment::Comment(const string& txt) : text(txt), likes(0) {}

void Comment::like() {
    ++likes;
}

string Comment::str() const {
    return text + " (Likes: " + to_string(likes) + ")";
}

bool isUint(const string& s) {
    return s.find_first_not_of("0123456789") == string::npos;
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
        cout << "Файл не найден. Будет создан при сохранении.\n";
        return;
    }

    string line;
    Song* currentSong = nullptr;

    while (getline(fin, line)) {
        if (line == "---") {
            currentSong = nullptr;
            continue;
        }

        if (line.rfind("#COMMENT:", 0) == 0 && currentSong) {
            string commentLine = line.substr(9);
            size_t sep = commentLine.rfind(';');
            if (sep != string::npos) {
                string text = commentLine.substr(0, sep);
                int likes = stoi(commentLine.substr(sep + 1));
                Comment c(text);
                c.likes = likes;
                currentSong->comments.push_back(c);
            }
        }
        else {
            stringstream ss(line);
            string group, title, bpmStr, key;

            if (getline(ss, group, ';') &&
                getline(ss, title, ';') &&
                getline(ss, bpmStr, ';') &&
                getline(ss, key, ';')) {

                try {
                    int bpm = stoi(bpmStr);
                    songs.emplace_back(group, title, bpm, key);
                    currentSong = &songs.back();    
                }
                catch (...) {
                    cout << "Ошибка чтения строки: " << line << endl;
                }
            }
        }
    }
}


void SongManager::saveToFile() {
    ofstream fout(filename);
    for (const auto& song : songs) {
        fout << song.group << ";" << song.title << ";" << song.bpm << ";" << song.key << ";\n";

        for (const auto& comment : song.comments) {
            fout << "#COMMENT:" << comment.text << ";" << comment.likes << "\n";
        }

        fout << "---\n";
    }
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
    string klmn;
    int id;
    cout << "Введите ID песни для просмотра комментариев: ";
    id = getSimpleInt(klmn);

    if (id < 0 || id >= songs.size()) {
        cout << "Неверный ID.\n";
        return;
    }

    Song& song = songs[id];
    int choice;
    string asd;
    do {
        clearScreen();
        cout << "Комментарии к песне: " << song.title << "\n";
        song.displayComments();
        cout << "\n1. Добавить комментарий\n2. Лайкнуть комментарий\n0. Назад\nВыбор: ";

        choice = getSimpleInt(asd);

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
        case 0: {
            clearScreen();
            break;
        }
        default: {
            cout << "Ошибка, неверный выбор" << endl;
            break;
        }
      }
    } while (choice != 0);
}

void SongManager::menu() {
    int choice;
    string dk;
    do {
        cout << "1. Показать все песни\n"
            << "2. Редактировать песню\n"
            << "3. Комментарии\n"
            << "4. Сохранить\n"
            << "5. Добавить новую песню\n"
            << "6. Удалить песню\n"
            << "0. Выход\n"
            << "Выбор: ";

        choice = getSimpleInt(dk);
     
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

    int getSimpleInt(const string& prompt) {
        string input;
        int value;

        while (true) {
            cout << prompt;
            getline(cin, input);
            if (!input.empty() && all_of(input.begin(), input.end(), ::isdigit)) {
                value = stoi(input);
                return value;
            }

            cout << "Введите только целое число.\n";
        }
    }
