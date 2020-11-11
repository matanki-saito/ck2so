#pragma once

#include "pch.h"

enum Ck2Version {
	UNKNOWN = 0,
	v3_3_0 = 330
};

typedef uint64_t DllErrorCode;

struct DllError {
	union {
		DllErrorCode code0;
		struct {
			bool timeout : 1;
			bool waitFailed : 1;
			bool proccessFaild : 1;
		};
	} mod;

	union {
		DllErrorCode code1;
		struct {
			bool fontBufferExpansionInjector : 1;
			bool fontSizeLimitInjector : 1;
			bool fontBufferHeapZeroClearInjector : 1;
			bool debugProc1Injector : 1;
			bool fontBufferClearInjector : 1;
			bool mainTextProc1Injector : 1;
			bool mainTextProc2Injector : 1;
			bool mainTextProc3Injector : 1;
			bool mainTextProc4Injector : 1;
			bool tooltipAndButtonProc1Injector : 1;
			bool tooltipAndButtonProc2Injector : 1;
			bool tooltipAndButtonProc3Injector : 1;
			bool tooltipAndButtonProc4Injector : 1;
			bool tooltipAndButtonProc5Injector : 1;
			bool tooltipAndButtonProc6Injector : 1;
			bool mapViewProc1Injector : 1;
			bool mapViewProc2Injector : 1;
			bool mapViewProc3Injector : 1;
			bool mapViewProc4Injector : 1;
			bool mapAdjustmentProc1Injector : 1;
			bool mapAdjustmentProc2Injector : 1;
			bool mapAdjustmentProc3Injector : 1;
			bool mapAdjustmentProc4Injector : 1;
			bool mapJustifyProc1Injector : 1;
			bool mapJustifyProc2Injector : 1;
			bool mapJustifyProc3Injector : 1;
			bool mapJustifyProc4Injector : 1;
			bool eventDialog1Injector : 1;
			bool eventDialog2Injector : 1;
			bool mapPopupProc1Injector : 1;
			bool mapPopupProc2Injector : 1;
			bool mapPopupProc3Injector : 1;
			bool listFieldAdjustmentProc1Injector : 1;
			bool listFieldAdjustmentProc2Injector : 1;
			bool listFieldAdjustmentProc3Injector : 1;
			bool fileSaveProc1Injector : 1;
			bool fileSaveProc2Injector : 1;
			bool fileSaveProc3Injector : 1;
			bool fileSaveProc4Injector : 1;
			bool fileSaveProc5Injector : 1;
			bool dateProc1Injector : 1;
			bool mapNudgeViewProc1Injector : 1;
			bool mainTextAdjustmentProc1Injector : 1;
			bool mainTextAdjustmentProc2Injector : 1;
			bool mainTextAdjustmentProc3Injector : 1;
			bool mainTextAdjustmentProc4Injector : 1;
			bool mainTextAdjustmentProc5Injector : 1;
			bool mainTextAdjustmentProc6Injector : 1;
			bool decorativeLetterProc1Injector : 1;
			bool decorativeLetterProc2Injector : 1;
			bool decorativeLetterProc3Injector : 1;
			bool decorativeLetterProc4Injector : 1;
			bool decorativeLetterProc5Injector : 1;
			bool decorativeLetterProc6Injector : 1;
			bool decorativeLetterProc7Injector : 1;
			bool decorativeLetterProc8Injector : 1;
			bool decorativeLetterProc9Injector : 1;
			bool decorativeLetterProc10Injector : 1;
			bool decorativeLetterProc11Injector : 1;
			bool mainTextOverflowProc1Injector : 1;
			bool general : 1;
			bool inputProc1Injector : 1;
			bool inputProc2Injector : 1;
		};
	} version;

	union {
		DllErrorCode code2;
		struct {
			bool fontBufferExpansionInjector : 1;
			bool fontSizeLimitInjector : 1;
			bool fontBufferHeapZeroClearInjector : 1;
			bool debugProc1Injector : 1;
			bool fontBufferClearInjector : 1;
			bool mainTextProc1Injector : 1;
			bool mainTextProc2Injector : 1;
			bool mainTextProc3Injector : 1;
			bool mainTextProc3Injector2 : 1;
			bool mainTextProc4Injector : 1;
			bool tooltipAndButtonProc1Injector : 1;
			bool tooltipAndButtonProc2Injector : 1;
			bool tooltipAndButtonProc3Injector : 1;
			bool tooltipAndButtonProc4Injector : 1;
			bool tooltipAndButtonProc5Injector : 1;
			bool tooltipAndButtonProc6Injector : 1;
			bool mapViewProc1Injector : 1;
			bool mapViewProc2Injector : 1;
			bool mapViewProc3Injector : 1;
			bool mapViewProc4Injector : 1;
			bool mapAdjustmentProc1Injector : 1;
			bool mapAdjustmentProc2Injector : 1;
			bool mapAdjustmentProc3Injector : 1;
			bool mapAdjustmentProc4Injector : 1;
			bool mapJustifyProc1Injector : 1;
			bool mapJustifyProc2Injector : 1;
			bool mapJustifyProc3Injector : 1;
			bool mapJustifyProc4Injector : 1;
			bool eventDialog1Injector : 1;
			bool eventDialog2Injector : 1;
			bool mapPopupProc1Injector : 1;
			bool mapPopupProc2Injector : 1;
			bool mapPopupProc3Injector : 1;
			bool listFieldAdjustmentProc1Injector : 1;
			bool listFieldAdjustmentProc2Injector : 1;
			bool listFieldAdjustmentProc3Injector : 1;
			bool fileSaveProc1Injector : 1;
			bool fileSaveProc2Injector : 1;
			bool fileSaveProc3Injector : 1;
			bool fileSaveProc4Injector : 1;
			bool fileSaveProc5Injector : 1;
			bool dateProc1Injector : 1;
			bool mapNudgeViewProc1Injector : 1;
			bool mainTextAdjustmentProc1Injector : 1;
			bool mainTextAdjustmentProc2Injector : 1;
			bool mainTextAdjustmentProc3Injector : 1;
			bool mainTextAdjustmentProc4Injector : 1;
			bool mainTextAdjustmentProc5Injector : 1;
			bool mainTextAdjustmentProc6Injector : 1;
			bool decorativeLetterProc1Injector : 1;
			bool decorativeLetterProc2Injector : 1;
			bool decorativeLetterProc3Injector : 1;
			bool decorativeLetterProc4Injector : 1;
			bool decorativeLetterProc5Injector : 1;
			bool decorativeLetterProc6Injector : 1;
			bool decorativeLetterProc7Injector : 1;
			bool decorativeLetterProc8Injector : 1;
			bool decorativeLetterProc9Injector : 1;
			bool decorativeLetterProc10Injector : 1;
			bool decorativeLetterProc11Injector : 1;
			bool mainTextOverflowProc1Injector : 1;
			bool general : 1;
			bool inputProc1Injector : 1;
			bool inputProc2Injector : 1;
		};
	} unmatch;

	void operator |= (DllError e)
	{
		this->mod.code0 |= e.mod.code0;
		this->version.code1 |= e.version.code1;
		this->unmatch.code2 |= e.unmatch.code2;
	}
};

typedef struct {
	Ck2Version version;
	bool test;
} RunOptions;

namespace Version {
	void GetVersionFromSo(RunOptions* option);
}

namespace Date {
	DllError Init(RunOptions option);
}

namespace Font {
	DllError Init(RunOptions option);
}

namespace MainText {
	DllError Init(RunOptions option);
}
