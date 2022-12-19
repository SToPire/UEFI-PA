#include "CustomRTDriver.h"
#include "Uefi/UefiSpec.h"

EFI_STATUS
EFIAPI
CustomRTServiceEntry (
  IN  EFI_CUSTOM_RT_INPUT  in,
  OUT EFI_CUSTOM_RT_OUTPUT *out
  )
{
  DEBUG ((DEBUG_INFO, "My GetTime\n"));
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
InitializeCustimRTDriver (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  DEBUG ((DEBUG_INFO, "My Init\n"));

  gRT->CustomRtService = CustomRTServiceEntry;

  return Status;
}
