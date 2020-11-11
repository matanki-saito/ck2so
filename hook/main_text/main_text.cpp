#include "../pch.h"
#include "../plugin_64.h"

extern "C" {
	int add(int a, int b);
    int addA(int a, int b);
}

int add(int a, int b)
{
  return a + b;
}

int addA(int a, int b)
{
  return a + b;
}

namespace MainText {

    extern "C" {
        void mainTextProc1();
        uintptr_t mainTextProc1JmpAddress;
        uintptr_t mainTextProc1ReturnAddress;
    }

	DllError mainTextProc1Injector(RunOptions options) {
		DllError e = {};

		switch (options.version) {
		case v3_3_0:
			// mov rdx, [r12 + rcx*8 + 0D0h]
			BytePattern::temp_instance().find_pattern("49 8B 94 CC D0 00 00 00");
			if (BytePattern::temp_instance().has_size(1, u8"テキスト処理ループ１の文字取得修正")) {
				uintptr_t address = BytePattern::temp_instance().get_first().address();

                // jz      short loc_xxxxx
				mainTextProc1JmpAddress = address + 0x69;

                // mov esi, [rdx+18h]
                mainTextProc1ReturnAddress = address + 0x10;

				Injector::MakeJMP(address, mainTextProc1, true);
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

	DllError Init(RunOptions options) {
		DllError result = {};

		result |= mainTextProc1Injector(options);

		return result;
	}
}