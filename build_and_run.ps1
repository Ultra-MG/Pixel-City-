$MSYS2_BASH = "C:\msys64\usr\bin\bash.exe"
$PROJECT_DIR = $PSScriptRoot

function To-MsysPath($path) {
    $path = $path -replace '\\','/'
    return "/c/" + $path.Substring(3)
}

$MSYS_PROJECT_DIR = To-MsysPath $PROJECT_DIR

# Add UCRT64 toolchain to PATH for this bash session
$UCRT64_PREFIX = "/ucrt64/bin:/usr/bin"

& $MSYS2_BASH -lc "
export PATH='$UCRT64_PREFIX':`$PATH
cd '$MSYS_PROJECT_DIR' &&
cmake -S . -B build -G Ninja &&
cmake --build build
"

if ($LASTEXITCODE -ne 0) {
    Write-Host '❌ Build failed' -ForegroundColor Red
    exit 1
}

& $MSYS2_BASH -lc "
export PATH='$UCRT64_PREFIX':`$PATH
cd '$MSYS_PROJECT_DIR' &&
mkdir -p dist &&
cp build/PixelCity.exe dist/ &&
ldd build/PixelCity.exe \
 | awk '/\/ucrt64\/bin\/.*\.dll/ {print \$3}' \
 | sort -u \
 | xargs -I{} cp -u {} dist/
"

Write-Host '▶ Running PixelCity from dist/' -ForegroundColor Green
.\dist\PixelCity.exe
