#ifndef MRED_INPUT_H
#define MRED_INPUT_H

#define CTRL_KEY(k) ((k) & 0x1f)

enum mred_key
{
        BACKSPACE = 127,
        ARROW_LEFT = 1000,
        ARROW_RIGHT,
        ARROW_UP,
        ARROW_DOWN,
        DEL_KEY,
        HOME_KEY,
        END_KEY,
        PAGE_UP,
        PAGE_DOWN
};

void mred_process_keypress ();
void mred_move_cursor (int key);
char * mred_prompt (char *prompt, void (*callback)(char *, int));

#endif /* !MRED_INPUT_H */
