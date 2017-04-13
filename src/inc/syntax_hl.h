#ifndef MRED_SYNTAX_HL_H
#define MRED_SYNTAX_HL_H

#include "mred.h"

struct mred_syntax {
	char *filetype;
	char **filematch;
	char **keywords;
	char *singleline_comment_start;
	char *multiline_comment_start;
	char *multiline_comment_end;
	int flags;
};

enum mred_highlight {
	HL_NORMAL = 0,
	HL_COMMENT,
	HL_MLCOMMENT,
	HL_KEYWORD1,
	HL_KEYWORD2,
	HL_STRING,
	HL_NUMBER,
	HL_MATCH
};

void mred_update_syntax (edrow *row);
int mred_syntax_to_color (int hl);
void mred_select_syntax_hl ();

#endif /* !MRED_SYNTAX_HL_H */
