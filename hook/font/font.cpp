#include "../pch.h"
#include "../plugin_64.h"

namespace Font {
	DllError fontBufferExpansionInjector(RunOptions options) {
		DllError e = {};

		switch (options.version) {
		case v3_3_0:
			BytePattern::temp_instance().find_pattern("BF 88 33 00 00");
			if (BytePattern::temp_instance().has_size(1, u8"Font buffer expansion")) {
				// mov     edi, 3388h 
				Injector::WriteMemory<uint8_t>(BytePattern::temp_instance().get_first().address(0x3), 0x10, true);
			}
			else {
				e.unmatch.fontBufferExpansionInjector = true;
			}

			BytePattern::temp_instance().find_pattern("BF 70 33 00 00");
			if (BytePattern::temp_instance().has_size(1, u8"Font buffer expansion")) {
				// mov     edi, 3370h
				Injector::WriteMemory<uint8_t>(BytePattern::temp_instance().get_first().address(0x3), 0x10, true);
			}
			else {
				e.unmatch.fontBufferExpansionInjector = true;
			}
			break;
		default:
			e.version.fontBufferExpansionInjector = true;
		}

		return e;
	}

	DllError fontSizeLimitInjector(RunOptions options) {
		DllError e = {};

		switch (options.version) {
		case v3_3_0:
			BytePattern::temp_instance().find_pattern("81 FD FF FF FF 01");
			if (BytePattern::temp_instance().has_size(2, u8"Font size limit")) {
				// cmp     ebp, 1FFFFFFh
				Injector::WriteMemory<uint8_t>(BytePattern::temp_instance().get_first().address(0x5), 0x03, true);

				// 
				Injector::WriteMemory<uint8_t>(BytePattern::temp_instance().get_second().address(0x5), 0x03, true);
			}
			else {
				e.unmatch.fontSizeLimitInjector = true;
			}
		default:
			e.version.fontSizeLimitInjector = true;
		}

		return e;
	}

	/*-----------------------------------------------*/

	DllError Init(RunOptions options) {
		DllError result = {};

		/* フォントバッファ拡張 */
		result |= fontBufferExpansionInjector(options);

		/* フォントサイズの拡張 */
		result |= fontSizeLimitInjector(options);

		return result;
	}
}