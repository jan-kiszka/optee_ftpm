//** Introduction
// Provide vendor-specific version and identifiers to core TPM library for
// return in capabilities.  These may not be compile time constants and therefore
// are provided by platform callbacks.  These platform functions are expected to
// always be available, even in failure mode.
//
//** Includes
#include "platform_interface/tpm_to_platform_interface.h"

// In this sample platform, these are compile time constants, but are not required to be.
#define MANUFACTURER    "OPTE"
#define VENDOR_STRING_1 "xCG "
#define VENDOR_STRING_2 "fTPM"
#define VENDOR_STRING_3 "\0\0\0\0"
#define VENDOR_STRING_4 "\0\0\0\0"
#define FIRMWARE_V1     (0x20250818)
#define FIRMWARE_V2     (0x00120000)
#define MAX_SVN         255

static uint16_t currentSvn  = 10;

// Similar to the Core Library's ByteArrayToUint32, but usable in Platform code.
static uint32_t StringToUint32(const char s[4])
{
    uint8_t* b = (uint8_t*)s;  // Avoid promotion to a signed integer type
    return (((uint32_t)b[0] << 8 | b[1]) << 8 | b[2]) << 8 | b[3];
}

// return the 4 character Manufacturer Capability code.  This
// should come from the platform library since that is provided by the manufacturer
LIB_EXPORT uint32_t _plat__GetManufacturerCapabilityCode(void)
{
    return StringToUint32(MANUFACTURER);
}

// return the 4 character VendorStrings for Capabilities.
// Index is ONE-BASED, and may be in the range [1,4] inclusive.
// Any other index returns all zeros. The return value will be interpreted
// as an array of 4 ASCII characters (with no null terminator)
LIB_EXPORT uint32_t _plat__GetVendorCapabilityCode(int index)
{
    switch(index)
    {
        case 1:
            return StringToUint32(VENDOR_STRING_1);
        case 2:
            return StringToUint32(VENDOR_STRING_2);
        case 3:
            return StringToUint32(VENDOR_STRING_3);
        case 4:
            return StringToUint32(VENDOR_STRING_4);
    }
    return 0;
}

// return the most-significant 32-bits of the TPM Firmware Version reported by
// getCapability.
LIB_EXPORT uint32_t _plat__GetTpmFirmwareVersionHigh(void)
{
    return FIRMWARE_V1;
}

// return the least-significant 32-bits of the TPM Firmware Version reported by
// getCapability.
LIB_EXPORT uint32_t _plat__GetTpmFirmwareVersionLow(void)
{
    return FIRMWARE_V2;
}

// return the TPM Firmware SVN reported by getCapability.
LIB_EXPORT uint16_t _plat__GetTpmFirmwareSvn(void)
{
    return currentSvn;
}

// return the TPM Firmware maximum SVN reported by getCapability.
LIB_EXPORT uint16_t _plat__GetTpmFirmwareMaxSvn(void)
{
    return MAX_SVN;
}

// return the TPM Type returned by TPM_PT_VENDOR_TPM_TYPE
LIB_EXPORT uint32_t _plat__GetTpmType(void)
{
    return 1;  // just the value the reference code has returned in the past.
}
