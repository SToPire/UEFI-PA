#ifndef _EFI_CUSTOM_RT_H
#define _EFI_CUSTOM_RT_H

#define EFI_CUSTOM_RT_SERVICE 0x19260817

struct efi_custom_rt_service_in{
    uint16_t dummy;
};
struct efi_custom_rt_service_out{
    uint16_t dummy;
};

struct efi_custom_rt_service {
    struct efi_custom_rt_service_in *in;
    struct efi_custom_rt_service_out *out;
	unsigned long *status;
};

#endif