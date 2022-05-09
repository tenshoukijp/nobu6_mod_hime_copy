#include "LuaWrappter.h"

#include <stdlib.h>
#include <time.h>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

#include <windows.h>

struct myパラメタ型 {
	int 番号;
	string パターン;
	int 個数;
};

vector<myパラメタ型> vBmpDataParam;

// Lua_ImportDataParam に相当する関数。1人分のデータを受け取って、そのままC++内の配列へと付け足す。
int Lua_ImportDataParam(lua_State *L) {

	myパラメタ型 data;


	if (lua_isnumber(L, 1)) {
		data.番号 = lua_tointeger(L, 1);
	}

	if (lua_isstring(L, 2)) {
		data.パターン = lua_tostring(L, 2);
	}

	if (lua_isnumber(L, 3)) {
		data.個数 = lua_tointeger(L, 3);
	}

	// C++内のリスト変数にdata追加
	vBmpDataParam.push_back(data);

	// Lに最後に書いた引数群を削除
	lua_pop(L, lua_gettop(L));

	// この関数が呼ばれた際に返す返り値の個数を返す
	return 0;
}

void ImportSettingParam() {
	try {
		// Lua_ImportDataParam関数 を  Lua_ImportDataParam という名前でLに登録
		lua_register(L, "Lua_ImportDataParam", &Lua_ImportDataParam);

		// ファイルを読み込んで実行
		LUA::Do_ファイル("hime_copy_set.txt");


		LUA::Do_コマンド
			(
			"for i, data in ipairs(list_hime_bmp) do\n"
			"    Lua_ImportDataParam(i, data[1], data[2])\n"
			"end\n"
			);

	}
	catch (LUA::例外型& e) {
		cerr << e.what() << endl;
	}
}


vector<string> GetFileList(string szFilePattern) {
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;

	vector<string> bmp_file_list;

	// ワイルドカードからファイル名群を取り出す。
	// 同じカウント数を１グループとする。
	hFind = FindFirstFile(szFilePattern.c_str(), &win32fd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return bmp_file_list;
	}

	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			; // ディレクトリは無視
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
	// 設定ファイルを元に、ファイル一覧を「数」をキー、「数に対応するファイルリスト」を値という形で詰め込んでいく
	for (auto& data : vBmpDataParam) {
		auto addlist = GetFileList(data.パターン);
		int num = data.個数;

		// カウント同じの存在したら、そこにファイル群を足す(# 始めてだったら、登録)
		auto curlist = numberListHash[num];
		// リストの追加
		std::copy(addlist.begin(), addlist.end(), std::back_inserter(curlist));
		numberListHash[num] = curlist;
	}

	// マップは常にキーでソートされる(自動) # 自動的に繰り返し数が少ないものでソートされた状態
	/*
	for (auto value : numberListHash) {
		cout << "キー" << value.first << endl;
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
			// 繰り返し分だけ
			for (int i = 0; i < iter.first; i++) {
				// リストを足しこむ
				std::copy(iter.second.begin(), iter.second.end(), std::back_inserter(copylist));
			}
		}
		else {
			// 768未満ならもっぺん足す
			while (copylist.size() < hime_count) {
				std::copy(iter.second.begin(), iter.second.end(), std::back_inserter(copylist));
			}
		}
	}

	// 姫数まで切り捨て
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
		CopyFile(szExistFileName, szNewFileName, FALSE); // ファイルが存在しても上書き
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
LPCTSTR lpExistingFileName, // 既存のファイルの名前
LPCTSTR lpNewFileName,      // 新しいファイルの名前
BOOL bFailIfExists          // ファイルが存在する場合の動作
);
*/

/*
;*/