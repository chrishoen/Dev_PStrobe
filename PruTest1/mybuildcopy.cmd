C:/ti/ccs/ccs/utils/bin/gmake.exe -k all || exit /b
rsync -azv --chmod=ugo=rwx ./gen/prutest1.out rsync://debian@bbx/cproc/pru



