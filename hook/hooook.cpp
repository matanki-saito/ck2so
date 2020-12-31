#include "pch.h"
#include "plugin_64.h"

__attribute__((constructor))
void hooook() {
	BytePattern::StartLog("ck2_jps_2_linux");

	DllError e = {};

	// 設定
	RunOptions options;
	// Version取得
	Version::GetVersionFromSo(&options);

	// 日付
	e |= Date::Init(options);

	// フォント読み込み
	e |= Font::Init(options);

	// メインテキスト
    e |= MainText::Init(options);
}
