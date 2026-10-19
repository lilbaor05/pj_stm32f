$ErrorActionPreference = "Stop"

$CC      = "arm-none-eabi-gcc"
$OBJCOPY = "arm-none-eabi-objcopy"
$SIZE    = "arm-none-eabi-size"

$BUILD = "build"

Write-Host "=== Grape32 Bare-Metal Build ==="

# Tao folder build neu chua co
New-Item -ItemType Directory -Force $BUILD | Out-Null


# ==========================================================
# Compiler flags
# ==========================================================

$COMMON = @(
    "-mcpu=cortex-m3",
    "-mthumb",
    "-DSTM32F103xE",
    "-IDrivers/CMSIS/Include",
    "-IDrivers/CMSIS/Device/ST/STM32F1xx/Include",
    "-ffunction-sections",
    "-fdata-sections",
    "-Wall",
    "-Wextra",
    "-O0",
    "-g3"
)


# ==========================================================
# Compile main.c
# ==========================================================

Write-Host "[1/5] Compile main.c"

& $CC @COMMON `
    "-std=c11" `
    "-c" "src/main.c" `
    "-o" "$BUILD/main.o"

if ($LASTEXITCODE -ne 0)
{
    exit $LASTEXITCODE
}


# ==========================================================
# Compile system_stm32f1xx.c
# ==========================================================

Write-Host "[2/5] Compile system_stm32f1xx.c"

& $CC @COMMON `
    "-std=c11" `
    "-c" "src/system_stm32f1xx.c" `
    "-o" "$BUILD/system_stm32f1xx.o"

if ($LASTEXITCODE -ne 0)
{
    exit $LASTEXITCODE
}


# ==========================================================
# Assemble startup
# ==========================================================

Write-Host "[3/5] Assemble startup"

& $CC `
    "-mcpu=cortex-m3" `
    "-mthumb" `
    "-x" "assembler-with-cpp" `
    "-c" "startup/startup_stm32f103xe.s" `
    "-o" "$BUILD/startup_stm32f103xe.o"

if ($LASTEXITCODE -ne 0)
{
    exit $LASTEXITCODE
}


# ==========================================================
# Link
# ==========================================================

Write-Host "[4/5] Link firmware"

& $CC `
    "-mcpu=cortex-m3" `
    "-mthumb" `
    "$BUILD/main.o" `
    "$BUILD/system_stm32f1xx.o" `
    "$BUILD/startup_stm32f103xe.o" `
    "-TSTM32F103RC.ld" `
    "-Wl,--gc-sections" `
    "-Wl,-Map=$BUILD/grape32.map" `
    "--specs=nano.specs" `
    "--specs=nosys.specs" `
    "-o" "$BUILD/grape32.elf"

if ($LASTEXITCODE -ne 0)
{
    exit $LASTEXITCODE
}


# ==========================================================
# Tao HEX + BIN
# ==========================================================

Write-Host "[5/5] Generate HEX and BIN"

& $OBJCOPY `
    "-O" "ihex" `
    "$BUILD/grape32.elf" `
    "$BUILD/grape32.hex"

& $OBJCOPY `
    "-O" "binary" `
    "$BUILD/grape32.elf" `
    "$BUILD/grape32.bin"


Write-Host ""
Write-Host "=== BUILD SUCCESS ==="
Write-Host ""

& $SIZE "$BUILD/grape32.elf"