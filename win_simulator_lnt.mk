#lint exec file
LINT = C:/lint/lint-nt.exe
LINT_OPTIONS = +linebuf +linebuf +linebuf +linebuf
ECHO = make/make_cmd/echo.exe
RM_FILE = C:/cygwin/bin/rm -f
#the path
BUILD_DIR = build/$(p)_builddir
#the files
dsp_files = $(wildcard $(BUILD_DIR)/win/*/*.dsp)
lnt_files = $(dsp_files:.dsp=.lnt)

.PHONY: all
all: create

.PHONY: create
create: $(lnt_files)
$(lnt_files): %.lnt : %.dsp
	@-$(ECHO) $<
	@-$(LINT) $(LINT_OPTIONS) $< > $@ 

.PHONY: clean
clean:
	@-$(RM_FILE) $(lnt_files)
