# JAVASCRIPT ECO ENVIRONMENT
#----------------------------------------------------------------------------#
#                          urjs                                              #
#----------------------------------------------------------------------------#


MCFLAG_OPT += --c99

JAVASCRIPT_DIR := JEE
JAVASCRIPT_DEPS_DIR := $(JAVASCRIPT_DIR)/deps
JAVASCRIPT_INC_DIR := $(JAVASCRIPT_DIR)/include
JAVASCRIPT_INC_DIR += $(JAVASCRIPT_DIR)/include/export



JAVASCRIPT_SRC_DIR := $(JAVASCRIPT_DIR)/src
JAVASCRIPT_ENGINE_DIR := $(JAVASCRIPT_DEPS_DIR)/scriptengine
JAVASCRIPT_TEST_DIR := $(JAVASCRIPT_DIR)/test
JERRYSCRIPT_DIR := $(JAVASCRIPT_DEPS_DIR)/jerryscript

MINCPATH = $(JAVASCRIPT_INC_DIR)
MSRCPATH = $(JAVASCRIPT_SRC_DIR)


# jerryscript engineer include path
JAVAYSCRIPT_ENGINEER_INCPATH = $(JERRYSCRIPT_DIR)/jerry-core/include \
                               $(JERRYSCRIPT_DIR)/jerry-ext/include \
                               $(JERRYSCRIPT_DIR)/temp
MINCPATH += $(JAVAYSCRIPT_ENGINEER_INCPATH)

# mocor include path
MINCPATH += MS_Ref/export/inc \
            common/export/inc \
            common/export/inc/trace_id \
            RTOS/export/inc \
			BASE/l4/export/inc \
            MS_Customize/export/inc \
            chip_drv/export/inc \
            DAPS/export/inc \
            char_lib/export/inc \
            ual/inc \
            ual/inc/device \
			chip_drv/export/inc/outdated
# jerryscript api ---begin

JAVASCRIPT_SRC_API_DIR = $(JAVASCRIPT_SRC_DIR)/api


# jerryscript src api bindings --begin
JAVASCRIPT_INC_DIR += $(JAVASCRIPT_SRC_API_DIR)/bindings/inc
JAVASCRIPT_SRC_API_BINDINGS_DIR = $(JAVASCRIPT_SRC_API_DIR)/bindings/src
MSRCPATH += $(JAVASCRIPT_SRC_API_BINDINGS_DIR)
SOURCES += js_bindings.c

# jerryscript src api bindings --end

# jerryscript src api modules --begin

JAVASCRIPT_SRC_API_MODULES_DIR = $(JAVASCRIPT_SRC_API_DIR)/modules

JAVASCRIPT_SRC_API_MODULES_GUI_DIR = $(JAVASCRIPT_SRC_API_MODULES_DIR)/gui
MINCPATH += $(JAVASCRIPT_SRC_API_MODULES_GUI_DIR)/inc
MSRCPATH += $(JAVASCRIPT_SRC_API_MODULES_GUI_DIR)/src
SOURCES += js_button.c \
           js_component.c \
           js_dashboard.c \
           js_event.c \
           js_image.c \
           js_page.c \
           js_gui_list.c \
           js_text.c \
           js_progressbar.c \
           js_edittext.c \
           js_div.c \
           js_gui_loop.c \
           js_slider.c \
           jerry_gui.c


# jerryscript src api packagemanager --begin

JAVASCRIPT_SRC_API_MODULES_PACKAGEMANAGER_DIR = $(JAVASCRIPT_SRC_API_MODULES_DIR)/packagemanager
MINCPATH += $(JAVASCRIPT_SRC_API_MODULES_PACKAGEMANAGER_DIR)
MSRCPATH += $(JAVASCRIPT_SRC_API_MODULES_PACKAGEMANAGER_DIR)
SOURCES += jerry_packagemanager.c

# jerryscript src api packagemanager --end

# jerryscript src api resourceload --begin

JAVASCRIPT_SRC_API_MODULES_RESOURCELOAD_DIR = $(JAVASCRIPT_SRC_API_MODULES_DIR)/resourceload
MINCPATH += $(JAVASCRIPT_SRC_API_MODULES_RESOURCELOAD_DIR)
MSRCPATH += $(JAVASCRIPT_SRC_API_MODULES_RESOURCELOAD_DIR)
SOURCES += jerry_resourceload.c

# jerryscript src api resourceload --end

# jerryscript src api permission --begin

JAVASCRIPT_SRC_API_MODULES_PERMISSION_DIR = $(JAVASCRIPT_SRC_API_MODULES_DIR)/permission
MINCPATH += $(JAVASCRIPT_SRC_API_MODULES_PERMISSION_DIR)
MSRCPATH += $(JAVASCRIPT_SRC_API_MODULES_PERMISSION_DIR)
SOURCES += jerry_permission.c

# jerryscript src api permission --end

JAVASCRIPT_SRC_API_MODULES_WATCHFACE_DIR = $(JAVASCRIPT_SRC_API_MODULES_DIR)/watchface
MINCPATH += $(JAVASCRIPT_SRC_API_MODULES_WATCHFACE_DIR)/inc
MSRCPATH += $(JAVASCRIPT_SRC_API_MODULES_WATCHFACE_DIR)/src
SOURCES += js_watchface_image.c \
           js_watchface_background.c \
           js_watchface_analogtime.c
SOURCES += js_watchface_day.c
SOURCES += js_watchface_digitaltime.c
SOURCES += js_watchface_month.c
SOURCES += js_watchface_multinum.c
SOURCES += js_watchface_week.c



JAVASCRIPT_SRC_API_MODULES_FS_DIR = $(JAVASCRIPT_SRC_API_MODULES_DIR)/fs
MINCPATH += $(JAVASCRIPT_SRC_API_MODULES_FS_DIR)/inc
MSRCPATH += $(JAVASCRIPT_SRC_API_MODULES_FS_DIR)/src
SOURCES += js_file.c

JAVASCRIPT_SRC_API_MODULES_KV_DIR = $(JAVASCRIPT_SRC_API_MODULES_DIR)/kv
MINCPATH += $(JAVASCRIPT_SRC_API_MODULES_KV_DIR)
MSRCPATH += $(JAVASCRIPT_SRC_API_MODULES_KV_DIR)
SOURCES +=   jerry_kv.c

JAVASCRIPT_SRC_API_MODULES_LOG_DIR = $(JAVASCRIPT_SRC_API_MODULES_DIR)/console
MINCPATH += $(JAVASCRIPT_SRC_API_MODULES_LOG_DIR)/inc
MSRCPATH += $(JAVASCRIPT_SRC_API_MODULES_LOG_DIR)/src
SOURCES += js_console.c

JAVASCRIPT_SRC_API_MODULES_AUDIO_DIR = $(JAVASCRIPT_SRC_API_MODULES_DIR)/audio
MINCPATH += $(JAVASCRIPT_SRC_API_MODULES_AUDIO_DIR)/inc
MSRCPATH += $(JAVASCRIPT_SRC_API_MODULES_AUDIO_DIR)/src
SOURCES += js_audio.c js_audio_event.c

JAVASCRIPT_SRC_API_MODULES_ART_DIR = $(JAVASCRIPT_SRC_API_MODULES_DIR)/jart
MINCPATH += $(JAVASCRIPT_SRC_API_MODULES_ART_DIR)/inc
MSRCPATH += $(JAVASCRIPT_SRC_API_MODULES_ART_DIR)/src
SOURCES += js_art.c

JAVASCRIPT_SRC_API_MODULES_I18N_DIR = $(JAVASCRIPT_SRC_API_MODULES_DIR)/i18n
MINCPATH += $(JAVASCRIPT_SRC_API_MODULES_I18N_DIR)/inc
MSRCPATH += $(JAVASCRIPT_SRC_API_MODULES_I18N_DIR)/src
SOURCES += js_i18n.c


JAVASCRIPT_SRC_API_MODULES_FEF_DIR = $(JAVASCRIPT_SRC_API_MODULES_DIR)/fef
MINCPATH += $(JAVASCRIPT_SRC_API_MODULES_FEF_DIR)/inc
MSRCPATH += $(JAVASCRIPT_SRC_API_MODULES_FEF_DIR)/src
SOURCES += js_fef.c


JAVASCRIPT_SRC_API_MODULES_DEVICE_DIR = $(JAVASCRIPT_SRC_API_MODULES_DIR)/device
MINCPATH += $(JAVASCRIPT_SRC_API_MODULES_DEVICE_DIR)/inc
MSRCPATH += $(JAVASCRIPT_SRC_API_MODULES_DEVICE_DIR)/src
SOURCES += js_device.c


JAVASCRIPT_SRC_API_MODULES_BUFFER_DIR = $(JAVASCRIPT_SRC_API_MODULES_DIR)/buffer
MINCPATH += $(JAVASCRIPT_SRC_API_MODULES_BUFFER_DIR)/inc
MSRCPATH += $(JAVASCRIPT_SRC_API_MODULES_BUFFER_DIR)/src
SOURCES += js_buffer.c

JAVASCRIPT_SRC_API_MODULES_TIMER_DIR = $(JAVASCRIPT_SRC_API_MODULES_DIR)/timer
MINCPATH += $(JAVASCRIPT_SRC_API_MODULES_TIMER_DIR)/inc
MSRCPATH += $(JAVASCRIPT_SRC_API_MODULES_TIMER_DIR)/src
SOURCES += js_timer.c


# jerryscript src api modules ---end

# jerryscript src api ---end



# jerryscript src art --begin

JAVASCRIPT_SRC_ART_DIR = $(JAVASCRIPT_SRC_DIR)/art
MINCPATH += $(JAVASCRIPT_SRC_ART_DIR)/inc
MINCPATH += MS_MMI_Main/source/mmi_app/app/permissionmanager/inc
MSRCPATH += $(JAVASCRIPT_SRC_ART_DIR)/src
SOURCES += urjs_art_bind.c \
           urjs_art_engine.c \
           urjs_art_main.c \
           urjs_art_module.c \
           urjs_art_memory.c \
           urjs_art_os_file.c

# jerryscript src art ---end





# jerryscript src platform --begin

JAVASCRIPT_SRC_PLATFORM_DIR = $(JAVASCRIPT_SRC_DIR)/platform
JAVASCRIPT_SRC_PLATFORM_MOCOR_DIR = $(JAVASCRIPT_SRC_PLATFORM_DIR)/mocor


# jerryscript src platform jerry ---begin

JAVASCRIPT_ENGINEER_IMPLEMENT_DIR = $(JAVASCRIPT_SRC_PLATFORM_MOCOR_DIR)/jerry
JAVASCRIPT_ENGINEER_IMPLEMENT_SRC_PATH = $(JAVASCRIPT_ENGINEER_IMPLEMENT_DIR)/src
JAVASCRIPT_ENGINEER_IMPLEMENT_INC_PATH = $(JAVASCRIPT_ENGINEER_IMPLEMENT_DIR)/inc
JAVASCRIPT_ENGINEER_IMPLEMENT_SOURCES = jerry_module.c \
                                        jerry_port.c \
                                        jerry_string.c \
                                        jerry_util.c

MINCPATH += $(JAVASCRIPT_ENGINEER_IMPLEMENT_INC_PATH)
MSRCPATH += $(JAVASCRIPT_ENGINEER_IMPLEMENT_SRC_PATH)
SOURCES += $(JAVASCRIPT_ENGINEER_IMPLEMENT_SOURCES)

# jerryscript src platform jerry ---end


# jerryscript src platform mocor gui ---begin

JAVASCRIPT_SRC_PLATFORM_MOCOR_GUI_DIR = $(JAVASCRIPT_SRC_PLATFORM_MOCOR_DIR)/native_gui
MINCPATH += $(JAVASCRIPT_SRC_PLATFORM_MOCOR_GUI_DIR)/inc
MINCPATH += MS_MMI_Main/source/mmi_app/kernel/h \
            MS_MMI_Main/source/mmi_kernel/include \
            CAF/Template/include \
            MS_MMI_Main/source/mmi_utility/h \
            MS_MMI_Main/source/mmi_app/common/h \
            MS_MMI_Main/source/mmi_ctrl/include \
            MS_MMI_Main/source/mmi_gui/include \
            MS_MMI_Main/source/resource \
            MS_MMI_Main/source/mmi_ctrl/source/basecontrol/h \
            MS_MMI_Main/source/mmi_ctrl/source/TypeManager/h \
            MS_MMI_Main/source/mmi_ctrl/source/Form/h \
            MS_MMI_Main/source/mmi_ctrl/source/Progressbar/h

MSRCPATH += $(JAVASCRIPT_SRC_PLATFORM_MOCOR_GUI_DIR)/src
SOURCES += button.c \
           component.c \
           dashboard.c \
           id.c \
           image.c \
           form.c \
           slider.c \
           page.c

# jerryscript src platform mocor gui ---end


# jerryscript src platform mocor watchface ---begin

JAVASCRIPT_SRC_PLATFORM_MOCOR_WATCHFACE_DIR = $(JAVASCRIPT_SRC_PLATFORM_MOCOR_DIR)/native_watchface
MINCPATH += $(JAVASCRIPT_SRC_PLATFORM_MOCOR_WATCHFACE_DIR)/inc
MINCPATH += MS_MMI_Main/source/mmi_app/common/ctrl_adapter \
            MS_MMI_Main/source/mmi_service/export/inc
MSRCPATH += $(JAVASCRIPT_SRC_PLATFORM_MOCOR_WATCHFACE_DIR)/src
SOURCES += watchface_image.c \
           watchface_background.c \
           watchface_analogtime.c


# jerryscript src platform mocor watchface ---end

# jerryscript src platform mocor timer ---begin

JAVASCRIPT_SRC_PLATFORM_MOCOR_TIMER_DIR = $(JAVASCRIPT_SRC_PLATFORM_MOCOR_DIR)/native_timer
MINCPATH += $(JAVASCRIPT_SRC_PLATFORM_MOCOR_TIMER_DIR)/inc
MSRCPATH += $(JAVASCRIPT_SRC_PLATFORM_MOCOR_TIMER_DIR)/src
SOURCES += timer.c

# jerryscript src platform mocor timer ---end

# jerryscript src platform mocor resourceload ---begin

JAVASCRIPT_SRC_PLATFORM_MOCOR_RESOURCELOAD_DIR = $(JAVASCRIPT_SRC_PLATFORM_MOCOR_DIR)/native_resourceload
MINCPATH += $(JAVASCRIPT_SRC_PLATFORM_MOCOR_RESOURCELOAD_DIR)/inc
MSRCPATH += $(JAVASCRIPT_SRC_PLATFORM_MOCOR_RESOURCELOAD_DIR)/src
SOURCES += resourceload.c

# jerryscript src platform mocor resourceload ---end


# jerryscript src platform mocor log ---begin
JAVASCRIPT_SRC_PLATFORM_MOCOR_LOG_DIR = $(JAVASCRIPT_SRC_PLATFORM_MOCOR_DIR)/native_console
MINCPATH += $(JAVASCRIPT_SRC_PLATFORM_MOCOR_LOG_DIR)/inc
MSRCPATH += $(JAVASCRIPT_SRC_PLATFORM_MOCOR_LOG_DIR)/src
SOURCES += console.c
# jerryscript src platform mocor log ---end


# jerryscript src platform --end



# jerryscript src pm --begin

JAVASCRIPT_SRC_PM_DIR = $(JAVASCRIPT_SRC_DIR)/packagemanager
MINCPATH += $(JAVASCRIPT_SRC_PM_DIR)/inc
MINCPATH += $(JAVASCRIPT_SRC_PM_DIR)/src/adapter/inc
MINCPATH += MS_MMI_Main/source/mmi_app/common/cjson

MSRCPATH += $(JAVASCRIPT_SRC_PM_DIR)/src
MSRCPATH += $(JAVASCRIPT_SRC_PM_DIR)/src/adapter/src

#support unzip by minizip
MINCPATH += $(JAVASCRIPT_DEPS_DIR)/minizip

# packagemanager
SOURCES += packagemanager.c \
           packagemanager_test.c \
           pm_internal.c \
           pm_utils.c \
           pm_data_cache.c

# packagemanager adapter
SOURCES += pm_adapter_zip.c \
           pm_adapter_sfs.c

# jerryscript src pm --end






# jerryscript engine --begin
MINCPATH += $(JAVASCRIPT_ENGINE_DIR)/inc
MSRCPATH += $(JAVASCRIPT_ENGINE_DIR)/src
SOURCES += script_controller.c
# jerryscript engine --end

# jerryscript test --begin
JAVASCRPIT_TEST_NATIVE_DIR = $(JAVASCRIPT_TEST_DIR)/native



# jerryscript test --end




# jerryscript
#----------------------------------------------------------------------------#
#                                jerryscript                                 #
#----------------------------------------------------------------------------#

MINCPATH += $(JERRYSCRIPT_DIR)/jerry-core/ecma/base $(JERRYSCRIPT_DIR)/jerry-core/parser/js $(JERRYSCRIPT_DIR)/jerry-core/jrt $(JERRYSCRIPT_DIR)/jerry-core $(JERRYSCRIPT_DIR)/jerry-core/include \
           $(JERRYSCRIPT_DIR)/jerry-core/lit $(JERRYSCRIPT_DIR)/jerry-core/jmem $(JERRYSCRIPT_DIR)/jerry-core/ecma/operations $(JERRYSCRIPT_DIR)/jerry-core/vm $(JERRYSCRIPT_DIR)/jerry-core/debugger \
           $(JERRYSCRIPT_DIR)/jerry-ext $(JERRYSCRIPT_DIR)/jerry-libm $(JERRYSCRIPT_DIR)/jerry-main $(JERRYSCRIPT_DIR)/jerry-port $(JERRYSCRIPT_DIR)/jerry-core/jcontext $(JERRYSCRIPT_DIR)/jerry-core/parser/regexp \
		   $(JERRYSCRIPT_DIR)/jerry-core/ecma/builtin-objects $(JERRYSCRIPT_DIR)/jerry-core/ecma/builtin-objects/typedarray jerry-core/ecma/builtin-objects $(JERRYSCRIPT_DIR)/jerry-port/default/include \
		   $(JERRYSCRIPT_DIR)/jerry-ext/arg $(JERRYSCRIPT_DIR)/jerry-ext/common $(JERRYSCRIPT_DIR)/jerry-ext/debugger $(JERRYSCRIPT_DIR)/jerry-ext/handle-scope $(JERRYSCRIPT_DIR)/jerry-ext/handler $(JERRYSCRIPT_DIR)/jerry-ext/include $(JERRYSCRIPT_DIR)/jerry-ext/module

#include source file
MSRCPATH  += $(JERRYSCRIPT_DIR)/jerry-core/vm $(JERRYSCRIPT_DIR)/jerry-debugger $(JERRYSCRIPT_DIR)/jerry-ext $(JERRYSCRIPT_DIR)/jerry-libm $(JERRYSCRIPT_DIR)/jerry-main $(JERRYSCRIPT_DIR)/jerry-port  $(JERRYSCRIPT_DIR)/jerry-core/jcontext $(JERRYSCRIPT_DIR)/jerry-core/parser/regexp \
     $(JERRYSCRIPT_DIR)/jerry-core/api $(JERRYSCRIPT_DIR)/jerry-core/debugger $(JERRYSCRIPT_DIR)/jerry-core/ecma/base $(JERRYSCRIPT_DIR)/jerry-core/ecma/builtin-objects $(JERRYSCRIPT_DIR)/jerry-core/ecma/builtin-objects/typedarray \
     $(JERRYSCRIPT_DIR)/jerry-core/ecma/operations $(JERRYSCRIPT_DIR)/jerry-core/jcontext $(JERRYSCRIPT_DIR)/jerry-core/jmem $(JERRYSCRIPT_DIR)/jerry-core/jrt $(JERRYSCRIPT_DIR)/jerry-core/lit $(JERRYSCRIPT_DIR)/jerry-core/parser/js $(JERRYSCRIPT_DIR)/jerry-core/parser/regexp \
	 $(JERRYSCRIPT_DIR)/jerry-port/default \
	 $(JERRYSCRIPT_DIR)/jerry-ext/arg $(JERRYSCRIPT_DIR)/jerry-ext/common $(JERRYSCRIPT_DIR)/jerry-ext/debugger $(JERRYSCRIPT_DIR)/jerry-ext/handle-scope $(JERRYSCRIPT_DIR)/jerry-ext/handler $(JERRYSCRIPT_DIR)/jerry-ext/include $(JERRYSCRIPT_DIR)/jerry-ext/module


MCFLAG_OPT += -DJERRY_EXTERNAL_CONTEXT=1 -DJERRY_LINE_INFO=1 -DJERRY_LOGGING=1 -DJERRY_ERROR_MESSAGES=1



SOURCES += opcodes.c opcodes-ecma-arithmetics.c opcodes-ecma-bitwise.c opcodes-ecma-relational-equality.c vm.c vm-stack.c vm-utils.c jerry.c jerry-debugger.c jerry-debugger-transport.c jerry-snapshot.c \
          debugger.c ecma-alloc.c ecma-gc.c ecma-helpers.c ecma-helpers-collection.c ecma-helpers-conversion.c ecma-helpers-errol.c ecma-helpers-external-pointers.c ecma-helpers-number.c \
		  ecma-helpers-string.c ecma-helpers-value.c ecma-init-finalize.c ecma-lcache.c ecma-literal-storage.c ecma-module.c ecma-property-hashmap.c \
		  ecma-builtin-float32array.c ecma-builtin-float32array-prototype.c ecma-builtin-float64array.c ecma-builtin-float64array-prototype.c ecma-builtin-int8array.c ecma-builtin-int8array-prototype.c \
		  ecma-builtin-int16array.c ecma-builtin-int16array-prototype.c ecma-builtin-int32array.c ecma-builtin-int32array-prototype.c ecma-builtin-typedarray.c ecma-builtin-typedarray-helpers.c \
		  ecma-builtin-typedarray-prototype.c ecma-builtin-uint8array.c ecma-builtin-uint8array-prototype.c ecma-builtin-uint8clampedarray.c ecma-builtin-uint8clampedarray-prototype.c ecma-builtin-uint16array.c \
		  ecma-builtin-uint16array-prototype.c ecma-builtin-uint32array.c ecma-builtin-uint32array-prototype.c ecma-builtin-array.c ecma-builtin-arraybuffer.c ecma-builtin-arraybuffer-prototype.c ecma-builtin-array-iterator-prototype.c \
		  ecma-builtin-array-prototype.c ecma-builtin-boolean.c ecma-builtin-boolean-prototype.c ecma-builtin-dataview.c ecma-builtin-dataview-prototype.c ecma-builtin-date.c ecma-builtin-date-prototype.c ecma-builtin-error.c

SOURCES += ecma-builtin-evalerror.c ecma-builtin-evalerror-prototype.c ecma-builtin-function.c ecma-builtin-function-prototype.c ecma-builtin-generator-prototype.c ecma-builtin-global.c ecma-builtin-helpers.c ecma-builtin-helpers-date.c \
		  ecma-builtin-helpers-error.c ecma-builtin-helpers-json.c ecma-builtin-helpers-sort.c ecma-builtin-iterator-prototype.c ecma-builtin-json.c ecma-builtin-map.c ecma-builtin-map-iterator-prototype.c \
		  ecma-builtin-map-prototype.c ecma-builtin-map.c ecma-builtin-math.c ecma-builtin-number-prototype.c ecma-builtin-number.c ecma-builtin-object-prototype.c ecma-builtin-error-prototype.c\
		  ecma-builtin-object.c ecma-builtin-promise-prototype.c ecma-builtin-promise.c ecma-builtin-rangeerror-prototype.c ecma-builtin-rangeerror.c ecma-builtin-referenceerror-prototype.c \
		  ecma-builtin-referenceerror.c ecma-builtin-reflect.c ecma-builtin-regexp-prototype.c ecma-builtin-regexp.c ecma-builtin-set-iterator-prototype.c ecma-builtin-set-prototype.c ecma-builtin-set.c \
		  ecma-builtin-string-iterator-prototype.c ecma-builtin-string-prototype.c ecma-builtin-string.c ecma-builtin-symbol-prototype.c ecma-builtin-symbol.c ecma-builtin-syntaxerror-prototype.c \
		  ecma-builtin-syntaxerror.c ecma-builtin-type-error-thrower.c ecma-builtin-typeerror-prototype.c ecma-builtin-typeerror.c ecma-builtin-urierror-prototype.c ecma-builtin-urierror.c \
		  ecma-builtin-weakmap-prototype.c ecma-builtin-weakmap.c ecma-builtin-weakset-prototype.c ecma-builtin-weakset.c ecma-builtins.c ecma-array-object.c ecma-arraybuffer-object.c ecma-boolean-object.c \
		  ecma-comparison.c ecma-container-object.c ecma-conversion.c ecma-dataview-object.c ecma-eval.c ecma-exceptions.c ecma-function-object.c ecma-get-put-value.c ecma-iterator-object.c ecma-jobqueue.c ecma-lex-env.c \
		  ecma-number-arithmetic.c ecma-number-object.c ecma-objects-arguments.c  ecma-objects-general.c ecma-objects.c ecma-promise-object.c ecma-reference.c ecma-regexp-object.c ecma-string-object.c ecma-symbol-object.c \
		  ecma-typedarray-object.c


SOURCES += jcontext.c jmem-allocator.c jmem-heap.c jmem-poolman.c jrt-fatals.c lit-char-helpers.c lit-magic-strings.c lit-strings.c byte-code.c common.c js-lexer.c js-parser-expr.c \
		  js-parser-mem.c js-parser-module.c js-parser-statm.c js-parser-util.c js-parser.c js-scanner-util.c js-scanner.c re-bytecode.c re-compiler.c re-parser.c 


#SOURCES +=		  default-date.c default-debugger.c default-external-context.c default-fatal.c default-io.c default-module.c

SOURCES += arg-js-iterator-helper.c arg-transform-functions.c arg.c debugger-common.c debugger-rp.c debugger-serial.c debugger-sha1.c debugger-tcp.c debugger-ws.c \
		  handle-scope-allocator.c handle-scope.c handler-assert.c handler-gc.c handler-print.c handler-register.c handler-resource-name.c module.c
		  
		  
# MINIZIP api ---begin		  
MINCPATH      += $(JAVASCRIPT_DIR)/deps/minizip
MINCPATH      += $(JAVASCRIPT_DIR)/deps/zlib
MSRCPATH      += $(JAVASCRIPT_DIR)/deps/minizip
SOURCES       += ioapi.c \
                miniunzip.c \
                unzip.c \
                fs_api.c

# MINIZIP api ---END	  		  

# zilib api ---begin	
MINCPATH      += $(JAVASCRIPT_DIR)/deps/zlib
#mocor support
MINCPATH += RTOS/export/inc/posix
MSRCPATH      += $(JAVASCRIPT_DIR)/deps/zlib
SOURCES       += adler32.c \
                crc32.c \
                inffast.c \
                inflate.c \
                inftrees.c \
                zutil.c
# zilib api ---end