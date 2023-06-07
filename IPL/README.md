# levinboot

BL2

### USB DRAM loader

    CROSS=aarch64-linux-gnu
    export CC=$CROSS-gcc
    export OBJCOPY=$CROSS-objcopy
    export LD=$CROSS-ld

    cd IPL/levinboot
    # git apply ../0001-Ignore-false-positive-stringop-overflow-warning.patch
    # git apply ../0002-Fix-linking-with-GCC-12.2.0.patch
    # git apply ../0003-Enable-ELF-loading-via-dramstage.patch
    # git apply ../0004-Minimal-Build.patch
    git apply ../0005-Boot-BL31-BL32-BL33-from-RAM.patch

    mkdir _build && cd _build
    ../configure.py --with-tf-a-headers ../../../Silicon/Arm/TFA/include/export --boards pbp
    ninja
    
    mkdir tools && cd tools
    CC=gcc ../../tools/configure
    ninja

### Load and run BL31

    sudo tools/usbtool --call sramstage-usb.bin --bulk --load 4200000 ../../../Silicon/Arm/TFA/build/rk3399/debug/bl31/bl31.elf --load 4000000 dramstage.bin --start 4000000 4204000

Log tail should look like this

    dramstage
    trng: 100 0
    LOAD 00010000…000370de → 00040000
    LOAD 00038000…00039f58 → ff3b0000
    LOAD 0003a000…0003b000 → ff8c0000
    LOAD 0003b000…0003c000 → ff8c1000
    LOAD 00000000…00000000 → ff8c2000
    ignoring GNU_STACK segment
    [12051156] handing off to BL31
    NOTICE:  BL31: v2.9(debug):v2.9.0-71-g9b5c0fcdb-dirty
    NOTICE:  BL31: Built : 07:43:03, Jun  5 2023
    INFO:    GICv3 with legacy support detected.
    INFO:    ARM GICv3 driver initialized in EL3
    INFO:    Maximum SPI INTID supported: 287
    INFO:    plat_rockchip_pmu_init(1624): pd status 93cf833e
    INFO:    BL31: Initializing runtime services
    INFO:    Initializing Service: opteed_fast
    INFO:    Initializing Service: std_svc
    INFO:    BL31: cortex_a53: CPU workaround for 855873 was applied
    WARNING: BL31: cortex_a53: CPU workaround for 1530924 was missing!
    INFO:    BL31: Initializing BL32

### Load and run BL31 -> BL32

    