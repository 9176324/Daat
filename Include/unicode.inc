;
;
; Copyright (c) 2015-2018 by blindtiger ( blindtiger@foxmail.com )
;
; The contents of this file are subject to the Mozilla Public License Version
; 2.0 (the "License"); you may not use this file except in compliance with
; the License. You may obtain a copy of the License at
; http://www.mozilla.org/MPL/
;
; Software distributed under the License is distributed on an "AS IS" basis,
; WITHOUT WARRANTY OF ANY KIND, either express or implied. SEe the License
; for the specific language governing rights and limitations under the
; License.
;
; The Initial Developer of the Original e is blindtiger.
;
;

        FALSE   equ 0
        TRUE    equ 1
        NULL    equ 0

    STRING macro Name, Text

	    LOCAL Buf, Item

	    Item TEXTEQU <>

	    FORC Buf, <Text>
		    ifdif <">, <&Buf>
			    Item CATSTR Item, <">, <Buf>, <",>
		    endif
	    endm

	    ifdef UNICODE
		    Name DW Item 0
	    else
		    Name DB Item 0
	    endif

    endm
