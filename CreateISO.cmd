@rem
@rem
@rem Copyright (c) 2015 - 2021 by blindtiger. All rights reserved.
@rem
@rem The contents of this file are subject to the Mozilla Disk License Version
@rem 2.0 (the "License"); you may not use this file except in compliance with
@rem the License. You may obtain a copy of the License at
@rem http://www.mozilla.org/MPL/
@rem
@rem Software distributed under the License is distributed on an "AS IS" basis,
@rem WITHOUT WARRANTY OF ANY KIND, either express or implied. SEe the License
@rem for the specific language governing rights and limitations under the
@rem License.
@rem
@rem The Initial Developer of the Original Code is blindtiger.
@rem
@rem

@if exist Daat.iso del /F /Q Daat.iso
@oscdimgx64 -lDaat -u1 .\ Daat.iso
