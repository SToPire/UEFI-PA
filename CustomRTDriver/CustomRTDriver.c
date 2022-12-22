#include "CustomRTDriver.h"
#include "Base.h"
#include "Library/BaseLib.h"
#include "Register/Intel/ArchitecturalMsr.h"
#include "Uefi/UefiBaseType.h"
#include "Uefi/UefiSpec.h"

/* Performance Monitor Service */
enum _PerfEventType {
  PERF_EVENT_LLCREF_PRIV,
  PERF_EVENT_LLCMISS_PRIV,
  PERF_EVENT_BRANCH_PRIV,
  PERF_EVENT_MISBRAN_PRIV,
  PERF_EVENT_LLCREF_USER,
  PERF_EVENT_LLCMISS_USER,
  PERF_EVENT_BRANCH_USER,
  PERF_EVENT_MISBRAN_USER,
};

struct {
  UINT32 EventSelect;
  UINT32 UnitMask;
  UINT32 UserMode;
  UINT32 OSMode;
  enum _PerfEventType Type;
} _PerfEventSelections[8] = {
    {0x2E, 0x4F, 0x0, 0x1, PERF_EVENT_LLCREF_PRIV},
    {0x2E, 0x41, 0x0, 0x1, PERF_EVENT_LLCMISS_PRIV},
    {0xC4, 0x00, 0x0, 0x1, PERF_EVENT_BRANCH_PRIV},
    {0xC5, 0x00, 0x0, 0x1, PERF_EVENT_MISBRAN_PRIV},
    {0x2E, 0x4F, 0x1, 0x0, PERF_EVENT_LLCREF_USER},
    {0x2E, 0x41, 0x1, 0x0, PERF_EVENT_LLCMISS_USER},
    {0xC4, 0x00, 0x1, 0x0, PERF_EVENT_BRANCH_USER},
    {0xC5, 0x00, 0x1, 0x0, PERF_EVENT_MISBRAN_USER},
};

UINT8 NumPMCounter, PMCWidth;

EFI_STATUS
InitializePerfMonitor(
  VOID
  )
{
  UINT32 eax, ebx, ecx, edx;
  UINT8 PMVersion;
  MSR_IA32_PERFEVTSEL_REGISTER perfevtsel_reg;

  /* Get Perf Monitor availability. */
  AsmCpuid(0x0A, &eax, &ebx, &ecx, &edx);

  PMVersion = eax & 0xFF;
  NumPMCounter = (eax >> 8) & 0xFF;
  PMCWidth = (eax >> 16) & 0xFF;

  if (PMVersion <= 0) {
    DEBUG((DEBUG_INFO, "PerfMonitor not supported\n"));
    return EFI_UNSUPPORTED;
  }

  DEBUG((DEBUG_INFO, "PerfMonitor Version=%u, NumPMCounter=%u, PMCWidth=%u\n",
         PMVersion, NumPMCounter, PMCWidth));

  for (int i = 0; i < NumPMCounter; i++) {
    perfevtsel_reg.Uint64 = 0;

    perfevtsel_reg.Bits.EventSelect = _PerfEventSelections[i].EventSelect;
    perfevtsel_reg.Bits.UMASK = _PerfEventSelections[i].UnitMask;
    perfevtsel_reg.Bits.USR = _PerfEventSelections[i].UserMode;
    perfevtsel_reg.Bits.OS = _PerfEventSelections[i].OSMode;
    perfevtsel_reg.Bits.EN = 1;

    /* No enough PMC, do not seperate user&priv stats. */
    if (NumPMCounter < 8) {
      perfevtsel_reg.Bits.USR = 1;
      perfevtsel_reg.Bits.OS = 1;
    }

    /* see SDM 19.2.1 */
    AsmWriteMsr64(MSR_IA32_PERFEVTSEL0 + i, perfevtsel_reg.Uint64);
  }

  /* see SDM 19.2.2 */
  AsmWriteMsr64(MSR_IA32_PERF_GLOBAL_CTRL, (1UL << NumPMCounter) - 1);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
CustomRTServiceEntry (
  IN  EFI_CUSTOM_RT_INPUT  *in,
  OUT EFI_CUSTOM_RT_OUTPUT *out
  )
{
  switch (in->TYPE) {
  case CUSTOM_RT_PERF_MONITOR:
    for (UINT8 i = 0; i < NumPMCounter; i++) {
      out->PERF_MONITOR_RES[i] =
          AsmReadMsr64(MSR_IA32_PMC0 + i) & ((1UL << PMCWidth) - 1);
    }
    break;
  default:
    DEBUG((DEBUG_INFO, "Unsupported service type=%u\n", in->TYPE));
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
InitializeCustimRTDriver (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  Status = InitializePerfMonitor();

  gRT->CustomRTService = CustomRTServiceEntry;

  return Status;
}
