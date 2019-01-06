/*
cepluginsdk.h
Updated July 4, 2017

v5.0.0
*/

#ifndef _CEPLUGINSDK_H_
#define _CEPLUGINSDK_H_

#include <wtypes.h>

#ifdef __cplusplus
/* Assume byte packing throughout */
extern "C" {
#endif	/* __cplusplus */

#define CESDK_VERSION 6

    typedef enum {
        ptAddressList = 0,
        ptMemoryView = 1,
        ptOnDebugEvent = 2,
        ptProcesswatcherEvent = 3,
        ptFunctionPointerchange = 4,
        ptMainMenu = 5,
        ptDisassemblerContext = 6,
        ptDisassemblerRenderLine = 7,
        ptAutoAssembler = 8
    } PluginType;

    typedef enum {
        aaInitialize = 0,
        aaPhase1 = 1,
        aaPhase2 = 2,
        aaFinalize = 3
    } AutoAssemblerPhase;

    typedef struct _PluginVersion {
        ULONG version; //write here the minimum version this dll is compatible with (Current supported version: 1 and 2: this SDK only describes 2)
        CHAR *pluginname; //make this point to a 0-terminated string (allocated memory or static addressin your dll, not stack)
    } PluginVersion, *PPluginVersion;

    typedef struct _PLUGINTYPE0_RECORD {
        CHAR *interpretedaddress; //pointer to a 255 bytes long string (0 terminated)
        ULONG_PTR address;//this is a read-only representaion of the address. Change interpretedaddress if you want to change this
        BOOLEAN ispointer; //readonly
        LONG countoffsets; //readonly
        ULONG *offsets; //array of dwords ranging from 0 to countoffsets-1 (readonly)
        CHAR *description; //pointer to a 255 bytes long string
        CHAR valuetype; //0=byte, 1=word, 2=dword, 3=float, 4=double, 5=bit, 6=int64, 7=string
        CHAR size; //stringlength or bitlength;
    } PLUGINTYPE0_RECORD, ADDRESSLIST_RECORD, *PPLUGINTYPE0_RECORD, *PADDRESSLIST_RECORD;

    //callback routines efinitions for registered plugin functions:
    typedef BOOLEAN(NTAPI * CEP_PLUGINTYPE0)(PPLUGINTYPE0_RECORD SelectedRecord);
    typedef BOOLEAN(NTAPI * CEP_PLUGINTYPE1)(ULONG *disassembleraddress, ULONG *selected_disassembler_address, ULONG *hexviewaddress);
    typedef LONG(NTAPI * CEP_PLUGINTYPE2)(PVOID DebugEvent);
    typedef VOID(NTAPI * CEP_PLUGINTYPE3)(ULONG processid, ULONG peprocess, BOOLEAN Created);
    typedef VOID(NTAPI * CEP_PLUGINTYPE4)(LONG reserved);
    typedef VOID(NTAPI * CEP_PLUGINTYPE5)(VOID);
    typedef BOOLEAN(NTAPI * CEP_PLUGINTYPE6ONPOPUP)(ULONG_PTR selectedAddress, CHAR **addressofname, BOOLEAN *show);
    typedef BOOLEAN(NTAPI * CEP_PLUGINTYPE6)(ULONG_PTR *selectedAddress);
    typedef VOID(NTAPI * CEP_PLUGINTYPE7)(ULONG_PTR address, CHAR **addressStringPointer, CHAR **bytestringpointer, CHAR **opcodestringpointer, CHAR **specialstringpointer, ULONG *textcolor);
    typedef VOID(NTAPI * CEP_PLUGINTYPE8)(CHAR **line, AutoAssemblerPhase phase, LONG id);

    typedef struct _PLUGINTYPE0_INIT {
        CHAR* name; //0 terminated string describing the name for the user's menu item
        CEP_PLUGINTYPE0 callbackroutine; //pointer to a callback routine of the type 0 plugin 
    } PLUGINTYPE0_INIT, ADDRESSLISTPLUGIN_INIT, *PPLUGINTYPE0_INIT, *PADDRESSLISTPLUGIN_INIT;

    typedef struct _PLUGINTYPE1_INIT {
        CHAR* name; //0 terminated string describing the name for the user's menu item
        CEP_PLUGINTYPE1 callbackroutine; //pointer to a callback routine of the type 1 plugin
        CHAR* shortcut; //0 terminated string containing the shortcut in textform. CE will try it's best to parse it to a valid shortcut
    } PLUGINTYPE1_INIT, MEMORYVIEWPLUGIN_INIT, *PPLUGINTYPE1_INIT, *PMEMORYVIEWPLUGIN_INIT;

    typedef struct _PLUGINTYPE2_INIT {
        CEP_PLUGINTYPE2 callbackroutine; //pointer to a callback routine of the type 2 plugin
    } PLUGINTYPE2_INIT, DEBUGEVENTPLUGIN_INIT, *PPLUGINTYPE2_INIT, *PDEBUGEVENTPLUGIN_INIT;

    typedef struct _PLUGINTYPE3_INIT {
        CEP_PLUGINTYPE3 callbackroutine; //pointer to a callback routine of the type 3 plugin
    } PLUGINTYPE3_INIT, PROCESSWATCHERPLUGIN_INIT, *PPLUGINTYPE3_INIT, *PPROCESSWATCHERPLUGIN_INIT;

    typedef struct _PLUGINTYPE4_INIT {
        CEP_PLUGINTYPE4 callbackroutine; //pointer to a callback routine of the type 4 plugin
    } PLUGINTYPE4_INIT, POINTERREASSIGNMENTPLUGIN_INIT, *PPLUGINTYPE4_INIT, *PPOINTERREASSIGNMENTPLUGIN_INIT;

    typedef struct _PLUGINTYPE5_INIT {
        CHAR* name; //0 terminated string describing the name for the user's menu item
        CEP_PLUGINTYPE5 callbackroutine;
        CHAR* shortcut; //0 terminated string containing the shortcut in textform. CE will try it's best to parse it to a valid shortcut
    } PLUGINTYPE5_INIT, MAINMENUPLUGIN_INIT, *PPLUGINTYPE5_INIT, *PMAINMENUPLUGIN_INIT;

    typedef struct _PLUGINTYPE6_INIT {
        CHAR* name; //0 terminated string describing the name for the user's menu item
        CEP_PLUGINTYPE6 callbackroutine;
        CEP_PLUGINTYPE6ONPOPUP callbackroutineOnPopup;
        CHAR* shortcut; //0 terminated string containing the shortcut in textform. CE will try it's best to parse it to a valid shortcut
    } PLUGINTYPE6_INIT, DISASSEMBLERCONTEXT_INIT, *PPLUGINTYPE6_INIT, *PDISASSEMBLERCONTEXT_INIT;

    typedef struct _PLUGINTYPE7_INIT {
        CEP_PLUGINTYPE7 callbackroutine; //pointer to a callback routine of the type 7 plugin
    } PLUGINTYPE7_INIT, DISASSEMBLERLINEPLUGIN_INIT, *PPLUGINTYPE7_INIT, *PDISASSEMBLERLINEPLUGIN_INIT;

    typedef struct _PLUGINTYPE8_INIT {
        CEP_PLUGINTYPE8 callbackroutine; //pointer to a callback routine of the type 8 plugin
    } PLUGINTYPE8_INIT, AUTOASSEMBLERPLUGIN_INIT, *PPLUGINTYPE8_INIT, *PAUTOASSEMBLERPLUGIN_INIT;

    typedef struct _REGISTERMODIFICATIONINFO {
        ULONG_PTR address; //addres to break on
        BOOLEAN change_eax;
        BOOLEAN change_ebx;
        BOOLEAN change_ecx;
        BOOLEAN change_edx;
        BOOLEAN change_esi;
        BOOLEAN change_edi;
        BOOLEAN change_ebp;
        BOOLEAN change_esp;
        BOOLEAN change_eip;

#ifdef _AMD64_
        BOOLEAN change_r8;
        BOOLEAN change_r9;
        BOOLEAN change_r10;
        BOOLEAN change_r11;
        BOOLEAN change_r12;
        BOOLEAN change_r13;
        BOOLEAN change_r14;
        BOOLEAN change_r15;
#endif

        BOOLEAN change_cf;
        BOOLEAN change_pf;
        BOOLEAN change_af;
        BOOLEAN change_zf;
        BOOLEAN change_sf;
        BOOLEAN change_of;
        ULONG_PTR new_eax;
        ULONG_PTR new_ebx;
        ULONG_PTR new_ecx;
        ULONG_PTR new_edx;
        ULONG_PTR new_esi;
        ULONG_PTR new_edi;
        ULONG_PTR new_ebp;
        ULONG_PTR new_esp;
        ULONG_PTR new_eip;

#ifdef _AMD64_
        ULONG_PTR new_r8;
        ULONG_PTR new_r9;
        ULONG_PTR new_r10;
        ULONG_PTR new_r11;
        ULONG_PTR new_r12;
        ULONG_PTR new_r13;
        ULONG_PTR new_r14;
        ULONG_PTR new_r15;
#endif

        BOOLEAN new_cf;
        BOOLEAN new_pf;
        BOOLEAN new_af;
        BOOLEAN new_zf;
        BOOLEAN new_sf;
        BOOLEAN new_of;
    } REGISTERMODIFICATIONINFO, *PREGISTERMODIFICATIONINFO;

    //the NTAPI stuff isn't really needed since I've set compiler options to force stdcall, but this makes it clear that stdcall is used to the reader
    typedef VOID(NTAPI * CEP_SHOWMESSAGE)(CHAR* message);
    typedef LONG(NTAPI * CEP_REGISTERFUNCTION) (LONG pluginid, PluginType functiontype, PVOID init);
    typedef BOOLEAN(NTAPI * CEP_UNREGISTERFUNCTION) (LONG pluginid, LONG functionid);
    typedef HANDLE(NTAPI * CEP_GETMAINWINDOWHANDLE) (VOID);
    typedef BOOLEAN(NTAPI * CEP_AUTOASSEMBLE) (CHAR *script);
    typedef BOOLEAN(NTAPI * CEP_ASSEMBLER) (ULONG_PTR address, CHAR* instruction, CHAR * output, LONG maxlength, LONG *returnedsize);
    typedef BOOLEAN(NTAPI * CEP_DISASSEMBLER) (ULONG_PTR address, CHAR* output, LONG maxsize);
    typedef BOOLEAN(NTAPI * CEP_CHANGEREGATADDRESS) (ULONG_PTR address, PREGISTERMODIFICATIONINFO changereg);
    typedef BOOLEAN(NTAPI * CEP_INJECTDLL) (CHAR *dllname, CHAR *functiontocall);
    typedef LONG(NTAPI * CEP_FREEZEMEM) (ULONG_PTR address, LONG size);
    typedef BOOLEAN(NTAPI * CEP_UNFREEZEMEM) (LONG freezeID);
    typedef BOOLEAN(NTAPI * CEP_FIXMEM) (VOID);
    typedef BOOLEAN(NTAPI * CEP_PROCESSLIST) (CHAR *listbuffer, LONG listsize);
    typedef BOOLEAN(NTAPI * CEP_RELOADSETTINGS) (VOID);
    typedef ULONG(NTAPI * CEP_GETADDRESSFROMPOINTER) (ULONG_PTR baseaddress, LONG offsetcount, LONG* offsets);
    typedef BOOLEAN(NTAPI * CEP_GENERATEAPIHOOKSCRIPT) (CHAR *address, CHAR *addresstojumpto, CHAR *addresstogetnewcalladdress, CHAR *script, LONG maxscriptsize);
    typedef BOOLEAN(NTAPI * CEP_ADDRESSTONAME) (ULONG_PTR address, CHAR *name, LONG maxnamesize);
    typedef BOOLEAN(NTAPI * CEP_NAMETOADDRESS) (CHAR *name, ULONG_PTR *address);

    typedef VOID(NTAPI * CEP_LOADDBK32)(VOID);
    typedef BOOLEAN(NTAPI * CEP_LOADDBVMIFNEEDED)(VOID);
    typedef ULONG(NTAPI * CEP_PREVIOUSOPCODE)(ULONG address);
    typedef ULONG(NTAPI * CEP_NEXTOPCODE)(ULONG address);
    typedef BOOLEAN(NTAPI * CEP_LOADMODULE)(CHAR *modulepath, CHAR *exportlist, LONG *maxsize);
    typedef BOOLEAN(NTAPI * CEP_DISASSEMBLEEX)(ULONG address, CHAR *output, LONG maxsize);
    typedef VOID(NTAPI * CEP_AA_ADDCOMMAND)(CHAR *command);
    typedef VOID(NTAPI * CEP_AA_DELCOMMAND)(CHAR *command);

    typedef PVOID(NTAPI * CEP_CREATETABLEENTRY)(VOID);
    typedef PVOID(NTAPI * CEP_GETTABLEENTRY)(CHAR *description);
    typedef BOOLEAN(NTAPI * CEP_MEMREC_SETDESCRIPTION)(PVOID memrec, CHAR *description);
    typedef PCHAR(NTAPI * CEP_MEMREC_GETDESCRIPTION)(PVOID memrec);
    typedef BOOLEAN(NTAPI * CEP_MEMREC_GETADDRESS)(PVOID memrec, ULONG_PTR *address, ULONG *offsets, LONG maxoffsets, LONG *neededOffsets);
    typedef BOOLEAN(NTAPI * CEP_MEMREC_SETADDRESS)(PVOID memrec, CHAR *address, ULONG *offsets, LONG offsetcount);
    typedef LONG(NTAPI * CEP_MEMREC_GETTYPE)(PVOID memrec);
    typedef BOOLEAN(NTAPI * CEP_MEMREC_SETTYPE)(PVOID memrec, LONG vtype);
    typedef BOOLEAN(NTAPI * CEP_MEMREC_GETVALUETYPE)(PVOID memrec, CHAR *value, LONG maxsize);
    typedef BOOLEAN(NTAPI * CEP_MEMREC_SETVALUETYPE)(PVOID memrec, CHAR *value);
    typedef CHAR* (NTAPI * CEP_MEMREC_GETSCRIPT)(PVOID memrec);
    typedef BOOLEAN(NTAPI * CEP_MEMREC_SETSCRIPT)(PVOID memrec, CHAR *script);
    typedef BOOLEAN(NTAPI * CEP_MEMREC_ISFROZEN)(PVOID memrec);
    typedef BOOLEAN(NTAPI * CEP_MEMREC_FREEZE)(PVOID memrec, LONG direction);
    typedef BOOLEAN(NTAPI * CEP_MEMREC_UNFREEZE)(PVOID memrec);
    typedef BOOLEAN(NTAPI * CEP_MEMREC_SETCOLOR)(PVOID memrec, ULONG color);
    typedef BOOLEAN(NTAPI * CEP_MEMREC_APPENDTOENTRY)(PVOID memrec1, PVOID memrec2);
    typedef BOOLEAN(NTAPI * CEP_MEMREC_DELETE)(PVOID memrec);

    typedef ULONG(NTAPI * CEP_GETPROCESSIDFROMPROCESSNAME)(CHAR *name);
    typedef ULONG(NTAPI * CEP_OPENPROCESS)(ULONG pid);
    typedef ULONG(NTAPI * CEP_DEBUGPROCESS)(LONG debuggerinterface);
    typedef VOID(NTAPI * CEP_PAUSE)(VOID);
    typedef VOID(NTAPI * CEP_UNPAUSE)(VOID);
    typedef BOOLEAN(NTAPI * CEP_DEBUG_SETBREAKPOINT)(ULONG_PTR address, LONG size, LONG trigger);
    typedef BOOLEAN(NTAPI * CEP_DEBUG_REMOVEBREAKPOINT)(ULONG_PTR address);
    typedef BOOLEAN(NTAPI * CEP_DEBUG_CONTINUEFROMBREAKPOINT)(LONG continueoption);

    typedef VOID(NTAPI * CEP_CLOSECE)(VOID);
    typedef VOID(NTAPI * CEP_HIDEALLCEWINDOWS)(VOID);
    typedef VOID(NTAPI * CEP_UNHIDEMAINCEWINDOW)(VOID);

    typedef PVOID(NTAPI * CEP_CREATEFORM)(VOID);
    typedef VOID(NTAPI * CEP_FORM_CENTERSCREEN)(PVOID form);
    typedef VOID(NTAPI * CEP_FORM_HIDE)(PVOID form);
    typedef VOID(NTAPI * CEP_FORM_SHOW)(PVOID form);
    typedef VOID(NTAPI * CEP_FORM_ONCLOSE)(PVOID form, PVOID function);

    typedef PVOID(NTAPI * CEP_CREATEPANEL)(PVOID owner);
    typedef PVOID(NTAPI * CEP_CREATEGROUPBOX)(PVOID owner);
    typedef PVOID(NTAPI * CEP_CREATEBUTTON)(PVOID owner);
    typedef PVOID(NTAPI * CEP_CREATEIMAGE)(PVOID owner);

    typedef BOOLEAN(NTAPI * CEP_IMAGE_LOADIMAGEFROMFILE)(PVOID image, CHAR *filename);
    typedef VOID(NTAPI * CEP_IMAGE_TRANSPARENT)(PVOID image, BOOLEAN transparent);
    typedef VOID(NTAPI * CEP_IMAGE_STRETCH)(PVOID image, BOOLEAN stretch);

    typedef PVOID(NTAPI * CEP_CREATELABEL)(PVOID owner);
    typedef PVOID(NTAPI * CEP_CREATEEDIT)(PVOID owner);
    typedef PVOID(NTAPI * CEP_CREATEMEMO)(PVOID owner);
    typedef PVOID(NTAPI * CEP_CREATETIMER)(PVOID owner);

    typedef VOID(NTAPI * CEP_TIMER_SETINTERVAL)(PVOID timer, LONG interval);
    typedef VOID(NTAPI * CEP_TIMER_ONTIMER)(PVOID timer, PVOID function);

    typedef VOID(NTAPI * CEP_CONTROL_SETCAPTION)(PVOID control, CHAR *caption);
    typedef BOOLEAN(NTAPI * CEP_CONTROL_GETCAPTION)(PVOID control, CHAR *caption, LONG maxsize);

    typedef VOID(NTAPI * CEP_CONTROL_SETPOSITION)(PVOID control, LONG x, LONG y);
    typedef LONG(NTAPI * CEP_CONTROL_GETX)(PVOID control);
    typedef LONG(NTAPI * CEP_CONTROL_GETY)(PVOID control);

    typedef VOID(NTAPI * CEP_CONTROL_SETSIZE)(PVOID control, LONG width, LONG height);
    typedef LONG(NTAPI * CEP_CONTROL_GETWIDTH)(PVOID control);
    typedef LONG(NTAPI * CEP_CONTROL_GETHEIGHT)(PVOID control);

    typedef VOID(NTAPI * CEP_CONTROL_SETALIGN)(PVOID control, LONG align);
    typedef VOID(NTAPI * CEP_CONTROL_ONCLICK)(PVOID control, PVOID function);

    typedef VOID(NTAPI * CEP_OBJECT_DESTROY)(PVOID object);

    typedef LONG(NTAPI * CEP_MESSAGEDIALOG)(CHAR *massage, LONG messagetype, LONG buttoncombination);
    typedef BOOLEAN(NTAPI * CEP_SPEEDHACK_SETSPEED)(float speed);
    // typedef lua_State *(__fastcall *CEP_GETLUASTATE)();

    /*
    function ce_messageDialog(message: pchar; messagetype: integer; buttoncombination: integer): integer; stdcall;
    function ce_speedhack_setSpeed(speed: single): BOOLEAN; stdcall;
    */

    typedef struct _ExportedFunctions {
        LONG sizeofExportedFunctions;
        CEP_SHOWMESSAGE ShowMessage; //Pointer to the ce showmessage function
        CEP_REGISTERFUNCTION RegisterFunction; //Use this to register a specific type of plugin
        CEP_UNREGISTERFUNCTION UnregisterFunction; //unregisters a function registered with registerfunction
        PULONG OpenedProcessID; //pointer to the currently selected processid
        PHANDLE OpenedProcessHandle; //pointer to the currently selected processhandle

        CEP_GETMAINWINDOWHANDLE GetMainWindowHandle; //returns the handle of the main window (for whatever reason, it is recommended to use delphi to make a real userinterface upgrade)
        CEP_AUTOASSEMBLE AutoAssemble; //Pointer to the AutoAssemble function
        CEP_ASSEMBLER Assembler; //pointer to the assembler function
        CEP_DISASSEMBLER Disassembler; //pointer to the disassembler function
        CEP_CHANGEREGATADDRESS ChangeRegistersAtAddress; //pointer to the ChangeRegAtBP function
        CEP_INJECTDLL InjectDLL; //pointer to ce's Inject DLL function
        CEP_FREEZEMEM FreezeMem; //pointer to the FreezeMem routine
        CEP_UNFREEZEMEM UnfreezeMem; //pointer to the UnfreezeMem routine (use this to undo freezes with FreezeMem)
        CEP_FIXMEM FixMem; //pointer to the fixmem routine
        CEP_PROCESSLIST ProcessList; //pointer to the processlist routine
        CEP_RELOADSETTINGS ReloadSettings; //pointer to the ReloadSettings routine
        CEP_GETADDRESSFROMPOINTER GetAddressFromPointer; //pointer to the GetAddressFromPointer routine

        //pointers to the address that contains the pointers to the functions
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        PVOID ReadProcessMemory;			//pointer to the pointer of ReadProcessMemory (Change it to hook that api, or use it yourself)
        PVOID WriteProcessMemory;			//pointer to the pointer of WriteProcessMemory (Change it to hook that api, or use it yourself)
        PVOID GetThreadContext;			    // ...
        PVOID SetThreadContext;			    // ...
        PVOID SuspendThread;				// ...
        PVOID ResumeThread;				    // ...
        PVOID OpenProcess;				    // ...
        PVOID WaitForDebugEvent;			// ...
        PVOID ContinueDebugEvent;			// ...
        PVOID DebugActiveProcess;			// ...
        PVOID StopDebugging;				// ...
        PVOID StopRegisterChange;			// ...
        PVOID VirtualProtect;				// ...
        PVOID VirtualProtectEx;			    // ...
        PVOID VirtualQueryEx;				// ...
        PVOID VirtualAllocEx;				// ...
        PVOID CreateRemoteThread;			// ...
        PVOID OpenThread;					// ...
        PVOID GetPEProcess;				    // ...
        PVOID GetPEThread;				    // ...
        PVOID GetThreadsProcessOffset;	    // ...
        PVOID GetThreadListEntryOffset;	    // ...
        PVOID GetProcessnameOffset;		    // ...
        PVOID GetDebugportOffset;			// ...
        PVOID GetPhysicalAddress;			// ...
        PVOID ProtectMe;					// ...
        PVOID GetCR4;						// ...
        PVOID GetCR3;						// ...
        PVOID SetCR3;						// ...
        PVOID GetSDT;						// ...
        PVOID GetSDTShadow;				    // ...
        PVOID setAlternateDebugMethod;	    // ...
        PVOID getAlternateDebugMethod;	    // ...
        PVOID DebugProcess;				    // ...
        PVOID ChangeRegOnBP;				// ...
        PVOID RetrieveDebugData;			// ...
        PVOID StartProcessWatch;			// ...
        PVOID WaitForProcessListData;		// ...
        PVOID GetProcessNameFromID;		    // ...
        PVOID GetProcessNameFromPEProcess;  // ...
        PVOID KernelOpenProcess;			// ...
        PVOID KernelReadProcessMemory;	    // ...
        PVOID KernelWriteProcessMemory;	    // ...
        PVOID KernelVirtualAllocEx;		    // ...
        PVOID IsValidHandle;				// ...
        PVOID GetIDTCurrentThread;		    // ...
        PVOID GetIDTs;					    // ...
        PVOID MakeWritable;				    // ...
        PVOID GetLoadedState;				// ...
        PVOID DBKSuspendThread;			    // ...
        PVOID DBKResumeThread;			    // ...
        PVOID DBKSuspendProcess;			// ...
        PVOID DBKResumeProcess;			    // ...
        PVOID KernelAlloc;				    // ...
        PVOID GetKProcAddress;			    // ...
        PVOID CreateToolhelp32Snapshot;	    // ...
        PVOID Process32First; 	            // ...
        PVOID Process32Next; 	            // ...
        PVOID Thread32First; 	            // ...
        PVOID Thread32Next; 	            // ...
        PVOID Module32First; 	            // ...
        PVOID Module32Next; 	            // ...
        PVOID Heap32ListFirst; 	            // ...
        PVOID Heap32ListNext; 	            // ...
        //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

        //advanced for delphi 7 enterprise dll programmers only
        PVOID mainform; //pointer to the Tmainform object.
        PVOID memorybrowser; //pointer to the TMemoryBrowser object (memory view windows), same as mainform

        //Plugin Version 2+
        CEP_NAMETOADDRESS sym_nameToAddress;
        CEP_ADDRESSTONAME sym_addressToName;
        CEP_GENERATEAPIHOOKSCRIPT sym_generateAPIHookScript;

        //Plugin version 3+
        CEP_LOADDBK32 loadDBK32;
        CEP_LOADDBVMIFNEEDED loaddbvmifneeded;
        CEP_PREVIOUSOPCODE previousOpcode;
        CEP_NEXTOPCODE nextOpcode;
        CEP_DISASSEMBLEEX disassembleEx;
        CEP_LOADMODULE loadModule;
        CEP_AA_ADDCOMMAND aa_AddExtraCommand;
        CEP_AA_DELCOMMAND aa_RemoveExtraCommand;

        //version 4 extension
        CEP_CREATETABLEENTRY createTableEntry;
        CEP_GETTABLEENTRY getTableEntry;
        CEP_MEMREC_SETDESCRIPTION memrec_setDescription;
        CEP_MEMREC_GETDESCRIPTION memrec_getDescription;
        CEP_MEMREC_GETADDRESS memrec_getAddress;
        CEP_MEMREC_SETADDRESS memrec_setAddress;
        CEP_MEMREC_GETTYPE memrec_getType;
        CEP_MEMREC_SETTYPE memrec_setType;
        CEP_MEMREC_GETVALUETYPE memrec_getValue;
        CEP_MEMREC_SETVALUETYPE memrec_setValue;
        CEP_MEMREC_GETSCRIPT memrec_getScript;
        CEP_MEMREC_SETSCRIPT memrec_setScript;
        CEP_MEMREC_ISFROZEN memrec_isfrozen;
        CEP_MEMREC_FREEZE memrec_freeze;
        CEP_MEMREC_UNFREEZE memrec_unfreeze;
        CEP_MEMREC_SETCOLOR memrec_setColor;
        CEP_MEMREC_APPENDTOENTRY memrec_appendtoentry;
        CEP_MEMREC_DELETE memrec_delete;

        CEP_GETPROCESSIDFROMPROCESSNAME getProcessIDFromProcessName;
        CEP_OPENPROCESS openProcessEx;
        CEP_DEBUGPROCESS debugProcessEx;
        CEP_PAUSE pause;
        CEP_UNPAUSE unpause;

        CEP_DEBUG_SETBREAKPOINT debug_setBreakpoint;
        CEP_DEBUG_REMOVEBREAKPOINT debug_removeBreakpoint;
        CEP_DEBUG_CONTINUEFROMBREAKPOINT debug_continueFromBreakpoint;

        CEP_CLOSECE closeCE;
        CEP_HIDEALLCEWINDOWS hideAllCEWindows;
        CEP_UNHIDEMAINCEWINDOW unhideMainCEwindow;
        CEP_CREATEFORM createForm;
        CEP_FORM_CENTERSCREEN form_centerScreen;
        CEP_FORM_HIDE form_hide;
        CEP_FORM_SHOW form_show;
        CEP_FORM_ONCLOSE form_onClose;

        CEP_CREATEPANEL createPanel;
        CEP_CREATEGROUPBOX createGroupBox;
        CEP_CREATEBUTTON createButton;
        CEP_CREATEIMAGE createImage;
        CEP_IMAGE_LOADIMAGEFROMFILE image_loadImageFromFile;
        CEP_IMAGE_TRANSPARENT image_transparent;
        CEP_IMAGE_STRETCH image_stretch;

        CEP_CREATELABEL createLabel;
        CEP_CREATEEDIT createEdit;
        CEP_CREATEMEMO createMemo;
        CEP_CREATETIMER createTimer;
        CEP_TIMER_SETINTERVAL timer_setInterval;
        CEP_TIMER_ONTIMER timer_onTimer;
        CEP_CONTROL_SETCAPTION control_setCaption;
        CEP_CONTROL_GETCAPTION control_getCaption;
        CEP_CONTROL_SETPOSITION control_setPosition;
        CEP_CONTROL_GETX control_getX;
        CEP_CONTROL_GETY control_getY;
        CEP_CONTROL_SETSIZE control_setSize;
        CEP_CONTROL_GETWIDTH control_getWidth;
        CEP_CONTROL_GETHEIGHT control_getHeight;
        CEP_CONTROL_SETALIGN control_setAlign;
        CEP_CONTROL_ONCLICK control_onClick;

        CEP_OBJECT_DESTROY object_destroy;
        CEP_MESSAGEDIALOG messageDialog;
        CEP_SPEEDHACK_SETSPEED speedhack_setSpeed;

        //V5: Todo, implement function declarations
        VOID * ExecuteKernelCode;
        VOID * UserdefinedInterruptHook;
        VOID * GetLuaState; /* CEP_GETLUASTATE */
        VOID * MainThreadCall;
    } ExportedFunctions, *PExportedFunctions;

    BOOLEAN
        NTAPI
        CEPlugin_GetVersion(
            __inout PPluginVersion pv,
            __inout LONG sizeofpluginversion
        );

    BOOLEAN
        NTAPI
        CEPlugin_InitializePlugin(
            __inout PExportedFunctions ef,
            __inout LONG pluginid
        );

    BOOLEAN
        NTAPI
        CEPlugin_DisablePlugin(
            VOID
        );

    //old versions without CEPlugin_ in front also work but are not recommended due to bugbrained compilers...

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif // !_CEPLUGINSDK_H_
