# Author: Rafael Garibotti

##### Project setup #####
# Name of your current project
PROJ_NAME = $(shell basename $(CURDIR))

LINKER_SCRIPT = ../STM32H745ZITx_FLASH_CM4.ld

# Directory containing CMSIS and HAL_Driver source code
CMSIS_DIR = ../Drivers/CMSIS
DRIVER_DIR = ../Drivers/STM32H7xx_HAL_Driver

##### Arm Toolchain #####
TRIPLE  = arm-none-eabi
CC      = ${TRIPLE}-gcc
LD      = ${TRIPLE}-ld
AS      = ${TRIPLE}-as
GDB 	= ${TRIPLE}-gdb
OBJCOPY = ${TRIPLE}-objcopy
OBJDUMP = ${TRIPLE}-objdump

##### Compiler options #####
CFLAGS = -g3 -std=gnu99 -Wall -T$(LINKER_SCRIPT)
CFLAGS += -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DUSE_FULL_LL_DRIVER -DSTM32H745xx
CFLAGS += -ffunction-sections -fdata-sections #-fstack-usage
CFLAGS += -mthumb -mcpu=cortex-m4 -Os -MMD -MP
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=nosys.specs --specs=nano.specs

##### Project specific libraries #####
SRC_FILES += $(wildcard Startup/*.s)
SRC_FILES += $(wildcard Src/*.c)
CFLAGS += -IInc

##### CMSIS libraries and source code #####
#SRC_FILES += $(CMSIS_DIR)/src/*
CFLAGS += -I$(CMSIS_DIR)/Include
CFLAGS += -I$(CMSIS_DIR)/Device/ST/STM32H7xx/Include

SRC_FILES += $(DRIVER_DIR)/Src/*
CFLAGS += -I$(DRIVER_DIR)/Inc

##### Wolfssl libraries and source code #####

# Wolfssl Defines
DEF = -DWOLFSSL_USER_SETTINGS

# Wolfssl Includes
CFLAGS += -I../wolfssl_lib/Header
CFLAGS += -I../wolfssl_lib

# WOLFSSL TLS FILES
SRC_FILES += ../wolfssl_lib/src/bio.c
SRC_FILES += ../wolfssl_lib/src/crl.c
SRC_FILES += ../wolfssl_lib/src/internal.c
SRC_FILES += ../wolfssl_lib/src/keys.c
SRC_FILES += ../wolfssl_lib/src/ocsp.c
SRC_FILES += ../wolfssl_lib/src/sniffer.c
SRC_FILES += ../wolfssl_lib/src/ssl.c
SRC_FILES += ../wolfssl_lib/src/tls.c
SRC_FILES += ../wolfssl_lib/src/tls13.c
SRC_FILES += ../wolfssl_lib/src/wolfio.c

SRC_FILES += ../wolfssl_lib/wolfcrypt/src/aes.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/cmac.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/des3.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/dh.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/ecc.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/hmac.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/random.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/rsa.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/sha.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/sha256.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/sha512.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/sha3.c

# wolfCrypt Additional
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/asm.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/asn.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/blake2s.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/chacha.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/chacha20_poly1305.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/coding.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/compress.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/cpuid.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/cryptocb.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/curve25519.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/curve448.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/ecc_fp.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/eccsi.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/ed25519.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/ed448.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/error.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/evp.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/fe_448.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/fe_low_mem.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/fe_operations.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/ge_448.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/ge_low_mem.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/ge_operations.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/hash.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/kdf.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/integer.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/logging.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/md5.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/memory.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/misc.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/pkcs12.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/pkcs7.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/poly1305.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/pwdbased.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/rc2.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/sakke.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/signature.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/srp.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/sp_arm32.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/sp_arm64.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/sp_armthumb.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/sp_c32.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/sp_c64.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/sp_cortexm.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/sp_dsp32.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/sp_int.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/sp_x86_64.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/tfm.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/wc_dsp.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/wc_encrypt.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/wc_pkcs11.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/wc_port.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/wolfevent.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/wolfmath.c

ifeq "$(ASYNC)" "1"
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/async.c
endif

ifeq "$(SELFTEST)" "1"
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/selftest.c
endif

# wolfCrypt non-standard algorithms (disabled by default)
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/arc4.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/blake2b.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/camellia.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/dsa.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/md2.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/md4.c
SRC_FILES += ../wolfssl_lib/wolfcrypt/src/ripemd.c


all: elf

##### Flash code to board #####
flash: hex
	STM32_Programmer_CLI -c port=SWD -w $(shell pwd)/$(PROJ_NAME).hex -v -ob displ -rst

##### Serial communication program #####
minicom:
	sudo minicom -D /dev/ttyACM0 -b 115200

##### Debug: GDB commands #####
gdb-server:
	ST-LINK_gdbserver

gdb-client:
	$(GDB) $(PROJ_NAME).elf -ex "target extended-remote localhost:61234" -ex "load" 

gdb:
	$(GDB) $(PROJ_NAME).elf -ex "target extended-remote localhost:61234" -ex "load" -ex "break main" -ex "continue"

# Print out disassembly of specified function using GDB (Example: make disass FUNC=main)
disass:
	$(GDB) $(PROJ_NAME).elf -batch -ex 'disass /r $(FUNC)'

##### General commands #####
clean:
	rm -f $(PROJ_NAME).bin $(PROJ_NAME).hex $(PROJ_NAME).elf $(PROJ_NAME).d $(PROJ_NAME).s *.su

disass-all:
	$(OBJDUMP) -D $(PROJ_NAME).elf > $(PROJ_NAME).s

elf:	$(SRC_FILES)
	$(CC) $(CFLAGS) $(DEF) -o $(PROJ_NAME).elf $^

hex:	
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex

bin:	
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

