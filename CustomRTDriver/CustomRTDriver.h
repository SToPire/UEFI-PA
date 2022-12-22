#ifndef _CUSTOM_RTDRIVER_H_
#define _CUSTOM_RTDRIVER_H_

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

enum EFI_CUSTOM_RT_REQ_TYPE {
  CUSTOM_RT_PERF_MONITOR,
};

EFI_STATUS
EFIAPI
InitializeCustimRTDriver (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

#endif
