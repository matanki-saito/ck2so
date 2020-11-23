#include "../pch.h"
#include "../plugin_64.h"

namespace MainText {
    extern "C" {
        void mainTextProc1();
    }
    extern "C" uintptr_t mainTextProc1ReturnAddress;
    extern "C" uintptr_t mainTextProc1JmpAddress;

	DllError mainTextProc1Injector(RunOptions options) {
		DllError e = {};

		switch (options.version) {
		case v3_3_0:
            // mov rdx, [r12 + rcx*8 + 0D0h]
            BytePattern::temp_instance().find_pattern("49 8B 94 CC D0 00 00 00");
            if (BytePattern::temp_instance().has_size(1, u8"テキスト処理ループ１のFont取得先")) {
                mainTextProc1JmpAddress = BytePattern::temp_instance().get_first().address();
            }
            else {
                e.unmatch.mainTextProc1Injector = true;
            }

            // movsxd rax, ebp
            BytePattern::temp_instance().find_pattern("48 63 C5 0F B6 88 00 8F 41 02");
            if (BytePattern::temp_instance().has_size(1, u8"テキスト処理ループ１の文字取得先")) {
                uintptr_t address = BytePattern::temp_instance().get_first().address();

                // mov     ds:src[rdx], cl
                mainTextProc1ReturnAddress = address + 0x11;

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

    // CBitmapFont::RenderToScreen
	DllError Init(RunOptions options) {
		DllError result = {};

		result |= mainTextProc1Injector(options);

		return result;
	}
}
