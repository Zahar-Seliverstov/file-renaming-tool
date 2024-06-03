#include <iostream>
#include <filesystem>
#include <string>
#include <windows.h>
#include <shlobj.h>
#include <commdlg.h>
#include <vector>
#include <conio.h>

namespace fs = std::filesystem;

std::wstring OpenFileDialog() {

	std::wstring folderPath;

	BROWSEINFO bi = {};
	bi.lpszTitle = L"Select à folder";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != nullptr) {
		wchar_t path[MAX_PATH];
		if (SHGetPathFromIDList(pidl, path)) {
			folderPath = path;
		}
		CoTaskMemFree(pidl);
	}
	return folderPath;
}

int main() {

	system("chcp 1251");
	system("cls");

	std::vector<std::wstring> allFilesInFolder;
	std::wstring newFilesName, folderPath;

	std::cout << "### select a folder in the window that opens\n";

	folderPath = OpenFileDialog();

	std::wcout << L"\n### \x1b[90mselected folder path: \x1b[93m" + folderPath + L"\x1b[0m\n";

	try {
		if (fs::exists(folderPath) && !fs::is_empty(folderPath)) {
			std::cout << "\n### \x1b[92mfolder found\x1b[0m\n";
		}
		else {
			std::cerr << "\n### \x1b[91folder not found\n\x1b[0m";
		}
	}
	catch (const fs::filesystem_error& e) {
		std::cerr << "\n###\x1b[91m error to opening file\x1b[0m" << e.what() << std::endl;
	}

	std::cout << "\n### enter a new name for the files\n\x1b[90mnew name: \x1b[0m";
	while (true) {
		std::getline(std::wcin, newFilesName);
		if (newFilesName.length() != 0) {
			break;
		}
		else {
			system("cls");
			std::cout << "\n### \x1b[91mincorrect name entered try again\n\x1b[90mnew name: \x1b[0m";
		}
	}

	for (auto& file : fs::directory_iterator(folderPath)) {
		allFilesInFolder.push_back(file.path());
	}

	try {
		for (size_t i = 0; i < allFilesInFolder.size(); i++) {

			fs::path file(allFilesInFolder[i]);
			std::wstring buildingNewFileName =
				folderPath + L"\\" +
				newFilesName + L'_'
				+ std::to_wstring(i + 1) +
				file.extension().wstring();

			fs::rename(allFilesInFolder[i], buildingNewFileName);
		}
	}
	catch (const fs::filesystem_error& e) {
		std::cerr << "\n### \x1b[91merror during file renaming\x1b[0m\n" << e.what() << std::endl;
	}

	std::cout << "\n### \x1b[92mrenaming was successful\x1b[0m\n";
	std::cout << "\n### number of renamed files:\x1b[93m " << allFilesInFolder.size() << "\x1b[0m\n";
	std::cout << "\n\x1b[90m\tlist of modifications\x1b[0m\n\n";

	for (size_t i = 0; i < allFilesInFolder.size(); i++) {
		fs::path file(allFilesInFolder[i]);
		std::wcout << "\x1b[90m " << i + 1 << L" \x1b[93m| \x1b[0m"
			<< file.filename().wstring() + file.extension().wstring()
			<< L"\x1b[90m renamed to \x1b[0m" << newFilesName + L"_" +
			std::to_wstring(i + 1) + file.extension().wstring() << std::endl;
	}

	std::cout << "\n### \x1b[90mpress enter to start over or escape to exit\x1b[0m\n";

	while (true) {
		int key = _getch();
		switch (key) {
		case 13:
			main();
			break;
		case 27: return 0;
		}
	}
}
