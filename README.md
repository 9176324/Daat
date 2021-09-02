# Daat
    a simple intel vt code both support x86 & x64. PatchGuard monitor.

# Create labs
    md X:\Labs
    cd /d X:\Labs
    git clone https://github.com/9176324/Daat
    git clone https://github.com/9176324/WinDDK
    git clone https://github.com/9176324/WRK
    git clone https://github.com/9176324/MSVC

# Build
    Method 1: run Rebuild.cmd
    Method 2: MSBuild "X:\Labs\Daat\Daat.sln" -t:Rebuild /p:Platform="x86|x64"
    Method 3: open "X:\Labs\Shark\Daat.sln" with VisualStudio
    