# Secure Boot

_BL33 refers to UFEI_

* optee_os needs 
  * Crypto Driver (TRNG+)
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

