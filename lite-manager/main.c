/* source/main.c
 *
 * MIT License
 *
 * Copyright(c) 2023-present @ li shanwen
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include "lm_log.h"
#include "lm_error.h"
#include "lm_parser.h"
#include "lm_string.h"
#include "lm_mem.h"
#include <getopt.h>
#include "config.h"
#include "lm_gen.h"
#include "lm_cmd.h"


#define    VERSION           "0.20250709"


static const char *makefile = NULL;
static const char *pro_name = "demo";
static const char *build_dir = "build";
static const char *projcfg = ".config";
static const char *lmcfg = "lm.cfg";
static const char *header_file = "config.h";
static const char *lmmk_file = ".lm.mk";
static const char *gcc_prefix="";
static int mem_size = CONFIG_MEM_POOL_SIZE;
static bool blind = false;


static void show_flag_usage(char *flag, char *example)
{
    printf("    \x1b[36mExample: %s += %s\x1b[0m \n", flag, example);
    printf("\n");
}

static void show_key_usage(void)
{
    printf("lite-manager key: [option] | [option]-$(macro_xxx) += [value] [value] ...\n");
    printf("\n[option]:\n");
    printf("    SRC:                   add c or c++ source files\n");
    printf("    SRC-$(CONFIG_XXX):     add c or c++ source files dependent on CONFIG_XXX\n");
    show_flag_usage("SRC", "main.c xxx.c  test/*.c");

    printf("    PATH:                  add c or c++ include path\n");
    printf("    PATH-$(CONFIG_XXX):    add c or c++ include path dependent on CONFIG_XXX\n");
    show_flag_usage("PATH", "include test/include");

    printf("    DEFINE:                add c or c++ global macro define\n");
    printf("    DEFINE-$(CONFIG_XXX):  add c or c++ global macro define dependent on CONFIG_XXX\n");
    show_flag_usage("DEFINE", "STM32F10X_HD");

    printf("    ASM:                   add asm source files\n");
    printf("    ASM-$(CONFIG_XXX):     add asm source files dependent on CONFIG_XXX\n");
    show_flag_usage("ASM", "startup.S  [Note: Must end with an uppercase .S letter]");

    printf("    LDS:                   add build link script file\n");
    printf("    LDS-$(CONFIG_XXX):     add build link script file dependent on CONFIG_XXX\n");
    show_flag_usage("LDS", "memory.lds");

    printf("    ASFLAG:                add asm build flag\n");
    printf("    ASFLAG-$(CONFIG_XXX):  add asm build flag dependent on CONFIG_XXX\n");
    show_flag_usage("ASFLAG", "-mcpu=cortex-m3 -mthumb -g -gdwarf-2 -Wall");

    printf("    MCFLAG:                add machine build flag\n");
    printf("    MCFLAG-$(CONFIG_XXX):  add machine build flag dependent on CONFIG_XXX\n");
    show_flag_usage("MCFLAG", "-mcpu=cortex-m3 -mthumb");

    printf("    CFLAG:                 add c build flag\n");
    printf("    CFLAG-$(CONFIG_XXX):   add c build flag dependent on CONFIG_XXX\n");
    show_flag_usage("CFLAG", "-mcpu=cortex-m3 -mthumb -g -gdwarf-2 -Wall");

    printf("    CPPFLAG:               add c++ build flag\n");
    printf("    CPPFLAG-$(CONFIG_XXX): add c++ build flag dependent on CONFIG_XXX\n");
    show_flag_usage("CPPFLAG", "-mcpu=cortex-m3 -mthumb -g -gdwarf-2 -Wall");

    printf("    LDFLAG:                add link flag\n");
    printf("    LDFLAG-$(CONFIG_XXX):  add link flag dependent on CONFIG_XXX\n");
    show_flag_usage("LDFLAG", "-lsdl -lrt -lc");

    printf("    LIB:                   add library\n");
    printf("    LIB-$(CONFIG_XXX):     add library dependent on CONFIG_XXX\n");
    show_flag_usage("LIB", "sdl rt lc");

    printf("    LIBPATH:               add library path\n");
    printf("    LIBPATH-$(CONFIG_XXX): add library path dependent on CONFIG_XXX\n");
    show_flag_usage("LIBPATH", "path/to/lib/path");

    printf("\n");
    printf("    include:               include sub lm.cfg\n");
    printf("    include-$(CONFIG_XXX): include sub lm.cfg dependent on CONFIG_XXX\n");
}


static void show_help(char *app_name)
{
    printf("Usage: %s [options] ... [options]\n", app_name);
    printf("[options:]\n");
    printf("    --help                                Display this help message\n");
    printf("    --flag                                Display this help detail message\n");
    printf("    --version                             Display the version of the lite-manager\n");
    printf("    --lmcfg                               Input lite manager file(toplayer lm.cfg), default: %s\n", lmcfg);
    printf("    --projcfg                             Input using project config file, default: %s\n", projcfg);
    printf("    --out                                 Output config header file, default: %s\n", header_file);
    printf("    --mk                                  Output config makefile file, default: %s\n", lmmk_file);
    printf("    --mem                                 Memory size used by lm, default: %dMB\n", mem_size);
    printf("    --blind                               Hide information about configuration macros\n");
    printf("\n");
    printf("    --gen                                 Generate Makefile: by toplayer lm.cfg, defaule: Makefile\n");
    printf("    --project                             Generate Makefile: project name, default: demo\n");
    printf("    --build                               Generate Makefile: build directory, default: build\n");
    printf("    --prefix                              Generate Makefile: cross compiler prefix\n");
    printf("\n");
    printf("    --rm                                  Delete directory or file\n");
    printf("    --cp                                  Copy file\n");
}


static struct option cmd_long_options[] =
{  
    {"help",      no_argument,             NULL, 'a'},
    {"flag",      no_argument,             NULL, 'b'},
    {"version",   no_argument,             NULL, 'c'},
    {"lmcfg",     required_argument,       NULL, 'd'},
    {"projcfg",   required_argument,       NULL, 'e'},
    {"out",       required_argument,       NULL, 'f'},
    {"mk",        required_argument,       NULL, 'g'},
    {"mem",       required_argument,       NULL, 'h'},
    {"blind",     no_argument,             NULL, 'i'},

    {"gen",       required_argument,       NULL, 'j'},
    {"project",   required_argument,       NULL, 'k'},
    {"build",     required_argument,       NULL, 'l'},
    {"prefix",    required_argument,       NULL, 'm'},

    {"rm",        required_argument,       NULL, 'n'},
    {"cp",        required_argument,       NULL, 'o'},
    {NULL,        0,                       NULL,  0},
};


static const char *shortopts = "abcd:e:f:g:h:i:j:k:l:m:n:o";


int main(int argc, char *argv[])
{
    int ret;
    int opt;

    if(argc == 1) {
        show_help(argv[0]);
        exit(0);
    }

    while ((opt = getopt_long (argc, argv, shortopts, cmd_long_options, NULL)) != -1) {
        switch (opt) {
            case 'a':
                show_help("lm.exe");
                exit(0);
                break;
            case 'b':
                show_key_usage();
                exit(0);
                break;
            case 'c':
                printf("lite-manager version "VERSION"\n");
                printf("Copyright (C) 2024-2030 li-shanwen(1477153217@qq.com), License MIT\n");
                exit(0);
                break;
            case 'd':
                lmcfg = optarg;
                break;
            case 'e':
                projcfg = optarg;
                break;
            case 'f':
                header_file = optarg;
                break;
            case 'g':
                lmmk_file = optarg;
                break;
            case 'h':
                mem_size = strtol(optarg, NULL, 10);
                break;
            case 'i':
                blind = true;
                break;
            case 'j':
                makefile = optarg;
                break;
            case 'k':
                pro_name = optarg;
                break;
            case 'l':
                build_dir = optarg;
                break;
            case 'm':
                gcc_prefix = optarg;
                break;
            case 'n':
                ret = lm_rm(optarg);
                exit(ret);
                break;
            case 'o':
                ret = lm_copy_file(optarg, argv[optind]);
                exit(ret);
                break;
            case '?':
                LM_LOG_ERROR("Unknown option: %c\n", optopt);
                exit(1);
                break;
            default:
                LM_LOG_ERROR("Unknown option: %c\n", optopt);
                exit(1);
                break;
        }
    }

    int pcode = -1;

#if (_WIN32)
    pcode = system("chcp.com 65001 >null");  //UTF-8
    if(pcode) {
        LM_LOG_INFO("failed to set page code");
    }

    pcode = system("del null"); // delete null file
    if(pcode) {
        LM_LOG_INFO("failed to del null file");
    }
#elif ( __linux__)
    pcode = system("export LANG=zh_CN.UTF-8");
    if(pcode) {
        LM_LOG_INFO("failed to set page code");
    }
#endif

    lm_mem_init(mem_size);

    lm_parser_init();

    if(!makefile) {
        ret = lm_parser_config_file(projcfg);
        if(ret == LM_ERR) {
            goto error;
        }
    }

    ret = lm_parser_lm_file(NULL, lmcfg);
    if(ret == LM_ERR) {
        goto error;
    }

    if(makefile) {
        lm_gen_mkfile_file(makefile, lmmk_file, lmcfg, projcfg, header_file, pro_name, build_dir, gcc_prefix);
        lm_gen_projcfg_file(projcfg);
        goto exit;
    }

    if(!makefile) {
        ret = lm_gen_lmmk_file(lmmk_file);
        if(ret == LM_ERR) {
            goto error;
        }

        ret = lm_gen_header_file(header_file);
        if(ret == LM_ERR) {
            goto error;
        }

        if(!blind) {
            lm_macro_print_all_value(lm_parser_get_macro_head());
        }
    }

exit:
    lm_mem_destroy();
    return 0;

error:
    lm_mem_destroy();
    LM_LOG_ERROR("parser failed, exiting");
    return -1;
}
