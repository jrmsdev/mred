#ifndef __MRED_FILETYPES_H
#define __MRED_FILETYPES_H

#define HL_DO_NUMBERS (1<<0)
#define HL_DO_STRINGS (1<<1)

char *C_HL_ext[] = {".c", ".h", ".cpp", NULL};
char *C_HL_kws[] = {
  "switch", "if", "while", "for", "break", "continue", "return", "else",
  "struct", "union", "typedef", "static", "enum", "class", "case", "default",
  "#define", "#ifdef", "#ifndef", "#endif", "#include",

  "int|", "long|", "double|", "float|", "char|", "unsigned|", "signed|",
  "void|", "NULL|", "size_t|", "ssize_t|", "time_t|", NULL
};

struct mred_syntax HLDB[] = {
	{
		"C",
		C_HL_ext,
		C_HL_kws,
		"//",
		"/*",
		"*/",
		HL_DO_NUMBERS | HL_DO_STRINGS
	},
};

#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

#endif /* !__MRED_FILETYPES_H */
