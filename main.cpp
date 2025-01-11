#include <QtCore/QCoreApplication>
#include <QtWidgets/qdatetimeedit.h>
#include <QtCore/qregularexpression.h>
#include <QtCore/qdebug.h>
#include <iostream>
#include <string.h>
#include <fstream>


using namespace std;


class Contact {
public:
    int u_id;
    string name;
    string surname;
    string middle_name;
    string mail;
    string address;
    vector<string> phone_numbers;
    string date_of_birth;

    Contact() {
        u_id = -1;
        name = "-";
        surname = "-";
        middle_name = "-";
        mail = "-";
        address = "-";
        phone_numbers = {};
        date_of_birth = "";
    }

    Contact(string str_data) {
        int pre_pos = 0, cur_pos = 0;
        string field_data;

        cur_pos = str_data.find("|", cur_pos + 1);
        field_data = str_data.substr(pre_pos, cur_pos - pre_pos);
        u_id = stoi(field_data);

        pre_pos = cur_pos;
        cur_pos = str_data.find("|", cur_pos + 1);
        field_data = str_data.substr(pre_pos + 1, cur_pos - pre_pos - 1);
        name = field_data;

        pre_pos = cur_pos;
        cur_pos = str_data.find("|", cur_pos + 1);
        field_data = str_data.substr(pre_pos + 1, cur_pos - pre_pos - 1);
        surname = field_data;

        pre_pos = cur_pos;
        cur_pos = str_data.find("|", cur_pos + 1);
        field_data = str_data.substr(pre_pos + 1, cur_pos - pre_pos - 1);
        middle_name = field_data;

        pre_pos = cur_pos;
        cur_pos = str_data.find("|", cur_pos + 1);
        field_data = str_data.substr(pre_pos + 1, cur_pos - pre_pos - 1);
        mail = field_data;

        pre_pos = cur_pos;
        cur_pos = str_data.find("|", cur_pos + 1);
        field_data = str_data.substr(pre_pos + 1, cur_pos - pre_pos - 1);
        address = field_data;

        pre_pos = cur_pos;
        cur_pos = str_data.find("|", cur_pos + 1);
        field_data = str_data.substr(pre_pos + 1, cur_pos - pre_pos - 1);
        date_of_birth = field_data;

        for (; str_data.find("|", cur_pos + 1) != -1; ) {
            pre_pos = cur_pos;
            cur_pos = str_data.find("|", cur_pos + 1);
            field_data = str_data.substr(pre_pos + 1, cur_pos - pre_pos - 1);
            phone_numbers.push_back(field_data);
        }
        phone_numbers.push_back(str_data.substr(cur_pos + 1, str_data.length() - cur_pos - 1));
    }

    void print_string() {
        cout << this->u_id << '|' << this->name << '|' << this->surname << '|' << this->middle_name << '|' << this->mail << '|' << this->address << '|' << this->date_of_birth << '|';
        for (auto it = this->phone_numbers.begin(); it != this->phone_numbers.end(); it++) {
            if (it == (this->phone_numbers.end() - 1))
                cout << *it << '\n';
            else
                cout << *it << '|';
        }
    };
    void print_detailed() {
        cout << "id: " << this->u_id << endl;
        cout << "1.Имя: " << this->name << endl;
        cout << "2.Фамилия: " << this->surname << endl;
        cout << "3.Отчество: " << this->middle_name << endl;
        cout << "4.email: " << this->mail << endl;
        cout << "5.address: " << this->address << endl;
        cout << "6.Телефонные номера: ";
        for (auto it = this->phone_numbers.begin(); it != this->phone_numbers.end(); it++)
            cout << *it << " | ";
        cout << endl;
        cout << "7.Дата рождения: " << this->date_of_birth << endl;
    };
};

 
void show_contacts(vector<Contact> contacts, int page = 0) {
    for (auto it = contacts.begin(); it != contacts.end(); it++) {
        it->print_string();
    }
}

bool check_mail(string some_str) {
    QRegularExpression mail("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$");
    QString tmp = some_str.c_str();
    return mail.match(tmp).hasMatch() && (tmp.length() < 50);
}

bool check_date(string date) {
    QString dateq = date.c_str();
    if (!QRegularExpression("^[0-9]{1,2}+[.]+[0-9]{1,2}+[.]+[0-9]{4,4}").match(dateq).hasMatch())
        return false;
    QDate test = QDate::fromString(dateq, "d'.'M'.'yyyy");
    if (test.isValid() == true)
        return true;
    return false;
}


bool check_phone(string phone_number) {
    QString phone_numberq = phone_number.c_str();
    if (QRegularExpression("^[+]+[0-9]{11,11}").match(phone_numberq).hasMatch())
       return true;
    if (QRegularExpression("^[0-9]{11,11}").match(phone_numberq).hasMatch())
        return true;
    if (QRegularExpression("^[+]+[0-9]+[(]+[0-9]{3,3}+[)]+[0-9]{7,7}").match(phone_numberq).hasMatch())
        return true;
    if (QRegularExpression("^[0-9]+[(]+[0-9]{3,3}+[)]+[0-9]{7,7}").match(phone_numberq).hasMatch())
        return true;
    if (QRegularExpression("^[+]+[0-9]+[(]+[0-9]{3,3}+[)]+[0-9]{3,3}+[-]+[0-9]{2,2}+[-]+[0-9]{2,2}").match(phone_numberq).hasMatch())
        return true;
    if (QRegularExpression("^[0-9]+[(]+[0-9]{3,3}+[)]+[0-9]{3,3}+[-]+[0-9]{2,2}+[-]+[0-9]{2,2}").match(phone_numberq).hasMatch())
        return true;
    return false;
}


void write_contact_to_file(Contact& new_contact) {

    fstream out("contacts.csv", ios::app);
    if (out.is_open()) {
        out << new_contact.u_id << '|' << new_contact.name << '|' << new_contact.surname << '|' << new_contact.middle_name << '|' << new_contact.mail << '|' << new_contact.address << '|' << new_contact.date_of_birth << '|';
        for (auto it = new_contact.phone_numbers.begin(); it != new_contact.phone_numbers.end(); it++) {
            if (it == (new_contact.phone_numbers.end() - 1))
                out << *it << '\n';
            else
                out << *it << '|';
        }
    }
    out.close();
}

void write_contacts(vector<Contact> &contacts) {
    fstream contact_file("contacts.csv", ios::out);
    for (auto it = contacts.begin(); it != contacts.end(); it++) {
        write_contact_to_file(*it);
    }
    contact_file.close();
}

void delete_contact(vector<Contact> &contacts) {
    int rm_id;
    show_contacts(contacts);
    cout << "\nВведите id контакта, который вы хотите удалить\n";
    cin >> rm_id;
    for (auto it = contacts.begin(); it != contacts.end(); it++) {
        if (it->u_id == rm_id) {
            contacts.erase(it);
            write_contacts(contacts);
            return;
        }
    }
    
}

void change_field(Contact &contact, int field_id) {
    string tmp_in;
    int option, counter;
    switch (field_id) {
        case 1:
            cout << "\nСтарое значение поля name: " << contact.name << endl;
            cout << "\nВведите новое значение поля: ";
            cin >> contact.name;
            break;
        case 2:
            cout << "\nСтарое значение поля surname: " << contact.surname << endl;
            cout << "\nВведите новое значение поля: ";
            cin >> contact.surname;
            break;
        case 3:
            cout << "\nСтарое значение поля middle_name: " << contact.middle_name << endl;
            cout << "\nВведите новое значение поля: ";
            cin >> contact.middle_name;
            break;
        case 4:
            cout << "\nСтарое значение поля mail: " << contact.mail << endl;
            cout << "\nВведите новое значение поля: ";
            cin >> contact.middle_name;
            break;
        case 5:
            cout << "\nСтарое значение поля address: " << contact.address << endl;
            cout << "\nВведите новое значение поля: ";
            cin >> contact.address;
            break;
        case 6:
            cout << "\nСтарое значение поля phone_numbers: ";
            for (int i = 0; i != contact.phone_numbers.size(); i++) {
                if (i == contact.phone_numbers.size() - 1)
                    cout << i << ". " << contact.phone_numbers[i];
                else
                    cout << i << ". " << contact.phone_numbers[i] << '|';
            }
            cout << "\nВыберите какой номер нужно изменить: ";
            cin >> option;
            if (option > contact.phone_numbers.size())
                return;
            cout << "\nСтарое значение номера: " << contact.phone_numbers[option - 1];
            cout << "\nВведите новое значение поля: ";
            cin >> tmp_in;
            if (check_phone(tmp_in))
                contact.phone_numbers[option - 1] = tmp_in;
            break;
        case 7:
            cout << "\nСтарое значение поля date_of_birth: " << contact.address << endl;
            cout << "\nВведите новое значение date_of_birth: ";
            cin >> contact.date_of_birth;
            break;
    }
}

void change_contact(vector<Contact> &contacts) {
    int ch_id, ch_field;
    show_contacts(contacts);
    cout << "\nВведите id контакта, который вы хотите изменить\n";
    cin >> ch_id;
    for (auto it = contacts.begin(); it != contacts.end(); it++) {
        if (it->u_id == ch_id) {
            it->print_detailed();
            cout << "\nВведите номер поля, которое вы желаете поменять\n";
            cin >> ch_field;
            change_field(*it, ch_field);
            write_contacts(contacts);
            return;
        }
    }
    cout << "\nКонтакт не найден\n";
}

void show_sorted(vector<Contact> contacts, int field_option = 0) {

}



void add_contact(vector<Contact> &contacts) {
    Contact new_contact;
    string tmp;
    if (contacts.size() == 0)
        new_contact.u_id = 1;
    else
        new_contact.u_id = (contacts.end() - 1)->u_id + 1;
    cout << "Введите Имя контакта\n";
    cin >> tmp;
    new_contact.name = tmp;

    cout << "Введите Фамилию контакта\n";
    cin >> tmp;
    new_contact.surname = tmp;

    cout << "Введите Отчество контакта\n";
    cin >> tmp;
    new_contact.middle_name = tmp;

    while (true) {
        cout << "Введите email контакта\n";
        cin >> tmp;
        if (check_mail(tmp)) {
            new_contact.mail = tmp;
            break;
        }
        else
            cout << "\nНеправильная введен email, повторите ввод\n";
    }

    cout << "Введите адрес контакта\n";
    cin >> tmp;
    new_contact.address = tmp;
 
    while (true) {
        cout << "Введите дату рождения контакта в формате dd.mm.yyyy\n";
        cin >> tmp;
        if (check_date(tmp)) {
            new_contact.date_of_birth = tmp;
            break;
        }
        else
            cout << "\nНеправильная дата, повторите ввод\n";
    }

    cout << "Введите номера телефонов(q - закончить ввод)\n";
    for (cin >> tmp;; cin >> tmp) {
        if (tmp == "q") {
            break;
        }
        else {
            if (check_phone(tmp))
                new_contact.phone_numbers.push_back(tmp);
            else
                cout << "\nНеправильно введен номер\n";
        }
    }
    new_contact.print_detailed();
    contacts.push_back(new_contact);
    write_contact_to_file(new_contact);
}



int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "RU");
    fstream contact_file("contacts.csv");
    string contact_str;
    vector<Contact> contacts = {};

    while (getline(contact_file, contact_str))
    {
        cout << contact_str << endl;
        if (contact_str.size() > 1)
            contacts.push_back(Contact(contact_str));
    }
    contact_file.close();

    string in;
    cout << "Введите опцию:\n 1 - добавить контакт \n 2 - редактировать существующий контакт \n 3 - удалить существующий контакт \n q - выйти из программы" << endl;
    cin >> in;
    for (; ; cin >> in) {
        if (in == "1")
            add_contact(contacts);
        else if (in == "2")
            change_contact(contacts);
        else if (in == "3")
            delete_contact(contacts);
        else if (in == "q")
            break;
        else
            cout << "\nВведена неправильная опция, повторите снова \n";
        cout << "Введите опцию:\n 1 - добавить контакт \n 2 - редактировать существующий контакт \n 3 - удалить существующий контакт \n q - выйти из программы" << endl;
    }
    return 0;
}
