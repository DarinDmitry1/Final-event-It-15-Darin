#include "Header.h"

CommentNode::CommentNode(const string& txt,
    CommentNode* p,
    CommentNode* n)
    : text(txt), likes(0), prev(p), next(n) {}

CommentList::CommentList() : head(nullptr), tail(nullptr), size(0) {}

CommentList::~CommentList() {
    CommentNode* current = head;
    while (current) {
        CommentNode* next = current->next;
        delete current;
        current = next;
    }
}

void CommentList::addComment(const string& text) {
    CommentNode* newNode = new CommentNode(text, tail, nullptr);

    if (!head) {
        head = newNode;
    }
    else {
        tail->next = newNode;
        newNode->prev = tail;
    }

    tail = newNode;
    size++;
}

void CommentList::likeComment(int index) {
    if (index < 0 || index >= size) return;

    CommentNode* current = head;
    for (int i = 0; i < index && current; ++i) {
        current = current->next;
    }

    if (current) {
        current->likes++;
    }
}

vector<CommentNode*> CommentList::getSortedComments() const {
    vector<CommentNode*> result;
    CommentNode* current = head;

    while (current) {
        result.push_back(current);
        current = current->next;
    }

    stable_sort(result.begin(), result.end(),
        [](const CommentNode* a, const CommentNode* b) {
            return a->likes > b->likes;
        });

    return result;
}

int CommentList::getSize() const {
    return size;
}

void CommentList::checkIntegrity() const {
    CommentNode* current = head;
    CommentNode* prev = nullptr;
    int count = 0;

    while (current) {
        if (current->prev != prev) {
            cerr << "Целостность нарушена на узле " << count << endl;
        }
        prev = current;
        current = current->next;
        count++;
    }

    if (count != size) {
        cerr << "Несоответствие по размеру, заявлено " << size
            << ", на самом деле " << count << endl;
    }
}

Song::Song(const string& grp, const string& ttl, int bpmVal, const string& k)
    : group(grp), title(ttl), bpm(bpmVal), key(k) {}

void Song::addComment(const string& text) {
    comments.addComment(text);
}

void Song::likeComment(int index) {
    comments.likeComment(index);
}

void Song::displayComments() const {
    vector<CommentNode*> sorted = comments.getSortedComments();
    for (size_t i = 0; i < sorted.size(); ++i) {
        cout << i << ". " << sorted[i]->text << " (Likes: " << sorted[i]->likes << ")\n";
    }
}

string Song::str() const {
    return group + " | " + title + " | BPM: " + to_string(bpm) + " | Key: " + key;
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
        if (line.empty()) continue;

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
                currentSong->addComment(text);
                if (currentSong->comments.getSize() > 0) {
                    auto comments = currentSong->comments.getSortedComments();
                    if (!comments.empty()) {
                        comments[0]->likes = likes;
                    }
                }
            }
        }
        else {
            stringstream ss(line);
            string group, title, bpmStr, key;

            if (getline(ss, group, ';') && getline(ss, title, ';') &&
                getline(ss, bpmStr, ';') && getline(ss, key, ';')) {
                try {
                    songs.emplace_back(group, title, stoi(bpmStr), key);
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
    if (!fout) {
        cerr << "Не удалось открыть файл для записи!" << endl;
        return;
    }

    for (const auto& song : songs) {
        fout << song.group << ";" << song.title << ";" << song.bpm << ";" << song.key << ";\n";

        auto comments = song.comments.getSortedComments();
        for (const auto& comment : comments) {
            fout << "#COMMENT:" << comment->text << ";" << comment->likes << "\n";
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
    int id;
    cout << "Введите ID песни для просмотра комментариев: ";
    cin >> id;

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

        switch(choice) {
            case 1: {
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
        }
    } while (choice != 0);
}

void SongManager::menu() {
    int choice;
    do {
        cout << "\n1. Показать все песни\n"
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
    system("cls");
#else
    system("clear");
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
