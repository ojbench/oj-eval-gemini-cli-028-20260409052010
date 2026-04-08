#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct Student {
    string name;
    char gender;
    int cls;
    int scores[9];
    int sum_score;
    int avg_score;
    int id;
};

int N = 0;
vector<Student> students;
unordered_map<string, int> name_to_id;
vector<int> leaderboard;
vector<int> rank_of;
bool started = false;
bool need_flush = false;

void update_avg(int id) {
    students[id].sum_score = 0;
    for (int i = 0; i < 9; ++i) {
        students[id].sum_score += students[id].scores[i];
    }
    students[id].avg_score = students[id].sum_score / 9;
}

bool cmp(int a, int b) {
    if (students[a].avg_score != students[b].avg_score) {
        return students[a].avg_score > students[b].avg_score;
    }
    for (int i = 0; i < 9; ++i) {
        if (students[a].scores[i] != students[b].scores[i]) {
            return students[a].scores[i] > students[b].scores[i];
        }
    }
    return students[a].name < students[b].name;
}

void flush_leaderboard() {
    if (!need_flush) return;
    sort(leaderboard.begin(), leaderboard.end(), cmp);
    for (int i = 0; i < N; ++i) {
        rank_of[leaderboard[i]] = i + 1;
    }
    need_flush = false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string cmd;
    while (cin >> cmd) {
        if (cmd == "ADD") {
            string name;
            char gender;
            int cls;
            int scores[9];
            cin >> name >> gender >> cls;
            for (int i = 0; i < 9; ++i) cin >> scores[i];

            if (started) {
                cout << "[Error]Cannot add student now.\n";
            } else if (name_to_id.count(name)) {
                cout << "[Error]Add failed.\n";
            } else {
                Student s;
                s.name = name;
                s.gender = gender;
                s.cls = cls;
                for (int i = 0; i < 9; ++i) s.scores[i] = scores[i];
                s.id = N;
                students.push_back(s);
                update_avg(N);
                name_to_id[name] = N;
                N++;
            }
        } else if (cmd == "START") {
            started = true;
            leaderboard.resize(N);
            rank_of.resize(N);
            for (int i = 0; i < N; ++i) leaderboard[i] = i;
            need_flush = true;
            flush_leaderboard();
        } else if (cmd == "UPDATE") {
            string name;
            int code, score;
            cin >> name >> code >> score;
            auto it = name_to_id.find(name);
            if (it == name_to_id.end()) {
                cout << "[Error]Update failed.\n";
            } else {
                int id = it->second;
                students[id].scores[code] = score;
                update_avg(id);
                need_flush = true;
            }
        } else if (cmd == "FLUSH") {
            flush_leaderboard();
        } else if (cmd == "PRINTLIST") {
            for (int i = 0; i < N; ++i) {
                int id = leaderboard[i];
                cout << rank_of[id] << " " << students[id].name << " "
                     << (students[id].gender == 'M' ? "male" : "female") << " "
                     << students[id].cls << " " << students[id].avg_score << "\n";
            }
        } else if (cmd == "QUERY") {
            string name;
            cin >> name;
            auto it = name_to_id.find(name);
            if (it == name_to_id.end()) {
                cout << "[Error]Query failed.\n";
            } else {
                cout << "STUDENT " << name << " NOW AT RANKING " << rank_of[it->second] << "\n";
            }
        } else if (cmd == "END") {
            break;
        }
    }
    return 0;
}