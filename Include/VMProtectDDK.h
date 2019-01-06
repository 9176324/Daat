#ifndef _VMPROTECTDDK_H_
#define _VMPROTECTDDK_H_

#include <wtypes.h>

#ifdef __cplusplus
/* Assume byte packing throughout */
extern "C" {
#endif	/* __cplusplus */

    // protection
    DECLSPEC_IMPORT
        VOID
        NTAPI
        VMProtectBegin(
            __in PCSTR MarkerName
        );

    DECLSPEC_IMPORT
        VOID
        NTAPI
        VMProtectBeginVirtualization(
            __in PCSTR MarkerName
        );

    DECLSPEC_IMPORT
        VOID NTAPI
        VMProtectBeginMutation(
            __in PCSTR MarkerName
        );

    DECLSPEC_IMPORT VOID
        NTAPI
        VMProtectBeginUltra(
            __in PCSTR MarkerName
        );

    DECLSPEC_IMPORT
        VOID
        NTAPI
        VMProtectBeginVirtualizationLockByKey(
            __in PCSTR MarkerName
        );

    DECLSPEC_IMPORT
        VOID
        NTAPI
        VMProtectBeginUltraLockByKey(
            __in PCSTR MarkerName
        );

    DECLSPEC_IMPORT
        VOID
        NTAPI
        VMProtectEnd(
            VOID
        );

    // utils
    DECLSPEC_IMPORT
        BOOLEAN
        NTAPI
        VMProtectIsProtected(
            VOID
        );

    DECLSPEC_IMPORT
        BOOLEAN
        NTAPI
        VMProtectIsDebuggerPresent(
            __in BOOLEAN CheckKernelMode
        );

    DECLSPEC_IMPORT
        BOOLEAN
        NTAPI
        VMProtectIsVirtualMachinePresent(
            VOID
        );

    DECLSPEC_IMPORT
        BOOLEAN
        NTAPI
        VMProtectIsValidImageCRC(
            VOID
        );

    DECLSPEC_IMPORT
        PCSTR
        NTAPI
        VMProtectDecryptStringA(
            __in PCSTR Value
        );

    DECLSPEC_IMPORT
        PCWSTR
        NTAPI
        VMProtectDecryptStringW(
            __in PCWSTR Value
        );

    DECLSPEC_IMPORT
        BOOLEAN
        NTAPI
        VMProtectFreeString(
            __in PVOID Value
        );

    // licensing
    typedef enum _VMPROTECT_SERIAL_STATE_FLAGS {
        SERIAL_STATE_SUCCESS = 0,
        SERIAL_STATE_FLAG_CORRUPTED = 0x00000001,
        SERIAL_STATE_FLAG_INVALID = 0x00000002,
        SERIAL_STATE_FLAG_BLACKLISTED = 0x00000004,
        SERIAL_STATE_FLAG_DATE_EXPIRED = 0x00000008,
        SERIAL_STATE_FLAG_RUNNING_TIME_OVER = 0x00000010,
        SERIAL_STATE_FLAG_BAD_HWID = 0x00000020,
        SERIAL_STATE_FLAG_MAX_BUILD_EXPIRED = 0x00000040,
    }VMPROTECT_SERIAL_STATE_FLAGS;

#pragma pack(push, 1)
    typedef struct _VMPROTECT_DATE {
        USHORT Year;
        UCHAR Month;
        UCHAR Day;
    } VMPROTECT_DATE;

    typedef struct _VMPROTECT_SERIAL_NUMBER_DATA {
        LONG State; // VMProtectSerialStateFlags
        WCHAR UserName[256]; // user name
        WCHAR EMail[256]; // email
        VMPROTECT_DATE Expire; // date of serial number expiration
        VMPROTECT_DATE MaxBuild; // max date of build, that will accept this key
        LONG RunningTime; // running time in minutes
        UCHAR UserDataLength; // length of user data in bUserData
        UCHAR UserData[255]; // up to 255 bytes of user data
    } VMPROTECT_SERIAL_NUMBER_DATA, *PVMPROTECT_SERIAL_NUMBER_DATA;
#pragma pack(pop)

    DECLSPEC_IMPORT
        LONG
        NTAPI
        VMProtectSetSerialNumber(
            __in PCSTR Serial
        );

    DECLSPEC_IMPORT
        LONG
        NTAPI
        VMProtectGetSerialNumberState(
            VOID
        );

    DECLSPEC_IMPORT
        BOOLEAN
        NTAPI
        VMProtectGetSerialNumberData(
            PVMPROTECT_SERIAL_NUMBER_DATA Data,
            LONG Size
        );

    DECLSPEC_IMPORT
        LONG
        NTAPI
        VMProtectGetCurrentHWID(
            PSTR Hwid,
            LONG Size
        );

    // activation
    typedef enum _VMPROTECT_ACTIVATION_FLAGS {
        ACTIVATION_OK = 0,
        ACTIVATION_SMALL_BUFFER,
        ACTIVATION_NO_CONNECTION,
        ACTIVATION_BAD_REPLY,
        ACTIVATION_BANNED,
        ACTIVATION_CORRUPTED,
        ACTIVATION_BAD_CODE,
        ACTIVATION_ALREADY_USED,
        ACTIVATION_SERIAL_UNKNOWN,
        ACTIVATION_EXPIRED,
        ACTIVATION_NOT_AVAILABLE
    }VMPROTECT_ACTIVATION_FLAGS;

    DECLSPEC_IMPORT
        LONG
        NTAPI
        VMProtectActivateLicense(
            PCSTR Code,
            PSTR Serial,
            LONG Size
        );

    DECLSPEC_IMPORT
        LONG
        NTAPI
        VMProtectDeactivateLicense(
            PCSTR Serial
        );

    DECLSPEC_IMPORT
        LONG
        NTAPI
        VMProtectGetOfflineActivationString(
            PCSTR Code,
            PSTR Buf,
            LONG size
        );

    DECLSPEC_IMPORT
        LONG
        NTAPI
        VMProtectGetOfflineDeactivationString(
            PCSTR Serial,
            PSTR Buf,
            LONG Size
        );

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif // !_VMPROTECTDDK_H_
