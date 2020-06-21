#include <stdio.h>
#include "byte_search/byte_pattern.h"
#include "injector/injector.hpp"
#include <sys/errno.h>


__attribute__((constructor))
void hooook() {
	BytePattern::StartLog("hook");

	typedef struct _DateFormat {
		char text[11];
	} DateFormat;

	// ex) 1444年11月11日
	DateFormat isoFormat = {
		{'y',' ',0x0F,' ','m','w',' ','d',' ',0x0E,0}
	};

	BytePattern::temp_instance().find_pattern("64 20 77 20 6D");
	if (BytePattern::temp_instance().has_size(1,"Change date format on the toolbar")) {
		printf("match pattern\n");

		uintptr_t address = BytePattern::temp_instance().get_first().address();

		Injector::WriteMemory<DateFormat>(address, isoFormat, true);
	}

	printf("hello world\n");
}