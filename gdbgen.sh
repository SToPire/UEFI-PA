# see https://retrage.github.io/2019/12/05/debugging-ovmf-en.html

BUILDDIR="Build/OvmfX64/DEBUG_GCC5/X64"
# https://github.com/retrage/peinfo
PEINFO="peinfo/peinfo"

grep Loading <ovmf-boot.log | grep -i efi | while read LINE; do
    BASE="$(echo ${LINE} | awk '{print $4}')"
    NAME="$(echo ${LINE} | awk '{print $6}' | tr -d "[:cntrl:]")"
    FILE="$(find ${BUILDDIR} -maxdepth 1 -name ${NAME} -type f)"
    if [ -n "${FILE}" ]; then
        ADDR="$(${PEINFO} ${FILE} | grep -A 5 text | grep VirtualAddress | awk '{print $2}')"
        TEXT="$(python3 -c "print(hex(${BASE} + ${ADDR}))")"
        echo "add-symbol-file $(echo ${FILE} | sed -e "s/\.efi/\.debug/g") ${TEXT}"
    fi
done
