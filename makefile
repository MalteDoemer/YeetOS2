
all: YeetOS

.EXPORT_ALL_VARIABLES: .PHONY

ARCH=x86

TOPDIR	:= $(abspath .)
MAKE=make --no-print-directory

ARCHIVES=\
Kernel/Kernel.a \
LibYT/LibYT.a \
Arch/$(ARCH)/Kernel/Kernel.a \
Arch/$(ARCH)/Boot/Boot.a \

LD_SCRIPT = Arch/$(ARCH)/Link.ld

SUBDIRS= Kernel LibYT Arch/$(ARCH)/Kernel Arch/$(ARCH)/Boot

# should define $(CC) $(LD) $(AS) $(AR) 
# and respectively $(C_FLAGS) $(LD_FLAGS) $(AS_FLAGS)
include Arch/$(ARCH)/make.config

DEFINES := __KERNEL__ $(DEFINES)

ifdef DEBUG
DEFINES := _DEBUG __DEBUG $(DEFINES)
#else
#DEFINES := NDEBUG $(DEFINES)
endif

INCDIRS := $(TOPDIR)/Include $(TOPDIR)/Include/LibYT $(INCDIRS)

DEFINES := $(patsubst %,-D%,$(DEFINES))
INCDIRS := $(patsubst %,-I%,$(INCDIRS))

C_FLAGS := $(C_FLAGS) $(DEFINES) $(INCDIRS)
CXX_FLAGS := $(CXX_FLAGS) $(DEFINES) $(INCDIRS)


YeetOS: subdirs
	$(LD) -T $(LD_SCRIPT) $(LD_FLAGS) --whole-archive $(ARCHIVES) -o $@

subdirs:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean:
	rm -f YeetOS
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done

# to compensate typos XD
clena: clean

disasm:
	scripts/disasm.sh

format:
	scripts/format.sh

install: YeetOS
	scripts/install.sh

run: install
	scripts/run.sh

debug: install
	scripts/debug.sh