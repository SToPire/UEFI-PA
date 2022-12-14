SHELL := /bin/bash

VDISK = ovmf.disk
QEMU = qemu-system-x86_64
QEMU_OPT = --bios Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd \
		   -enable-kvm -debugcon file:"ovmf-boot.log" \
		   -global isa-debugcon.iobase=0x402 -global ICH9-LPC.disable_s3=1 \
		   -drive file=$(VDISK),format=raw,index=0,media=disk \
		   -net user,hostfwd=tcp::10022-:22 -net nic \
		   -m 4G
GDBPORT = 12306


build:
	$(MAKE) -C BaseTools
	. edksetup.sh && sleep 3 && build --arch=X64 --platform=OvmfPkg/OvmfPkgX64.dsc --tagname=GCC5

clean:
	$(MAKE) -C BaseTools clean
	rm -rf Build

qemu:
	rm -f ovmf-boot.log
	$(QEMU) $(QEMU_OPT)

qemu-gdb:
	$(QEMU) $(QEMU_OPT) -S -gdb tcp::$(GDBPORT)

.PHONY: build clean
