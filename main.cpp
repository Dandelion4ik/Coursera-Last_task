#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

class Date {
public:
    [[nodiscard]] int GetYear() const {
        return this->year;
    }

    [[nodiscard]] int GetMonth() const {
        return this->month;
    }

    [[nodiscard]] int GetDay() const {
        return this->day;
    }

    friend bool operator<(const Date &a, const Date &b) {
        if (a.year > b.year)
            return false;
        else if (a.year < b.year)
            return true;
        if (a.month > b.month)
            return false;
        else if (a.month < b.month)
            return true;
        if (a.day > b.day)
            return false;
        else if (a.day < b.day)
            return true;
        return false;
    }

    bool operator==(const Date &a) const {
        return this->year == a.year && this->month == a.month && this->day == a.day;
    }


    Date(int year, int month, int day) {
        this->year = year;
        this->month = month;
        this->day = day;
    }

    Date() {
        this->year = 0;
        this->month = 0;
        this->day = 0;
    }


    void Print_Date() const {
        cout<<setfill('0');
        cout<<setw(4)<< this->year << "-" <<setw(2)
        << this->month << "-" <<setw(2)<< this->day;
    }


private:
    int year;
    int month;
    int day;
};


class Database {
public:

    void AddEvent(const Date &date, const string &event) {
        Date a;
        if (date == a) {
            return;
        }
        bool flag = true;
        for (auto &it:base[date]) {
            if (it == event)
                flag = false;
        }
        if (flag)
            base[date].push_back(event);
    }

    void Find(const Date &date) {
        sort(base[date].begin(), base[date].end());
        for (auto &it:base[date]) {
            cout << it << endl;
        }
    }

    void Print() {
        for (auto[key, value]:base) {
            sort(value.begin(), value.end());
            for (auto &new_it:value) {
                if (new_it.length() == 0)
                    continue;
                key.Print_Date();
                cout << " " << new_it;
                cout << endl;
            }
        }
    }

    bool DeleteEvent(const Date &date, const string &event) {
        vector<string> new_vec;
        for (auto &it:base[date]) {
            if (it != event) {
                new_vec.push_back(it);
            }
        }
        if(new_vec.size()==base[date].size()) {
            return false;
        }
        base[date]=new_vec;
        return true;
    }

    int DeleteDate(const Date &date) {
        int n = base[date].size();
        base[date].clear();
        return n;
    }


private:
    map<Date, vector<string>> base;
};


void search_command(const string &enter, string &command, int &i) {
    while (enter[i] != ' ' && enter[i] != '\0') {
        command += enter[i];
        i++;
    }
    if (command != "Add" && command != "Find" && command != "Del" && command != "Print") {
        throw exception();
    }
    i++;
}

void search_data(const string &enter, string &data, int &i) {
    int kost = 0;
    int p=i;
    int k=i;
    while (enter[i] != ' ' && enter[i] != '\0') {
        if (enter[i] == '-')
            kost++;
        data += enter[i];
        i++;
    }
    i++;
    while (enter[p] != ' ' && enter[p] != '\0') {
        if ((enter[p]<'0'||enter[p]>'9')&&enter[p]!='-'&&enter[p]!='+') {
            throw exception();
        }
        p++;
    }
    if(enter[k]=='-'&&enter[k+1]=='-')
        throw exception();
    if (kost < 2)
        throw exception();
    for (int k = 0; k < data.size() - 2; k++) {
        if (data[k] == '-' & data[k + 1] == '-' & data[k + 2] == '-')
            throw exception();
    }
    if(data[data.size()-1]=='-')
        throw exception();
}

void search_year(const string &data, int &year, int &k, string &year_string) {
    if (data[k] == '-' && data[k + 1] == '-') {
        year_string += data[k];
        k++;
    }
    int c = 0;
    while (data[k] == '+') {
        year_string += data[k];
        k++;
        c++;
    }
    int count = 0;
    if (data[k] == '-') {
        year_string += data[k];
        k++;
        count = 1;
    }
    while (data[k] != '-') {
        year_string += data[k];
        k++;
    }
    k++;
    year = stoi(year_string);
}

void search_month(const string &data, int &month, int &k, string &month_string) {
    if (data[k] == '-') {
        month_string += data[k];
        k++;
    }
    while (data[k] != '-') {
        month_string += data[k];
        k++;
    }
    k++;
    month = stoi(month_string);
    if (month <= 0 || month > 12)
        throw exception();
}

void search_day(const string &data, int &day, int &k, string &day_string) {
    if (data[k] == '-') {
        day_string += data[k];
        k++;
    }
    while (data[k] != ' ' && data[k] != '\0') {
        day_string += data[k];
        k++;
    }
    k++;
    day = stoi(day_string);
    if (day <= 0 || day > 31)
        throw exception();
}

string search_event(const string &enter, int &k) {
    string event;
    while (enter[k] != '\0' && k < enter.length()) {
        event += enter[k];
        k++;
    }
    return event;
}

Date search_time(const string &enter, string &data, string &command, string &event, int &i, int &k) {
    try {
        search_data(enter, data, i);
    } catch (exception &b) {
        cout << "Wrong date format: " << data << endl;
        command = "";
        data = "";
        event = "";
        i = 0;
        exit(1);
    }
    string year_string = "";
    int year = 0;
    try {
        search_year(data, year, k, year_string);
    } catch (exception &c) {
        cout << "Year value is invalid: "<< year_string << endl;
        exit(1);
    }

    string month_string = "";
    int month = 0;
    try {
        search_month(data, month, k, month_string);
    } catch (exception &c) {
        cout << "Month value is invalid: " << month_string << endl;
        exit(1);
    }

    string day_string = "";
    int day = 0;
    try {
        search_day(data, day, k, day_string);
    } catch (exception &c) {
        cout << "Day value is invalid: " << day_string << endl;
        exit(1);
    }
    return {year, month, day};
}


int main() {
    Database db;
    string enter;
    string command = "";
    string data = "";
    string event = "";
    int i = 0;
    Database top;
    while (getline(cin, enter)) {
        if (enter.length()==0){
            continue;
        }
        try {
            search_command(enter, command, i);
        } catch (exception &a) {
            cout << "Unknown command: " << command << endl;// ЕНДЛ ВОЗМОЖНО НУЖНО УБРАТЬ
            command = "";
            data = "";
            event = "";
            i = 0;
            return 1;
        }
        if (command == "Add") {
            int k = 0;
            Date time = search_time(enter, data, command, event, i, k);
            k += 4;
            event = search_event(enter, k);
            if (event.length() != 0) {
                top.AddEvent(time, event);
            }
        } else if (command == "Find") {
            int k = 0;
            Date time = search_time(enter, data, command, event, i, k);
            top.Find(time);
        } else if (command == "Print") {
            top.Print();
        } else if (command == "Del") {
            int k = 0;
            Date time = search_time(enter, data, command, event, i, k);
            Date a;
            if(time==a)
                continue;
            k += 4;
            event = "";
            event = search_event(enter, k);
            if (event.length()) {
                if (top.DeleteEvent(time, event)) {
                    cout << "Deleted successfully" << endl;
                } else {
                    cout << "Event not found" << endl;
                }
            } else {
                int n = top.DeleteDate(time);
                cout << "Deleted " << n << " events\n";
            }
        }
        i = 0;
        enter = "";
        command = "";
        data = "";
        event = "";
    }


    return 0;
}
