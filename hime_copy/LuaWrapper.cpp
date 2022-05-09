#include "LuaWrappter.h"


namespace LUA {
	double Get_グローバル数値(string 変数名) {
		try {
			double ret = lua_getglobal_numtype((char *)変数名.c_str());
			return ret;
		}
		catch (...) {
			throw LUA::例外型("LUA::Get_グローバル数値エラー");
		}
	}

	string Get_グローバル文字列(string 変数名) {
		try {
			string ret = lua_getglobal_strtype((char *)変数名.c_str());
			return ret;
		}
		catch (...) {
			throw LUA::例外型("LUA::Get_グローバル文字列エラー");
		}
	}


	int Do_コマンド(string コマンド文字列) {
		int ret = lua_DoStringFormat(コマンド文字列.c_str());
		if (ret) {
			throw LUA::例外型("LUA::Do_コマンドエラー");
		}
		else {
			return ret;
		}
	}

	int Do_ファイル(string ファイル名) {
		string コマンド文字列 = "dofile('" + ファイル名 + "')";
		int ret = lua_DoStringFormat(コマンド文字列.c_str());
		if (ret) {
			throw LUA::例外型("LUA::Do_ファイルエラー");
		}
		else {
			return ret;
		}
	}

}

ostream& operator<<(ostream& os, LUA::例外型& e)
{
	os << e.what();
	return os;
}
