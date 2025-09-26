#ifndef __LM_GEN_H__
#define __LM_GEN_H__

int lm_gen_header_file(const char* file_path);
int lm_gen_lmmk_file(const char* file_path);
int lm_gen_projcfg_file(const char* file_path);
int lm_gen_mkfile_file(const char *makefile, const char *lmmk, const char *lmcfg, const char *projcfg, 
                       const char *header_file, const char *pro_name, const char *build_dir, const char *gcc_prefix);



#endif // !__LM_GEN_H__