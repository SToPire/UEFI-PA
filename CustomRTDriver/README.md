We implement a UEFI custom runtime service driver with several dummy funcs.

#### File Structure

```
edk2/CustomRTDriver.c
├── CustomRTDriver.c
├── CustomRTDriver.h
├── CustomRTDriver.inf
├── README.md
└── linux-tools # non-uefi part of this programming assignment
    ├── kmodule	# kernel module as a wrapper of custom efi runtime service
    │   ├── Makefile
    │   ├── efi_custom_rt.c
    │   └── efi_custom_rt.h
    ├── user_tool.c # user space tool issuing custom efi runtime service
    └── kernel.patch # modify v5.15 linux kernel to support custom efi runtime service
```

---

#### Environment

- Host: WSL Ubuntu 22.04
- OVMF: tianocore/edk2, with tag `edk2-stable202211`
- Guest: Ubuntu 22.04, kernel version `v5.15`

#### Usage

1. Compile OVMF firmware with `make build`.

2. Preparing a qemu VM disk called `ovmf.disk`, then install Guest OS.

3. Get Linux kernel source with tag `v5.15`, apply the patch`kernel.patch`.

4. Compile the new kernel, with `CONFIG_EFI_MIXED` **NOT** set.

5. Switch to the new kernel, compile and install kernel module `efi_custom_rt`.

6. Compile and run user program `user_tool`.

#### Procedure of Development

1. Build UEFI.
    
    Save your codes, run `make build` at *UEFI-PA/*. Run `[sudo] make qemu` in the same directory.
2. Connect to VM.
   
   Connect to uefi vm with `ssh -p 10022 uefi@localhost`.
3. Install the new module.
   
   Check out to module directory with `cd efi-runtime/src`, and run `make`.Install new kernel module with `sudo insmod efi_custom_rt.ko` at *efi-runtime/src*.
4. Update user_tool.
   
   Modify **user_tool.c** in the **virtual machine**. After coding, run `gcc user_tools.c -o user_tool`.
5. Run user_tool.

   