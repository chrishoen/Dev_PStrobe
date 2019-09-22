C:/ti/ccs/ccs/utils/bin/gmake.exe -k all || exit /b
rsync -azv --chmod=ugo=rwx ./gen/prumsg1.out rsync://debian@bbx/aproc/pru



