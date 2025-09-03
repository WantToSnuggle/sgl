#ifndef __LM_CMD_H__
#define __LM_CMD_H__

#include <stdint.h>
#include "lm_log.h"


int lm_rm(const char *dir_name);
int lm_mkdir(const char *dir_name);
int lm_copy_file(const char *source_path, const char *dest_path);
void lm_echo(const char *info);
void lm_echo_red(const char *info);
void lm_echo_green(const char *info);
void lm_echo_blue(const char *info);


#endif //!__LM_CMD_H__