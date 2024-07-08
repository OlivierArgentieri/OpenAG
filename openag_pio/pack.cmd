set outdir=.\.pio\build\esp32outa
set indir=.\.pio\build\aaaa
mkdir %outdir%
python -m esptool --chip esp32 merge_bin -o %outdir%/merged_firmware.bin --flash_mode dio --flash_freq 40m --flash_size 4MB 0x1000 %indir%/bootloader.bin 0x8000 %indir%/partitions.bin 0x10000 %indir%/firmware.bin

copy %indir%\firmware.bin %outdir%\firmware.bin
copy %indir%\partitions.bin %outdir%\partitions.bin
copy %indir%\bootloader.bin %outdir%/bootloader.bin
copy %indir%\spiffs.bin %outdir%\spiffs.bin
