#ifndef __MRED_FILETYPES_H
#define __MRED_FILETYPES_H

#define HL_DO_NUMBERS (1<<0)

char *C_HL_extensions[] = {".c", ".h", ".cpp", NULL};

struct mred_syntax HLDB[] = {
	{"C", C_HL_extensions, HL_DO_NUMBERS},
};

#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

#endif /* !__MRED_FILETYPES_H */
