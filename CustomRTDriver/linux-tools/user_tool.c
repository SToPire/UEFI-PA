#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdint.h>

#include "kmodule/efi_custom_rt.h"

int main()
{
	int fd = open("/dev/efi_custom_rt", O_RDWR);
	if (fd < 0) {
		return 1;
	}

	struct efi_custom_rt_service_in in;
	struct efi_custom_rt_service_out out;
	unsigned long status;

	in.dummy = 23333;

	struct efi_custom_rt_service arg;
	arg.in = &in;
	arg.out = &out;
	arg.status = &status;

	int ret = ioctl(fd, EFI_CUSTOM_RT_SERVICE, &arg);
	printf("ret=%d, out->dummy=%u\n", ret, out.dummy);
	return 0;
}