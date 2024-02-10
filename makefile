SRC := $(wildcard src/*.c)
SRC += $(wildcard vendor/*.c)
OBJS := $(patsubst %.c,build/%.o,$(notdir $(SRC)))

build/vdm: init $(OBJS)
	@echo "  CC \t\033[35;1m$(notdir $@)\033[0m"
	@$(CC) $(OBJS) -o $@ -lm

init:
	@mkdir -p build
	# @./scripts/genh

build/%.o: src/%.c
	@echo "  CC \t$(notdir $@)"
	@$(CC) $(CCPARAMS) -o $@ -c $< -I include/ -I .

build/%.o: vendor/%.c
	@echo "  CC \t$(notdir $@)"
	@$(CC) $(CCPARAMS) -o $@ -c $< -I include/ -I .

run: build/vdm
	@build/vdm

clean:
	@rm -rf build/

