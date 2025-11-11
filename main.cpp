#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <termios.h>
#include <unistd.h>
#include "linenoise.h"
using namespace std;

void clear() {
	system("clear");
	return;
}

void startup() {
    clear();
	cout << R"( _     _____ ____  )" << "\n";
	cout << R"(| |   |  ___/ ___| )" << "\n";
	cout << R"(| |   | |_  \___ \ )" << "\n";
	cout << R"(| |___|  _|  ___) |)" << "\n";
	cout << R"(|_____|_|   |____/ )" << "\n";
	cout << "Welcome to LinuxFromScratch aka LFS" << "\n";
    cout << "Version 1.0.0" << "\n";
	return;
}

void calculator() {
    vector<string> COMMANDS = {
        "<expression>",
        "clear",
        "exit"
    };
	cout << "COMMANDS: " << "\n";
    for (string & COMMAND : COMMANDS) cout << COMMAND << "\n";
    string cmd = "python3 calc.py";
    system(cmd.c_str());
	return;
}

struct Entry { string site, username, password; };
const string FILENAME = "passwords.gpg";
string get_hidden_input() {
    string pass;
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    getline(cin, pass);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    cout << "\n";
    return pass;
}
bool save_data(const string &master, const vector<Entry> &data) {
    string tmpfile = "/tmp/passwords.tmp";
    ofstream fout(tmpfile, ios::trunc);
    if (!fout) return false;
    for (const auto &e : data)
        fout << e.site << "\n" << e.username << "\n" << e.password << "\n";
    fout.close();
    string cmd = "gpg --symmetric --cipher-algo AES256 "
                 "--batch --yes --pinentry-mode loopback "
                 "--passphrase \"" + master + "\" "
                 "-o " + FILENAME + " " + tmpfile;
    system(cmd.c_str());
    system(("shred -u " + tmpfile).c_str());
    return true;
}
bool load_data(const string &master, vector<Entry> &data) {
    string tmpfile = "/tmp/passwords.tmp";
    string cmd = "gpg --decrypt --batch --yes --pinentry-mode loopback "
                 "--passphrase \"" + master + "\" "
                 "-o " + tmpfile + " " + FILENAME;
    if (system(cmd.c_str()) != 0) return false;
    ifstream fin(tmpfile);
    if (!fin) return false;
    Entry e;
    while (getline(fin, e.site) && getline(fin, e.username) && getline(fin, e.password)) {
        if (e.site.empty() && e.username.empty() && e.password.empty()) continue;
        data.push_back(e);
    }
    fin.close();
    system(("shred -u " + tmpfile).c_str());
    return true;
}
void passwdmn() {
    vector<Entry> data;
    string master;
    cout << "Enter master password: ";
    master = get_hidden_input();
    if (!load_data(master, data)) {
        cout << "Failed to decrypt password file!" << "\n";
        return;
    }
    vector<string> COMMANDS = {
    	"list",
    	"add",
    	"view",
        "find",
        "edit",
        "remove",
        "clear",
        "exit"
    };
    string input;
    char* raw;
    while ((raw = linenoise("[psswdmn] >>> ")) != NULL) {
        if (*raw) linenoiseHistoryAdd(raw);
        input = raw;
        linenoiseFree(raw);
        if (input == "exit") break;
        if (input.empty()) continue;
        if (input == "clear") {system("clear"); continue;}
        if (input == "list") {
            cout << "COMMANDS:\n";
            for (auto &c : COMMANDS) cout << c << "\n";
            continue;
        }
        if (input == "add") {
            Entry e;
            cout << "Enter site name: "; getline(cin, e.site);
            cout << "Enter username: "; getline(cin, e.username);
            cout << "Enter password: "; getline(cin, e.password);
            data.push_back(e);
            save_data(master, data);
            continue;
        }
        if (input == "view") {
            int index = 1;
            for (auto &e : data) {
                cout << "[" << index << "]" << "\n"; 
                cout << e.site << "\n" << e.username << "\n" << e.password << "\n\n";
                ++index;
            }
            if (data.empty()) cout << "No entries!" << "\n";
            continue;
        }
        if (input == "find") {
            cout << "Search: "; string key; getline(cin,key);
            bool found = false;
            for (auto &e : data) {
                if (e.site.find(key) != string::npos) {
                    cout << e.site << "  " << e.username << "  " << e.password << "\n";
                    found = true;
                }
            }
            if (!found) cout << "No matches!" << "\n";
            continue;
        }
        if (input == "edit") {
            cout << "Enter index: "; string idxs; getline(cin, idxs);
            int idx = stoi(idxs);
            if (idx < 1 || idx > (int)data.size()) { cout << "INDEX OUT OF RANGE!\n"; continue; }
            Entry &e = data[idx-1];
            string tmp;
            cout << "Current site: " << e.site << "\n" << "New site: "; 
            getline(cin,tmp); 
            if (!tmp.empty()) e.site=tmp;
            cout << "Current username: " << e.username << "\n" << "New username: "; 
            getline(cin,tmp); 
            if (!tmp.empty()) e.username=tmp;
            cout << "Current password: " << e.password << "\n" << "New password: "; 
            getline(cin,tmp); 
            if (!tmp.empty()) e.password=tmp;
            save_data(master,data);
            continue;
        }
        if (input == "remove") {
            cout << "Enter index: "; string idxs; getline(cin, idxs);
            int idx = stoi(idxs);
            if (idx < 1 || idx > (int)data.size()) {
                cout << "INDEX OUT OF RANGE!" << "\n"; 
                continue;
            }
            data.erase(data.begin()+idx-1);
            save_data(master,data);
            continue;
        }
        cout << "INVALID COMMAND!" << "\n";
    }
}

void archinstall() {
    char* raw;
    vector<string> COMMANDS = {
        "list",
        "grub",
        "pacman",
        "yay",
        "swap",        
        "settingxfce",
        "database",
        "cleanup",
        "dns",
        "clear",
        "exit"
    };
    system("chmod +x ./grub.sh ./pacman.sh ./swap.sh ./yay.sh ./settingxfce.sh ./database.sh ./dns.sh ./cleanup.sh 2>/dev/null");
    while ((raw = linenoise("[archinstall] >>> ")) != NULL) {
        if (*raw) linenoiseHistoryAdd(raw);
        string input = raw;
        linenoiseFree(raw);
        if (input == "exit") break;
        if (input == "") continue;
        if (input == "clear") {
            clear();
            continue;
        }
        if (input == "list") {
            cout << "COMMANDS: " << "\n";
            for (string &COMMAND : COMMANDS) cout << COMMAND << "\n";
            continue;
        }
        if (input == "grub") {
            string COMMAND = "sudo ./grub.sh";
            system(COMMAND.c_str());
            continue;
        }
        if (input == "pacman") {
            string COMMAND = "sudo ./pacman.sh";
            system(COMMAND.c_str());
            continue;
        }
        if (input == "swap") {
            string COMMAND = "sudo ./swap.sh";
            system(COMMAND.c_str());
            continue;
        }
        if (input == "yay") {
            string COMMAND = "./yay.sh";
            system(COMMAND.c_str());
            continue;
        }
        if (input == "settingxfce") {
            string COMMAND = "./settingxfce.sh";
            system(COMMAND.c_str());
            continue;
        }
        if (input == "database") {
        	string COMMAND = "sudo ./database.sh";
            system(COMMAND.c_str());
            continue;
        }        
        if (input == "dns") {
            string COMMAND = "sudo ./dns.sh";
            system(COMMAND.c_str());
            continue;
        }
        if (input == "cleanup") {
        	string COMMAND = "./cleanup.sh";
            system(COMMAND.c_str());
            continue;
        }
        cout << "INVALID COMMAND!" << "\n";
    }
    return;
}

void command(string input) {
    vector<string> COMMANDS = {
        "help",
        "list",
        "calc",
        "passwdmn",
        "archinstall",
        "clear",
        "exit"
    };
	if (input == "list") {
		cout << "COMMANDS:" << "\n";
        for (string &COMMAND : COMMANDS) cout << COMMAND << "\n";
		return;
	} 
    if (input == "help") {
        string COMMAND = "cat README.md";
        system(COMMAND.c_str());
        return;
    }
	if (input == "calc") {
		calculator();
		return;
	} 
	if (input == "passwdmn") {
		passwdmn();
		return;
	} 
    if (input == "archinstall") {
        archinstall();
        return;
    }
	if (input == "clear") {
		clear();
		return;
	}
	cout << "INVALID COMMAND!";
	cout << "\n";
	return;
}

int main() {
    startup();
    char* raw;
    while ((raw = linenoise(">>> ")) != NULL) {
        if (*raw) linenoiseHistoryAdd(raw);
        string input = raw;
        linenoiseFree(raw);
        if (input == "exit") return 0;
        if (input.empty()) continue;
        command(input);
        cout << "\n";
    }
    return 0;
}