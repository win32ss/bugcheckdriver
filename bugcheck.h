
#ifdef _M_X64          // This list of defines exists because my unusual build environment (VS2008 IDE + VS2022 compiler/linkers)               
  #define _AMD64_      // would not change archs easily.
#elif _M_IX86
  #define _X86_
#elif _M_ARM
  #define _ARM_
#elif _M_ARM64
  #define _ARM64_
#endif             

#include <ntddk.h>


typedef struct BugCheckData {
	ULONG BugCheckCode;
	ULONG_PTR BugCheckParameter1;
	ULONG_PTR BugCheckParameter2;
	ULONG_PTR BugCheckParameter3;
	ULONG_PTR BugCheckParameter4;
//	LONG64 TextColor;
//	LONG64 SolidColorUpperLeftX;
//	LONG64 SolidColorUpperLeftY;
//	LONG64 SolidColorBottomRightX;
//	LONG64 SolidColorBottomRightY;
//	LONG64 BackgroundColor;
} BugCheckData, *PBugCheckData;

 //__int64 InbvSetTextColor(LONG64 Color);

// VOID InbvSolidColorFill(LONG64 x1, LONG64 y1, LONG64 x2, LONG64 y2, LONG64 Color);
