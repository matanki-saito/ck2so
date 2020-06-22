#include "../pch.h"
#include "../plugin_64.h"

using namespace std;

namespace Version {
	typedef struct _Pattern {
		char ascii1;
		char dot;
		char ascii2;

		int calVer() {
			int ver = (ascii1 - 0x30) * 100 + (ascii2 - 0x30)*10;
			return ver;
		}
	} Pattern;

	string versionString(Ck2Version version) {
		switch (version) {
		case v3_3_0:
			return u8"v3_3_0";
		default:
			return u8"UNKNOWN";
		}
	}

	void GetVersionFromSo(RunOptions* options) {
		Ck2Version version;

		// CK2 v3.x.
		BytePattern::temp_instance().find_pattern("00 33 2E 33 2E 33");
		if (BytePattern::temp_instance().count() > 0) {
			// ??を取得する
			Pattern ptn = Injector::ReadMemory<Pattern>(BytePattern::temp_instance().get_first().address(0x1), true);

			switch (ptn.calVer()) {
			case 330:
			case 331:
				version = v3_3_0;
				break;
			default:
				version = UNKNOWN;
				break;
				;;
			}

			BytePattern::LoggingInfo(versionString(version));
		}
		else {
			version = UNKNOWN;
		}
		
		options->version = version;
	}
}