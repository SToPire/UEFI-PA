#ifndef _EFI_CUSTOM_RT_H
#define _EFI_CUSTOM_RT_H

#define EFI_CUSTOM_RT_SERVICE 0x19260817

enum efi_custom_rt_req_type {
    CUSTOM_RT_PERF_MONITOR,
};

struct efi_custom_rt_service_in{
    uint8_t type;
};
struct efi_custom_rt_service_out{
    uint64_t perf_monitor_res[8];
};

struct efi_custom_rt_service {
    struct efi_custom_rt_service_in *in;
    struct efi_custom_rt_service_out *out;
	unsigned long *status;
};

enum PerfEventType {
  PERF_LLCREF_PRIV,
  PERF_LLCMISS_PRIV,
  PERF_BRANCH_PRIV,
  PERF_MISBRAN_PRIV,
  PERF_LLCREF_USER,
  PERF_LLCMISS_USER,
  PERF_BRANCH_USER,
  PERF_MISBRAN_USER,
};

#endif