// flipAdaptiveBrightness.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <memory>
#include <cstdio>

using namespace std;

string exec(const char* cmd) {
	shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) return "ERROR";
	char buffer[256];
	std::string result = "";
	while (!feof(pipe.get())) {
		if (fgets(buffer, 256, pipe.get()) != NULL)
			result += buffer;
	}
	return result;
}

string findCurrentAdaptiveValue(string status)
{
	int point = status.find("Current AC Power Setting Index: 0x0000000");
	cout << "String last position: " << point << endl;
	string currentVal = status.substr(point+41, 1);
	return currentVal;
}

int main()
{
	string status = exec("powercfg /q SCHEME_CURRENT SUB_VIDEO ADAPTBRIGHT");
	cout << "(" << status << ")" << endl;
	string currentVal = findCurrentAdaptiveValue(status);

	if (!(currentVal == "0" || currentVal == "1"))
	{
		cout << "Currentval not expected: " << currentVal << endl;
		system("pause");
		return 1;
	}

	cout << "Current AC value: (" << currentVal << ")" << endl;
	if (currentVal == "1")
	{
		system("powercfg /SETACVALUEINDEX SCHEME_CURRENT SUB_VIDEO ADAPTBRIGHT 000");
		cout << "Set value to: 0" << endl;
	}
	else
	{
		system("powercfg /SETACVALUEINDEX SCHEME_CURRENT SUB_VIDEO ADAPTBRIGHT 001");
		cout << "Set value to: 1" << endl;
	}

	system("sc stop SensrSvc");
	system("sc start SensrSvc");
	//system("pause");
}

