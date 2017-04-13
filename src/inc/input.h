#ifndef MRED_INPUT_H
#define MRED_INPUT_H

#define CTRL_KEY(k) ((k) & 0x1f)

void mred_process_keypress ();
void mred_move_cursor (int key);
char * mred_prompt (char *prompt, void (*callback)(char *, int));

#endif /* !MRED_INPUT_H */
