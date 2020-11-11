#include "stdio.h"

int main()
{
    FILE* in;
    FILE* out;
    in = fopen("Alphabet", "r");
    out = fopen("Alphabet_parsed", "w");
    char ch = 0;
    char is_newline = 0;
    int cur_letter = 0;
    while (1)
    {
        ch = fgetc(in);
        switch (ch) {
            case EOF: goto exit;
            case '\n':
                if (is_newline)
                {
                    fputc(32, out);
                    fputc('/', out);
                    fputc('/', out);
                    fputc(32, out);
                    if (cur_letter > 9)
                        fputc(cur_letter + 55, out);
                    else
                        fputc(cur_letter + 48, out);
                    fputc(ch, out);
                    is_newline = 0;
                    cur_letter++;
                } else is_newline = 1;
                break;
            default:
                fputc(ch, out);
                fputc(',', out);
                is_newline = 0;
        }
    }
exit:
    fclose(in);
    return 0;
}
