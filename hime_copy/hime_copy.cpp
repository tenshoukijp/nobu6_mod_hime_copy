#include "LuaWrappter.h"

#include <stdlib.h>
#include <time.h>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

#include <windows.h>

struct my�p�����^�^ {
	int �ԍ�;
	string �p�^�[��;
	int ��;
};

vector<my�p�����^�^> vBmpDataParam;

// Lua_ImportDataParam �ɑ�������֐��B1�l���̃f�[�^���󂯎���āA���̂܂�C++���̔z��ւƕt�������B
int Lua_ImportDataParam(lua_State *L) {

	my�p�����^�^ data;


	if (lua_isnumber(L, 1)) {
		data.�ԍ� = lua_tointeger(L, 1);
	}

	if (lua_isstring(L, 2)) {
		data.�p�^�[�� = lua_tostring(L, 2);
	}

	if (lua_isnumber(L, 3)) {
		data.�� = lua_tointeger(L, 3);
	}

	// C++���̃��X�g�ϐ���data�ǉ�
	vBmpDataParam.push_back(data);

	// L�ɍŌ�ɏ����������Q���폜
	lua_pop(L, lua_gettop(L));

	// ���̊֐����Ă΂ꂽ�ۂɕԂ��Ԃ�l�̌���Ԃ�
	return 0;
}

void ImportSettingParam() {
	try {
		// Lua_ImportDataParam�֐� ��  Lua_ImportDataParam �Ƃ������O��L�ɓo�^
		lua_register(L, "Lua_ImportDataParam", &Lua_ImportDataParam);

		// �t�@�C����ǂݍ���Ŏ��s
		LUA::Do_�t�@�C��("hime_copy_set.txt");


		LUA::Do_�R�}���h
			(
			"for i, data in ipairs(list_hime_bmp) do\n"
			"    Lua_ImportDataParam(i, data[1], data[2])\n"
			"end\n"
			);

	}
	catch (LUA::��O�^& e) {
		cerr << e.what() << endl;
	}
}


vector<string> GetFileList(string szFilePattern) {
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;

	vector<string> bmp_file_list;

	// ���C���h�J�[�h����t�@�C�����Q�����o���B
	// �����J�E���g�����P�O���[�v�Ƃ���B
	hFind = FindFirstFile(szFilePattern.c_str(), &win32fd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return bmp_file_list;
	}

	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			; // �f�B���N�g���͖���
		}
		else {
			bmp_file_list.push_back(win32fd.cFileName);
		}
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);

	return bmp_file_list;
}

map<int, vector<string>> numberListHash;

void BmpDataParamToNumberListHash() {
	// �ݒ�t�@�C�������ɁA�t�@�C���ꗗ���u���v���L�[�A�u���ɑΉ�����t�@�C�����X�g�v��l�Ƃ����`�ŋl�ߍ���ł���
	for (auto& data : vBmpDataParam) {
		auto addlist = GetFileList(data.�p�^�[��);
		int num = data.��;

		// �J�E���g�����̑��݂�����A�����Ƀt�@�C���Q�𑫂�(# �n�߂Ă�������A�o�^)
		auto curlist = numberListHash[num];
		// ���X�g�̒ǉ�
		std::copy(addlist.begin(), addlist.end(), std::back_inserter(curlist));
		numberListHash[num] = curlist;
	}

	// �}�b�v�͏�ɃL�[�Ń\�[�g�����(����) # �����I�ɌJ��Ԃ��������Ȃ����̂Ń\�[�g���ꂽ���
	/*
	for (auto value : numberListHash) {
		cout << "�L�[" << value.first << endl;
		for (auto v : value.second) {
			cout << v << ":";
		}
		cout << endl;
	}
	*/
}

const int hime_bgn = 3001;
const int hime_end = 3768;
const int hime_count = hime_end - hime_bgn + 1;

vector<string> copylist;
void MakeCopyList() {

	for (auto iter : numberListHash) {
		if (iter.first != 999) {
			// �J��Ԃ�������
			for (int i = 0; i < iter.first; i++) {
				// ���X�g�𑫂�����
				std::copy(iter.second.begin(), iter.second.end(), std::back_inserter(copylist));
			}
		}
		else {
			// 768�����Ȃ�����؂񑫂�
			while (copylist.size() < hime_count) {
				std::copy(iter.second.begin(), iter.second.end(), std::back_inserter(copylist));
			}
		}
	}

	// �P���܂Ő؂�̂�
	copylist.resize(hime_count);
}


void RandomShuffleCopyList() {
	srand((unsigned)time(NULL));
	std::random_shuffle(copylist.begin(), copylist.end());
}

void ReproductionCopy() {
	int i = 0;
	for (auto file : copylist) {
		cout << file << endl;
		char szNewFileName[256] = "";
		const char *szExistFileName = file.c_str();
		sprintf(szNewFileName, ".\\kao_24bit\\%04d.bmp", hime_bgn + i++);
		CopyFile(szExistFileName, szNewFileName, FALSE); // �t�@�C�������݂��Ă��㏑��
	}
}


void main() {

	LoadLuaInterpreter();

	ImportSettingParam();

	BmpDataParamToNumberListHash();

	MakeCopyList();

	RandomShuffleCopyList();

	ReproductionCopy();

	FreeLuaInterpreter();
}

/*
BOOL CopyFile(
LPCTSTR lpExistingFileName, // �����̃t�@�C���̖��O
LPCTSTR lpNewFileName,      // �V�����t�@�C���̖��O
BOOL bFailIfExists          // �t�@�C�������݂���ꍇ�̓���
);
*/

/*
;*/