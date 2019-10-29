C:/ti/ccs/ccs/utils/bin/gmake.exe -k all || exit /b
rsync -azv --chmod=ugo=rwx ./gen/pstrobe.out rsync://debian@bba/aproc/pru



