#include <QtCore/QCoreApplication>
#include <QtWidgets/qdatetimeedit.h>
#include <QtCore/qregularexpression.h>
#include <QtCore/qdebug.h>
#include <QApplication>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QMessageBox>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QComboBox>
#include <iostream>
#include <string.h>
#include <fstream>
#include <qdebug.h>
#include <algorithm>
#include "sqlite3.h"


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

bool check_date(QDate date) {
    if (date > QDate::currentDate())
        return false;
    if (!date.isValid())
        return false;
    return true;
}

bool check_name(QString name) {
    if (QRegularExpression("[A-Za-z]{1,}").match(name).hasMatch())
        return true;
    if (QRegularExpression(QString::fromLocal8Bit("[А-Яа-я]{1,}")).match(name).hasMatch())
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

void delete_contact_db(vector<int> id_list) {
    sqlite3* db;
    char* err_msg = 0;
    int rc = sqlite3_open("contacts.db", &db);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return;
    }
    string sql = "";
    for (auto it = id_list.begin(); it != id_list.end(); it++) {
        sql = "DELETE FROM people WHERE id =  " + to_string(*it);
        rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err_msg);
        if (rc != SQLITE_OK)
        {
            printf("SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);      // очищаем ресурсы
            sqlite3_close(db);
            return;
        }
    }
    sqlite3_close(db);
}

void change_contact_db(Contact& chng_contact) {
    sqlite3* db;
    char* err_msg = 0;
    int rc = sqlite3_open("contacts.db", &db);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return;
    }
    string sql = "UPDATE people SET name = '" + chng_contact.name + "', surname =  '" + chng_contact.surname + "', middle_name = '" + chng_contact.middle_name + \
        "', email = '" + chng_contact.mail + "', address = '" + chng_contact.address + "', date_of_birth = '" + chng_contact.date_of_birth + "', phone_numbers = '";
    for (auto it = chng_contact.phone_numbers.begin(); it != chng_contact.phone_numbers.end(); it++) {
        sql += *it;
        if (it != chng_contact.phone_numbers.end() - 1)
            sql += '|';
    }
    sql += "' WHERE id = ";
    sql += to_string(chng_contact.u_id);
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);      // очищаем ресурсы
        sqlite3_close(db);
        return;
    }
    sqlite3_close(db);
}

void add_contact_db(Contact& chng_contact) {
    sqlite3* db;
    char* err_msg = 0;
    int rc = sqlite3_open("contacts.db", &db);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return;
    }
    string sql = "INSERT INTO people(name, surname, middle_name, email, address, date_of_birth, phone_numbers) "
        "VALUES ('" + chng_contact.name + "', '" + chng_contact.surname + "', '" + chng_contact.middle_name + "', '" + chng_contact.mail + "', '" + chng_contact.address + "', '" + chng_contact.date_of_birth + "', '";
    for (auto it = chng_contact.phone_numbers.begin(); it != chng_contact.phone_numbers.end(); it++) {
        sql += *it;
        if (it != chng_contact.phone_numbers.end() - 1)
            sql += '|';
    }
    sql += "');";
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);      // очищаем ресурсы
        sqlite3_close(db);
        return;
    }
    sql.clear();
    sqlite3_close(db);
}

class ButtonBlock : public QWidget {
    Q_OBJECT
public:
    QPushButton* b_add_contact = new QPushButton(QString("add contact"), this);
    QPushButton* b_delete = new QPushButton(QString("delete"), this);
    QPushButton* b_edit = new QPushButton(QString("edit"), this);

    ButtonBlock(QWidget* parent) : QWidget(parent) {
        b_add_contact->show();
        b_delete->show();
        b_edit->show();
    }
};

class AddContactWindow : public QWidget {
    Q_OBJECT

public:
    vector<Contact>* contacts_window;
    QPushButton* save = new QPushButton(QString("save"), this);
    QLineEdit* name = new QLineEdit(this);
    QLineEdit* surname = new QLineEdit(this);
    QLineEdit* middle_name = new QLineEdit(this);
    QLineEdit * email = new QLineEdit(this);
    QLineEdit* address = new QLineEdit(this);
    QDateEdit* date = new QDateEdit(this);
    QTextEdit* phone_numbers = new QTextEdit(this);

    AddContactWindow(vector<Contact>* contacts) : QWidget() {
        contacts_window = contacts;

        this->setMinimumSize(400, 300);

        QLabel* name_text = new QLabel("name", this);
        name_text->move(100, 40);
        name->move(180, 40);

        QLabel* surname_text = new QLabel("surname", this);
        surname_text->move(100, 60);
        surname->move(180, 60);

        QLabel* middle_name_text = new QLabel("middle_name", this);
        middle_name_text->move(100, 80);
        middle_name->move(180, 80);

        QLabel* email_text = new QLabel("email", this);
        email_text->move(100, 100);
        email->move(180, 100);

        QLabel* address_text = new QLabel("address", this);
        address_text->move(100, 120);
        address->move(180, 120);

        QLabel* date_text = new QLabel("date of birth", this);
        date_text->move(100, 140);
        date->move(180, 140);

        QLabel* phones_text = new QLabel("phones", this);
        phones_text->move(100, 160);
        phone_numbers->move(180, 160);
        phone_numbers->setMaximumSize(name->width(), 50);

        save->move(100, 180);
        connect(save, SIGNAL(clicked()), this, SLOT(save_contact()));
    }

    void closeEvent(QCloseEvent* event) {
        delete this;
    }

public slots:
    void save_contact() {
        qDebug() << phone_numbers->toPlainText();
        Contact new_contact;
        string tmp;
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
        if (contacts_window->size() == 0)
            new_contact.u_id = 1;
        else
            new_contact.u_id = (contacts_window->end() - 1)->u_id + 1;

        tmp = name->text().toStdString();
        if (check_name(name->text()))
            new_contact.name = tmp;
        else
            return;

        
        tmp = surname->text().toStdString();
        if (check_name(surname->text()))
            new_contact.surname = tmp;
        else
            return;

        tmp = middle_name->text().toStdString();
        if (check_name(middle_name->text()))
            new_contact.middle_name = tmp;
        else
            return;

        tmp = email->text().toStdString();
        if (check_mail(tmp)) {
            new_contact.mail = tmp;
        }
        else {
            QMessageBox* warn = new QMessageBox;
            warn->setText(QString::fromLocal8Bit("Неправильно введен email, повторите ввод"));
            warn->exec();
            return;
        }

        tmp = address->text().toStdString();
        new_contact.address = tmp;


        if (check_date(date->date())) {
            tmp = date->date().toString("yyyy.MM.dd").toStdString();
            new_contact.date_of_birth = tmp;
        }
        else {
            QMessageBox* warn = new QMessageBox;
            warn->setText(QString::fromLocal8Bit("Неправильная дата, повторите ввод"));
            warn->exec();
            return;
        }

        QStringList phones_spplitted = phone_numbers->toPlainText().split('\n');
        for (auto it = phones_spplitted.begin(); it != phones_spplitted.end(); it++) {
            if (check_phone(it->toStdString()))
                new_contact.phone_numbers.push_back(it->toStdString());
            else {
                QMessageBox* warn = new QMessageBox;
                warn->setText(QString::fromLocal8Bit("Неправильный телефон"));
                warn->exec();
                return;
            }
        }
        new_contact.print_detailed();
        contacts_window->push_back(new_contact);
        add_contact_db(new_contact);
    };
};

class ChangeContactWindow : public QWidget {
    Q_OBJECT
public:
    Contact* contact;
    QPushButton* update = new QPushButton(QString("update"), this);
    QLineEdit* name = new QLineEdit(this);
    QLineEdit* surname = new QLineEdit(this);
    QLineEdit* middle_name = new QLineEdit(this);
    QLineEdit* email = new QLineEdit(this);
    QLineEdit* address = new QLineEdit(this);
    QDateEdit* date = new QDateEdit(this);
    QTextEdit* phone_numbers = new QTextEdit(this);
    ChangeContactWindow(Contact* contact) : QWidget() {
        this->contact = contact;
        this->setMinimumSize(400, 300);

        QLabel* name_text = new QLabel("name", this);
        name_text->move(100, 40);
        name->setText(contact->name.c_str());
        name->move(180, 40);

        QLabel* surname_text = new QLabel("surname", this);
        surname_text->move(100, 60);
        surname->setText(contact->surname.c_str());
        surname->move(180, 60);

        QLabel* middle_name_text = new QLabel("middle_name", this);
        middle_name_text->move(100, 80);
        middle_name->setText(contact->middle_name.c_str());
        middle_name->move(180, 80);

        QLabel* email_text = new QLabel("email", this);
        email_text->move(100, 100);
        email->setText(contact->mail.c_str());
        email->move(180, 100);

        QLabel* address_text = new QLabel("address", this);
        address_text->move(100, 120);
        address->setText(contact->address.c_str());
        address->move(180, 120);

        QLabel* date_text = new QLabel("date of birth", this);
        date_text->move(100, 140);
        date->setDate(QDate::fromString(contact->date_of_birth.c_str(), "yyyy.MM.dd"));
        date->move(180, 140);

        QString phone_string = "";
        for (auto it = contact->phone_numbers.begin(); it != contact->phone_numbers.end(); it++) {
            phone_string += it->c_str();
            if (it != contact->phone_numbers.end() - 1)
                phone_string += '\n';
        }
        QLabel* phones_text = new QLabel("phones", this);
        phones_text->move(100, 160);
        phone_numbers->move(180, 160);
        phone_numbers->setText(phone_string);
        phone_numbers->setMaximumSize(name->width(), 50);

        update->move(100, 180);
        connect(update, SIGNAL(clicked()), this, SLOT(update_contact()));
    }

    void closeEvent(QCloseEvent* event) {
        delete this;
    }

public slots:
    void update_contact() {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
        qDebug() << phone_numbers->toPlainText();
        string tmp;

        tmp = name->text().toStdString();
        if (check_name(name->text()))
            contact->name = tmp;
        else
            return;

        tmp = surname->text().toStdString();
        if (check_name(surname->text()))
            contact->surname = tmp;
        else
            return;

        tmp = middle_name->text().toStdString();
        if (check_name(middle_name->text()))
            contact->middle_name = tmp;
        else
            return;

        tmp = email->text().toStdString();
        if (check_mail(tmp)) {
            contact->mail = tmp;
        }
        else {
            QMessageBox* warn = new QMessageBox;
            warn->setText(QString::fromLocal8Bit("Неправильно введен email, повторите ввод"));
            warn->exec();
        }

        tmp = address->text().toStdString();
        contact->address = tmp;


        if (check_date(date->date())) {
            tmp = date->date().toString("yyyy.MM.dd").toStdString();
            contact->date_of_birth = tmp;
        }
        else {
            QMessageBox* warn = new QMessageBox;
            warn->setText(QString::fromLocal8Bit("Неправильная дата, повторите ввод"));
            warn->exec();
            return;
        }

        contact->phone_numbers = {};
        QStringList phones_spplitted = phone_numbers->toPlainText().split('\n');
        for (auto it = phones_spplitted.begin(); it != phones_spplitted.end(); it++) {
            if (check_phone(it->toStdString()))
                contact->phone_numbers.push_back(it->toStdString());
            else {
                QMessageBox* warn = new QMessageBox;
                warn->setText(QString::fromLocal8Bit("Неправильный телефон"));
                warn->exec();
                return;
            }
        }
        contact->print_detailed();
        change_contact_db(*contact);
    };
    void delete_w() {
        delete this;
    }
};

class ContactList : public QTableWidget {
    Q_OBJECT

public:
    vector<Contact>* contacts_list;
    ContactList(QWidget* parent, vector<Contact>& contacts) : QTableWidget(parent) {
        contacts_list = &contacts;
        QString tmp = "";
        this->setRowCount(contacts.size());
        this->setColumnCount(8);
        this->setHorizontalHeaderLabels({ "id", "name", "surname", "middle name", "email", "address", "date of birth", "phone numbers" });
        for (int i = 0; i < contacts.size(); i++) {
            this->setItem(i, 0, new QTableWidgetItem(to_string(contacts[i].u_id).c_str()));
            this->setItem(i, 1, new QTableWidgetItem(QString(contacts[i].name.c_str())));
            this->setItem(i, 2, new QTableWidgetItem(QString(contacts[i].surname.c_str())));
            this->setItem(i, 3, new QTableWidgetItem(QString(contacts[i].middle_name.c_str())));
            this->setItem(i, 4, new QTableWidgetItem(QString(contacts[i].mail.c_str())));
            this->setItem(i, 5, new QTableWidgetItem(QString(contacts[i].address.c_str())));
            this->setItem(i, 6, new QTableWidgetItem(QString(contacts[i].date_of_birth.c_str())));
            for (auto it = contacts[i].phone_numbers.begin(); it != contacts[i].phone_numbers.end(); it++) {
                tmp += it->c_str();
                tmp += " ";
            }
            this->setItem(i, 7, new QTableWidgetItem(tmp));
            tmp = "";
        }
        this->setFixedHeight(400);
        this->setMaximumWidth(1000);
        this->setMinimumWidth(800);
        QHeaderView* test_w = this->horizontalHeader();
        connect((QObject*)this->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(show_sorted(int)));
    }
    void refresh_table_search(int field, const QString& target) { 
        vector<Contact>& contacts = *contacts_list;
        vector<Contact*> tmp_contacts = {};
        for (auto it = contacts_list->begin(); it != contacts_list->end(); it++) {
            if (field == 0 && it->u_id == target.toInt())
                tmp_contacts.push_back(&(*it));
            else if (field == 1 && it->name.find(target.toStdString()) != -1)
                tmp_contacts.push_back(&(*it));
            else if (field == 2 && it->surname.find(target.toStdString()) != -1)
                tmp_contacts.push_back(&(*it));
            else if (field == 3 && it->middle_name.find(target.toStdString()) != -1)
                tmp_contacts.push_back(&(*it));
            else if (field == 4 && it->mail.find(target.toStdString()) != -1)
                tmp_contacts.push_back(&(*it));
            else if (field == 5 && it->address.find(target.toStdString()) != -1)
                tmp_contacts.push_back(&(*it));
            else if (field == 6 && it->date_of_birth.find(target.toStdString()) != -1)
                tmp_contacts.push_back(&(*it));
            else if (field == 7) {
                for (auto itp = it->phone_numbers.begin(); itp != it->phone_numbers.end(); itp++) {
                    if (itp->find(target.toStdString()) != -1) {
                        tmp_contacts.push_back(&(*it));
                        break;
                    }
                }
            }
        }

        QString tmp = "";
        this->setRowCount(tmp_contacts.size());
        this->setColumnCount(8);
        this->setHorizontalHeaderLabels({ "id", "name", "surname", "middle name", "email", "address", "date of birth", "phone numbers" });
        for (int i = 0; i < tmp_contacts.size(); i++) {
            this->setItem(i, 0, new QTableWidgetItem(to_string(tmp_contacts[i]->u_id).c_str()));
            this->setItem(i, 1, new QTableWidgetItem(QString(tmp_contacts[i]->name.c_str())));
            this->setItem(i, 2, new QTableWidgetItem(QString(tmp_contacts[i]->surname.c_str())));
            this->setItem(i, 3, new QTableWidgetItem(QString(tmp_contacts[i]->middle_name.c_str())));
            this->setItem(i, 4, new QTableWidgetItem(QString(tmp_contacts[i]->mail.c_str())));
            this->setItem(i, 5, new QTableWidgetItem(QString(tmp_contacts[i]->address.c_str())));
            this->setItem(i, 6, new QTableWidgetItem(QString(tmp_contacts[i]->date_of_birth.c_str())));
            for (auto it = tmp_contacts[i]->phone_numbers.begin(); it != tmp_contacts[i]->phone_numbers.end(); it++) {
                tmp += it->c_str();
                tmp += " ";
            }
            this->setItem(i, 7, new QTableWidgetItem(tmp));
            tmp = "";
        }
        this->setFixedHeight(400);
        this->setMaximumWidth(1000);
        this->setMinimumWidth(800);
    }
public slots:
    void delete_row() {
        vector<int> ids_to_delete = {};
        int rows_to_delete = 0;
        int row_id = 0;
        if (this->selectedRanges().size() > 0) {
            rows_to_delete = this->selectedRanges().begin()->bottomRow() + 1 - selectedRanges().begin()->topRow();
            row_id = selectedRanges().begin()->topRow();
        }

        for (int i = 0; i < rows_to_delete; i++) {
            for (auto it = contacts_list->begin(); it != contacts_list->end(); it++)
                if (it->u_id == this->item(row_id, 0)->text().toInt()) {
                    ids_to_delete.push_back(it->u_id);
                    contacts_list->erase(it);
                    break;
                }
            removeRow(row_id);
        }
        delete_contact_db(ids_to_delete);    
    }
    void create_contact() {
        
        AddContactWindow* add_w = new AddContactWindow(contacts_list);
        connect(add_w->save, SIGNAL(clicked()), this, SLOT(refresh_table()));
        add_w->show();
    }

    void update_contact() {
        int id = 0;
        if (this->selectedRanges().size() > 0) {
            if (this->selectedRanges().begin()->bottomRow() == selectedRanges().begin()->topRow()) {
                id = this->item(this->selectedRanges().begin()->bottomRow(), 0)->data(Qt::DisplayRole).toInt();
                qDebug() << id;
                for (auto it = contacts_list->begin(); it != contacts_list->end(); it++) {
                    if (it->u_id == id) {
                        ChangeContactWindow* add_w = new ChangeContactWindow(&(*it));
                        connect(add_w->update, SIGNAL(clicked()), this, SLOT(refresh_table()));
                        add_w->show();
                        break;
                    }

                }
            }      
        }
    }

    void refresh_table() {
        vector<Contact>& contacts = *contacts_list;
        QString tmp = "";
        this->setRowCount(contacts.size());
        this->setColumnCount(8);
        this->setHorizontalHeaderLabels({ "id", "name", "surname", "middle name", "email", "address", "date of birth", "phone numbers" });
        for (int i = 0; i < contacts.size(); i++) {
            this->setItem(i, 0, new QTableWidgetItem(to_string(contacts[i].u_id).c_str()));
            this->setItem(i, 1, new QTableWidgetItem(QString(contacts[i].name.c_str())));
            this->setItem(i, 2, new QTableWidgetItem(QString(contacts[i].surname.c_str())));
            this->setItem(i, 3, new QTableWidgetItem(QString(contacts[i].middle_name.c_str())));
            this->setItem(i, 4, new QTableWidgetItem(QString(contacts[i].mail.c_str())));
            this->setItem(i, 5, new QTableWidgetItem(QString(contacts[i].address.c_str())));
            this->setItem(i, 6, new QTableWidgetItem(QString(contacts[i].date_of_birth.c_str())));
            for (auto it = contacts[i].phone_numbers.begin(); it != contacts[i].phone_numbers.end(); it++) {
                tmp += it->c_str();
                tmp += " ";
            }
            this->setItem(i, 7, new QTableWidgetItem(tmp));
            tmp = "";
        }
        this->setFixedHeight(400);
        this->setMaximumWidth(1000);
        this->setMinimumWidth(800);
    }
    void show_sorted(int column) {
        qDebug() << column;
        this->sortByColumn(column);
    }
    
};

class SearchBlock : public QWidget {
    Q_OBJECT
public:
    QPushButton* search = new QPushButton(QString("search"), this);
    QLineEdit* search_field = new QLineEdit(this);
    QComboBox* field_option = new QComboBox(this);
    SearchBlock(QWidget* parent) : QWidget(parent) {
        field_option->addItem("id");
        field_option->addItem("name");
        field_option->addItem("surname");
        field_option->addItem("middle_name");
        field_option->addItem("email");
        field_option->addItem("address");
        field_option->addItem("date");
        field_option->addItem("phone_numbers");
        field_option->show();
        search->show();
        search_field->show();
    }
};

class MainContactWindow : public QWidget {
    Q_OBJECT
public:
    ContactList* contact_table;
    ButtonBlock* buttons;
    SearchBlock* search_buttons;
    vector<Contact>* contacts;
    vector<AddContactWindow>* add_windows;
    vector<ChangeContactWindow>* chng_windows;
    MainContactWindow(vector<Contact>& contacts) : QWidget() {
        this->contacts = &contacts;
        contact_table = new ContactList(this, contacts);
        buttons = new ButtonBlock(this);
        buttons->move(30, contact_table->height() + 30);
        buttons->b_delete->move(30, 0);
        buttons->b_add_contact->move(110, 0);
        buttons->b_edit->move(190, 0);
        
        search_buttons = new SearchBlock(this);
        search_buttons->move(300, contact_table->height() + 30);
        search_buttons->field_option->move(110, 0);
        search_buttons->search_field->move(230, 0);
        search_buttons->search->move(470, 0);
        contact_table->raise();

        connect(buttons->b_delete, SIGNAL(clicked()), contact_table, SLOT(delete_row()));
        connect(buttons->b_add_contact, SIGNAL(clicked()), contact_table, SLOT(create_contact()));
        connect(buttons->b_edit, SIGNAL(clicked()), contact_table, SLOT(update_contact()));
        connect(search_buttons->search, SIGNAL(clicked()), this, SLOT(search()));

        this->setMinimumSize(1000, 500);
    };
public slots:
    void search() {
        qDebug() << search_buttons->field_option->currentIndex();
        if (search_buttons->search_field->text().size() == 0)
            this->contact_table->refresh_table();
        else
            contact_table->refresh_table_search(search_buttons->field_option->currentIndex(), search_buttons->search_field->text());
    }
};

int callback(void* contacts, int colCount, char** columns, char** colNames) {
    vector<Contact>* tmp_contacts = (vector<Contact>*)contacts;
    Contact new_contact;
    new_contact.u_id = stoi(string(columns[0]));
    new_contact.name = columns[1];
    new_contact.surname = columns[2];
    new_contact.middle_name = columns[3];
    new_contact.mail = columns[4];
    new_contact.address = columns[5];
    new_contact.date_of_birth = columns[6];
    QStringList tmp = QString(columns[7]).split('|');
    for (int i = 0; i < tmp.size(); i++) {
        new_contact.phone_numbers.push_back(tmp[i].toStdString());
    }
    tmp_contacts->push_back(new_contact);
    return 0;
}

#include "main.moc"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("KOI8-R"));
    setlocale(LC_ALL, "RU");
    
    string contact_str;
    vector<Contact> contacts = {};

    sqlite3* db;
    char* err_msg = 0;
    int rc = sqlite3_open("contacts.db", &db);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return 1;
    }
    const char* sql = "CREATE TABLE IF NOT EXISTS people(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, surname TEXT, middle_name TEXT, email TEXT, address TEXT, date_of_birth TEXT, phone_numbers TEXT);";
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);      // очищаем ресурсы
        sqlite3_close(db);
        return 1;
    }

    sql = "SELECT * FROM people";
    rc = sqlite3_exec(db, sql, callback, &contacts, &err_msg);
     
    sqlite3_close(db);
    QApplication a(argc, argv);
    MainContactWindow* mw = new MainContactWindow(contacts);
    mw->show();
    return a.exec();
}