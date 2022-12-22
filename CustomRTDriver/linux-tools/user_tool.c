#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "kmodule/efi_custom_rt.h"

void _printPerfMonitorStats(struct efi_custom_rt_service_out *out)
{
	uint64_t *res = out->perf_monitor_res;
	// No user pmc data -> User&Priv stats are not seperated.
	int SeperateUserAndPriv = res[PERF_LLCMISS_USER];

	printf("Last Level Cache(LLC) Stat:\n");
	if (SeperateUserAndPriv) {
		printf("\tUser(miss/total): %lu/%lu, %.2f%%\n",
		       res[PERF_LLCMISS_USER], res[PERF_LLCREF_USER],
		       res[PERF_LLCMISS_USER] * 100.0f / res[PERF_LLCREF_USER]);
	}
	printf("\t%s(miss/total): %lu/%lu, %.2f%%\n",
	       SeperateUserAndPriv ? "Priv" : "", res[PERF_LLCMISS_PRIV],
	       res[PERF_LLCREF_PRIV],
	       res[PERF_LLCMISS_PRIV] * 100.0f / res[PERF_LLCREF_PRIV]);
	printf("Branch Prediction Stat:\n");
	if (SeperateUserAndPriv) {
		printf("\tUser(miss/total): %lu/%lu, %.2f%%\n",
		       res[PERF_MISBRAN_USER], res[PERF_BRANCH_USER],
		       res[PERF_MISBRAN_USER] * 100.0f / res[PERF_BRANCH_USER]);
	}
	printf("\t%s(miss/total): %lu/%lu, %.2f%%\n",
	       SeperateUserAndPriv ? "Priv" : "", res[PERF_MISBRAN_PRIV],
	       res[PERF_BRANCH_PRIV],
	       res[PERF_MISBRAN_PRIV] * 100.0f / res[PERF_BRANCH_PRIV]);
}

int main(int argc, char *argv[])
{
	int fd;
	int ret;

	struct efi_custom_rt_service_in in;
	struct efi_custom_rt_service_out out;
	unsigned long status;
	struct efi_custom_rt_service arg;

	if (argc != 2) {
		fprintf(stderr, "Usage: user_tool [perf]\n");
		return -1;
	}

	fd = open("/dev/efi_custom_rt", O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "open /dev/efi_custom_rt fail\n");
		return fd;
	}

	if (strcmp(argv[1], "perf") == 0) {
		in.type = CUSTOM_RT_PERF_MONITOR;
	} else {
		fprintf(stderr, "Unknown service name: %s\n", argv[1]);
		return -1;
	}

	arg.in = &in;
	arg.out = &out;
	arg.status = &status;

	ret = ioctl(fd, EFI_CUSTOM_RT_SERVICE, &arg);

	if (strcmp(argv[1], "perf") == 0) {
		_printPerfMonitorStats(&out);
	} else {
		fprintf(stderr, "Unknown service name: %s\n", argv[1]);
		return -1;
	}

	return 0;
}