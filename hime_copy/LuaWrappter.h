#pragma once

#include "LuaIF.h"
using namespace std;

namespace LUA {

	/// <summary>
	/// LUAの例外型。
	/// </summary>
	class 例外型 : public std::exception {
	public:
		例外型(std::string cause) : std::exception(cause.c_str()) {}
	};


	/// <summary>
	/// LUAの指定のグローバス変数の中身を「実数(小数)」とみなして、取得。
	/// <para>返り値：変数の中身を実数(少数)とみなして取得する。</para>
	/// <para>取得に失敗した場合、「LUA::例外型」の例外が投げられる。</para>
	/// </summary>
	/// <param name="変数名">中身を取得したいグローバル変数名</param>
	/// <returns>変数の中身である実数</returns>
	double Get_グローバル数値(string 変数名);


	/// <summary>
	/// LUAの対象のグローバス変数の中身を「文字列」とみなして、取得。
	/// <para>返り値：変数の中身を文字列とみなして取得する。</para>
	/// <para>取得に失敗した場合、「LUA::例外型」の例外が投げられる。</para>
	/// </summary>
	/// <param name="変数名">中身を取得したいグローバル変数名</param>
	/// <returns>変数の中身である文字列</returns>
	string Get_グローバル文字列(string 変数名);


	/// <summary>
	/// LUAのステート(コード)を実行する。
	/// <para>返り値：何もエラーがなければ、０が返ってくる。</para>
	/// <para>エラーがあれば、「LUA::例外型」の例外が投げられる。</para>
	/// </summary>
	/// <param name="コマンド文字列">実行したいLUAのステート(コード、命令文)</param>
	/// <returns>何もエラーがなければ、０が返ってくる。<para>エラーがあれば、「LUA::例外型」の例外が投げられる。</para></returns>
	int Do_コマンド(string コマンド文字列);


	/// <summary>
	/// 指定のLUAのファイルをLUA解釈して実行する。
	/// <para>返り値：何もエラーがなければ、０が返ってくる。</para>
	/// <para>エラーがあれば、「LUA::例外型」の例外が投げられる。</para>
	/// </summary>
	/// <param name="ファイル名">実行したいLUAのファイル名</param>
	/// <returns>何もエラーがなければ、０が返ってくる。<para>エラーがあれば、「LUA::例外型」の例外が投げられる。</para></returns>
	int Do_ファイル(string ファイル名);
}

ostream& operator<<(ostream& os, LUA::例外型& e);

