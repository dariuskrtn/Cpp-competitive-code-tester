#include <windows.h>
#include <fstream>
#include <tchar.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;
double time;

vector<string> get_files(string folder);
string getexepath();
void write (const char path[], const char dest[]);
HANDLE open(const char path[]);
double run(const char path[]);
string compare_sol(const char in1[], const char in2[]);
bool config_exists();
void getConfig(string &exe, string &input, string &output);
void create_config(string exe, string input, string output);
int main()
{
    string path = getexepath()+"\\test";
    cout << path << endl;

    string input, output, exe;
    getConfig(exe, input, output);

    //open(exe.c_str());
    vector<string> files = get_files(path);
    int testai=0;
    int ac=0;
    for (int i=0; i<files.size(); i++) {
        if (files[i].length()<3) continue;
        if (files[i].substr(files[i].size()-3, files[i].size())!=".in") continue;
        testai++;
        string in = ".\\test\\"+files[i];
        write(in.c_str(), input.c_str());
        cout << files[i] << " - ";
        double laikas = run(exe.c_str());
        if (laikas>=time) {
            cout << "Failed! (Time limit exceeded)" << endl;
            continue;
        }
        string out = in.substr(0, in.length()-3)+".sol";
        string res = compare_sol(output.c_str(), out.c_str());
        if (res!="") {
            cout << "Failed! (Wrong answer) - " << res << endl;
            continue;
        }
        ac++;
        cout << "Accepted! " << fixed << setprecision(2) << laikas << " sec." << endl;
        //cout << out << endl;
        //write(out.c_str(), output.c_str());
    }
    cout << ac << "/" << testai << " Tests passed" << endl;
    system("pause");
    return 0;
}
void getConfig(string &exe, string &input, string &output)
{
    if (config_exists()) {
    ifstream in("conf.txt");
    in >> exe >> time >> input >> output;
    in.close();
    } else {
    cout << "You can use config file 'conf.txt'" << endl;
    cout << ".exe file: ";
    cin >> exe;
    cout << "Execution time: ";
    cin >> time;
    cout << "Input file: ";
    cin >> input;
    cout << "Output file: ";
    cin >> output;
    cout << "Config created automatically 'conf.txt'" << endl;
    create_config(exe, input, output);
    }
}
bool config_exists()
{
    ifstream in("conf.txt");
    bool t = in.good();
    in.close();
    return t;
}
void create_config(string exe, string input, string output)
{
    ofstream out("conf.txt");
    out << exe << endl;
    out << time << endl;
    out << input << endl;
    out << output << endl;
    out.close();
}
string compare_sol(const char in1[], const char in2[])
{
ifstream in(in1);
ifstream inn(in2);
string t1, t2;
while (!in.eof()) {
    getline(in, t1);
    getline(inn, t2);
    if (t1!=t2 && t1!="") {
        return t1 + " != " + t2;
    }
}
in.close();
inn.close();
return "";
}
double run(const char path[])
{
    HANDLE programa;
    programa = open(path);
    double i=0;
    while (i<time) {
        Sleep(50);
        i+=0.05;
        DWORD exitCode = 0;
        GetExitCodeProcess(programa, &exitCode);
        if (exitCode != STILL_ACTIVE) {
            break;
        }
    }
    if (i>=time) {
    TerminateProcess(programa, 1);
    return time;
    }
    return i;
}

void write (const char path[], const char dest[])
{
    ifstream in(path);
    ofstream out(dest);
    string t;
    while (!in.eof()) {
    getline(in, t);
    out << t << endl;
    }
    in.close();
    out.close();
}
vector<string> get_files(string folder)
{
    vector<string> names;
    char search_path[200];
    sprintf(search_path, "%s/*.*", folder.c_str());
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path, &fd);
    if(hFind != INVALID_HANDLE_VALUE) {
        do {
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                names.push_back(fd.cFileName);
            }
        }while(::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
    return names;
}

string getexepath()
{
  char result[ MAX_PATH ];
  string s = string( result, GetModuleFileName( NULL, result, MAX_PATH ) );
  for (int i=s.length()-1; i>=0; i--) {
    if (s[i]=='\\') {
        s=s.substr(0, i);
        break;
    }
  }
  return s;
}

HANDLE open(const char path[])
{
    	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	bool procesas =
	CreateProcess
			(
			TEXT(path),
			NULL,NULL,NULL,FALSE,
			CREATE_NEW_CONSOLE,
			NULL,NULL,
			&si,
			&pi
			);
	if (!procesas) {
			cout << "Unable to execute.";
		}
		return pi.hProcess;
}
