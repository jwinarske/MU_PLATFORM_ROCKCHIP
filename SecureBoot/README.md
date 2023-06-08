# Secure Boot

_BL33 refers to UFEI_

* optee_os needs 
  * ASLR seed implementation
  * HW ID (eFuse) implementation

### Build optee-os-tadevkit

See build_ta_sdk() in tools/build.sh

### Build Trusted App (TA) fTPM

See build_ftpm() in tools/build.sh

### Build OPTEE OS

See build_optee_os() in tools/build.sh

### Build Trusted Firmware A with SPD=opteed

See build_atf() in tools/build.sh

## References

* OP-TEE
https://optee.readthedocs.io/en/latest/architecture/secure_boot.html?highlight=BL32#armv8-a-using-the-authentication-framework-in-tf-a

* Trusted Firmware-A
https://github.com/ARM-software/arm-trusted-firmware/tree/master/docs/getting_started
https://github.com/ARM-software/arm-trusted-firmware/blob/master/docs/design/auth-framework.rst
https://github.com/ARM-software/arm-trusted-firmware/blob/master/docs/design/trusted-board-boot.rst
https://trustedfirmware-a.readthedocs.io/en/latest/components/secure-partition-manager-mm.html
https://trustedfirmware-a.readthedocs.io/en/latest/design_documents/measured_boot_poc.html
https://trustedfirmware-a.readthedocs.io/en/v2.6/design/firmware-design.html#firmware-image-package-fip

* MU TEE Example
https://github.com/ms-iot/MU_PLATFORM_NXP

* edk2 aarch64 platform examples
https://github.com/tianocore/edk2-platforms

* TPM
https://github.com/tpm2-software/tpm2-tools


### Debug Boot Log

```
[13092286] handing off to BL31
NOTICE:  BL31: v2.9(debug):v2.9.0-dirty
NOTICE:  BL31: Built : 08:56:03, Jun  8 2023
INFO:    GICv3 with legacy support detected.
INFO:    ARM GICv3 driver initialized in EL3
INFO:    Maximum SPI INTID supported: 287
INFO:    plat_rockchip_pmu_init(1624): pd status 93cf833e
INFO:    BL31: Initializing runtime services
INFO:    BL31: cortex_a53: CPU workaround for 855873 was applied
INFO:    BL31: cortex_a53: CPU workaround for 1530924 was applied
INFO:    BL31: Initializing BL32
D/TC:0   plat_get_aslr_seed:116 Warning: no ASLR seed
D/TC:0   add_phys_mem:635 ROUNDDOWN((0xF8000000 + 0x06E00000), CORE_MMU_PGDIR_SIZE) type IO_SEC 0xfee00000 size 0x00200000
D/TC:0   add_phys_mem:635 ROUNDDOWN((0xF8000000 + 0x071A0000), CORE_MMU_PGDIR_SIZE) type IO_NSEC 0xff000000 size 0x00200000
D/TC:0   add_phys_mem:635 ROUNDDOWN((0xF8000000 + 0x07330000), CORE_MMU_PGDIR_SIZE) type IO_SEC 0xff200000 size 0x00200000
D/TC:0   add_phys_mem:635 TEE_SHMEM_START type NSEC_SHM 0x32000000 size 0x00400000
D/TC:0   add_phys_mem:635 TA_RAM_START type TA_RAM 0x30200000 size 0x01e00000
D/TC:0   add_phys_mem:635 VCORE_UNPG_RW_PA type TEE_RAM_RW 0x300a4000 size 0x0015c000
D/TC:0   add_phys_mem:635 VCORE_UNPG_RX_PA type TEE_RAM_RX 0x30000000 size 0x000a4000
D/TC:0   add_va_space:675 type RES_VASPACE size 0x00a00000
D/TC:0   add_va_space:675 type SHM_VASPACE size 0x02000000
D/TC:0   dump_mmap_table:800 type TEE_RAM_RX   va 0x30000000..0x300a3fff pa 0x30000000..0x300a3fff size 0x000a4000 (smallpg)
D/TC:0   dump_mmap_table:800 type TEE_RAM_RW   va 0x300a4000..0x301fffff pa 0x300a4000..0x301fffff size 0x0015c000 (smallpg)
D/TC:0   dump_mmap_table:800 type SHM_VASPACE  va 0x30200000..0x321fffff pa 0x00000000..0x01ffffff size 0x02000000 (pgdir)
D/TC:0   dump_mmap_table:800 type RES_VASPACE  va 0x32200000..0x32bfffff pa 0x00000000..0x009fffff size 0x00a00000 (pgdir)
D/TC:0   dump_mmap_table:800 type TA_RAM       va 0x32c00000..0x349fffff pa 0x30200000..0x31ffffff size 0x01e00000 (pgdir)
D/TC:0   dump_mmap_table:800 type NSEC_SHM     va 0x34a00000..0x34dfffff pa 0x32000000..0x323fffff size 0x00400000 (pgdir)
D/TC:0   dump_mmap_table:800 type IO_SEC       va 0x34e00000..0x34ffffff pa 0xfee00000..0xfeffffff size 0x00200000 (pgdir)
D/TC:0   dump_mmap_table:800 type IO_NSEC      va 0x35000000..0x351fffff pa 0xff000000..0xff1fffff size 0x00200000 (pgdir)
D/TC:0   dump_mmap_table:800 type IO_SEC       va 0x35200000..0x353fffff pa 0xff200000..0xff3fffff size 0x00200000 (pgdir)
D/TC:0   core_mmu_xlat_table_alloc:526 xlat tables used 1 / 5
D/TC:0   core_mmu_xlat_table_alloc:526 xlat tables used 2 / 5
I/TC: 
I/TC: OP-TEE version: 3.21.0 (gcc version 12.2.1 20220819 (Red Hat Cross 12.2.1-2) (GCC)) #2 Thu Jun  8 15:56:00 UTC 2023 aarch64
I/TC: WARNING: This OP-TEE configuration might be insecure!
I/TC: WARNING: Please check https://optee.readthedocs.io/en/latest/architecture/porting_guidelines.html
I/TC: Primary CPU initializing
D/TC:0 0 boot_init_primary_late:1457 Executing at offset 0 with virtual load address 0x30000000
D/TC:0 0 call_preinitcalls:21 level 2 mobj_mapped_shm_init()
D/TC:0 0 mobj_mapped_shm_init:463 Shared memory address range: 30200000, 32200000
D/TC:0 0 call_initcalls:40 level 1 register_time_source()
D/TC:0 0 call_initcalls:40 level 1 teecore_init_pub_ram()
D/TC:0 0 call_initcalls:40 level 3 platform_init()
D/TC:0 0 platform_secure_ddr_region:35 protecting region 1: 0x30000000-0x32000000
D/TC:0 0 call_initcalls:40 level 3 check_ta_store()
D/TC:0 0 check_ta_store:417 TA store: "early TA"
D/TC:0 0 check_ta_store:417 TA store: "REE"
D/TC:0 0 call_initcalls:40 level 3 early_ta_init()
D/TC:0 0 early_ta_init:56 Early TA bc50d971-d4c9-42c4-82cb-343fb7f37896 size 165734 (compressed, uncompressed 351232)
D/TC:0 0 early_ta_init:56 Early TA 023f8f1a-292a-432b-8fc4-de8471358067 size 33338 (compressed, uncompressed 59280)
D/TC:0 0 call_initcalls:40 level 3 verify_pseudo_tas_conformance()
D/TC:0 0 call_initcalls:40 level 3 tee_cryp_init()
D/TC:0 0 call_initcalls:40 level 4 tee_fs_init_key_manager()
D/TC:0 0 call_initcalls:40 level 6 mobj_init()
D/TC:0 0 call_initcalls:40 level 6 default_mobj_init()
I/TC: Primary CPU switching to normal world boot
INFO:    BL31: Preparing for EL3 exit to normal world
INFO:    Entry point address = 0xa00000
INFO:    SPSR = 0x3c5
UEFI firmware (version  built at 13:50:16 on Jun  7 2023)
```