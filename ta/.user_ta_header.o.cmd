old-cmd-./user_ta_header.o := /home/optee/RPi3/build/../toolchains/aarch64/bin/aarch64-linux-gnu-gcc -std=gnu99 -Werror -fdiagnostics-show-option -Wall -Wcast-align -Werror-implicit-function-declaration -Wextra -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wshadow -Wstrict-prototypes -Wswitch-default -Wwrite-strings -Wno-missing-field-initializers -Wno-format-zero-length -Waggregate-return -Wredundant-decls -Wold-style-definition -Wstrict-aliasing=2 -Wundef -pedantic -Wdeclaration-after-statement -Wno-error=cast-align -O0 -g3 -fpie -mstrict-align -MD -MF ./.user_ta_header.o.d -MT user_ta_header.o -nostdinc -isystem /home/optee/RPi3/toolchains/aarch64/bin/../lib/gcc/aarch64-linux-gnu/5.3.1/include -DCFG_TEE_TA_LOG_LEVEL=4 -I./include -DARM64=1 -D__LP64__=1 -DTRACE_LEVEL=4 -I. -I/home/optee/RPi3/optee_os/out/arm/export-ta_arm64/include -c /home/optee/RPi3/optee_os/out/arm/export-ta_arm64/src/user_ta_header.c -o user_ta_header.o