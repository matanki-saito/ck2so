#include "../pch.h"
#include "../plugin_64.h"

namespace MainText {
    extern "C" {
        void mainTextProc1();
        void mainTextProc2();
    }
    extern "C" uintptr_t mainTextProc1ReturnAddress;
    extern "C" uintptr_t mainTextProc2ReturnAddress;

	DllError mainTextProc1Injector(RunOptions options) {
		DllError e = {};

		switch (options.version) {
		case v3_3_0:
            // movsxd rax, ebp
            BytePattern::temp_instance().find_pattern("48 63 C5 0F B6 88 00 8F 41 02");
            if (BytePattern::temp_instance().has_size(1, u8"テキスト処理ループ１の文字取得")) {
                uintptr_t address = BytePattern::temp_instance().get_first().address();

                // jz   short loc_xxxxx
                mainTextProc1ReturnAddress = address + 0x1F;

                Injector::MakeJMP(address,mainTextProc1,true);
            }
            else {
                e.unmatch.mainTextProc1Injector = true;
            }
            break;
		default:
			e.version.mainTextProc1Injector = true;
		}

		return e;
	}

	/*-----------------------------------------------*/

    DllError mainTextProc2Injector(RunOptions options) {
        DllError e = {};

        switch (options.version) {
        case v3_3_0:
            // movsxd  r14, r13d
            BytePattern::temp_instance().find_pattern("4D 63 F5 41 0F B6 86 00 97 41 02");
            if (BytePattern::temp_instance().has_size(1, u8"テキスト処理ループ2の文字取得")) {
                uintptr_t address = BytePattern::temp_instance().get_first().address();

                // jz   short loc_xxxxx
                mainTextProc2ReturnAddress = address + 0x13;

                Injector::MakeJMP(address,mainTextProc2,true);
            }
            else {
                e.unmatch.mainTextProc2Injector = true;
            }
            break;
        default:
            e.version.mainTextProc2Injector = true;
        }

        return e;
    }

    /*-----------------------------------------------*/

    // CBitmapFont::RenderToScreen
	DllError Init(RunOptions options) {
		DllError result = {};

		result |= mainTextProc1Injector(options);
        result |= mainTextProc2Injector(options);

		return result;
	}
}
