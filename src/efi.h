/*
  void* fields in structs means not implemented
*/

#include <stdint.h>
#include <stddef.h>

#ifndef _UCHAR_H
typedef uint_least16_t char16_t;
#endif

typedef int16_t   INT16;
typedef uint16_t  UINT16;
typedef int32_t   INT32;
typedef uint32_t  UINT32;
typedef int64_t   INT64;
typedef uint64_t  UINT64;
typedef uint64_t  UINTN;
typedef char16_t  CHAR16;	// UTF-16, but should use UCS-2 code points 0x0000-0xFFFF
typedef void      VOID;

typedef UINTN     EFI_STATUS;
typedef VOID*     EFI_HANDLE;

#define IN                             /* Datum is passed to the function */
#define OUT                            /* Datum is returned from the function */
#define OPTIONAL                       /* Passing the datum to the function is
                                          optional, and a NULL may be passed if
                                          the value is not supplied */
#define CONST const                    /* Datum is read-only */
#define EFIAPI __attribute__((ms_abi)) /* x86-64 Microsoft calling convention */

#define EFI_SUCCESS 0

// EFI Simple Text Input Protocol: UEFI Spec 2.10 section 12.3
// Forward declare struct for this to work and compile
typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef struct {
  UINT16 ScanCode;
  CHAR16 UnicodeChar;
} EFI_INPUT_KEY;

typedef EFI_STATUS (EFIAPI *EFI_INPUT_READ_KEY) (
  IN  EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *This,
  OUT EFI_INPUT_KEY                   *Key
);

typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
  void*               Reset;
  EFI_INPUT_READ_KEY  ReadKeyStroke;
  void*               WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

// EFI Simple Text Output Protocol: UEFI Spec 2.10 section 12.4
// Forward declare struct for this to work and compile
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

// Text attributes: UEFI Spec 2.10 section 12.4.7
#define EFI_BLACK  0x00
#define EFI_BLUE   0x01
#define EFI_GREEN  0x02
#define EFI_CYAN   0x03
#define EFI_RED    0x04
#define EFI_YELLOW 0x0E
#define EFI_WHITE  0x0F

// Only use 0x00-0x07 for background with this macro!
#define EFI_TEXT_ATTR(Foreground,Background) \
  ((Foreground) | ((Background) << 4))

typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING) (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN CHAR16                          *String
);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_ATTRIBUTE) (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN UINTN                           Attribute
);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_CLEAR_SCREEN) (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
);

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
  void*                           Reset;
  EFI_TEXT_STRING                 OutputString;
  void*                           TestString;
  void*                           QueryMode;
  void*                           SetMode;
  EFI_TEXT_SET_ATTRIBUTE          SetAttribute;
  EFI_TEXT_CLEAR_SCREEN           ClearScreen;
  void*                           SetCursorPosition;
  void*                           EnableCursor;
  void*                           Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef enum {
  EfiResetCold,
  EfiResetWarm,
  EfiResetShutdown,
  EfiResetPlatformSpecific
} EFI_RESET_TYPE;

typedef VOID (EFIAPI *EFI_RESET_SYSTEM) (
  IN EFI_RESET_TYPE ResetType,
  IN EFI_STATUS     ResetStatus,
  IN UINTN          DataSize,
  IN VOID           *ResetData OPTIONAL
);

// EFI Table Header: UEFI Spec 2.10 section 4.2
typedef struct {
  UINT64  Signature;
  UINT32  Revision;
  UINT32  HeaderSize;
  UINT32  CRC32;
  UINT32  Reserved;
} EFI_TABLE_HEADER;

typedef struct {
  EFI_TABLE_HEADER Hdr;

  // Time Services
  void*                           GetTime;
  void*                           SetTime;
  void*                           GetWakeupTime;
  void*                           SetWakeupTime;

  // Virtual Memory Services
  void*                           SetVirtualAddressMap;
  void*                           ConvertPointer;

  // Variable Services
  void*                           GetVariable;
  void*                           GetNextVariableName;
  void*                           SetVariable;

  // Miscellaneous Services
  void*                           GetNextHighMonotonicCount;
  EFI_RESET_SYSTEM                ResetSystem;

  // UEFI 2.0 Capsule Services
  void*                           UpdateCapsule;
  void*                           QueryCapsuleCapabilities;

  // Miscellaneous UEFI 2.0 Service
  void*                           QueryVariableInfo;
} EFI_RUNTIME_SERVICES;

// EFI System Table. UEFI Spec 2.10 Section 4.3
typedef struct {
  EFI_TABLE_HEADER                 Hdr;
  CHAR16                          *FirmwareVendor;
  UINT32                           FirmwareRevision;
  EFI_HANDLE                       ConsoleInHandle;
  EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *ConIn;
  EFI_HANDLE                       ConsoleOutHandle;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
  EFI_HANDLE                       StandardErrorHandle;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
	EFI_RUNTIME_SERVICES            *RuntimeServices;
	EFI_NOT_IMPLEMENTED              BootServices;
	UINTN                            NumberOfTableEntries;
	EFI_NOT_IMPLEMENTED              ConfigurationTable;
} EFI_SYSTEM_TABLE;

