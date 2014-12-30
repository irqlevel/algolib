#include <include/crt.h>
#include <include/char.h>

char char_to_hex(char c)
{
        if (c < 10)
                return '0' + c;
        else
                return 'a' + c - 10;
}

int bytes_buf_hex_buf(char *src, int src_count, char *hex, int hex_count)
{
        int i;

        if (hex_count < 2*src_count)
                return -1;

        for (i = 0; i < src_count; i++) {
                *hex++ = char_to_hex((*src >> 4) & 0xF);
                *hex++ = char_to_hex(*src++ & 0xF);
        }
        return 0;
}

char *bytes_hex(char *src, int src_count)
{
        char *hex;
        int hex_count;

        if (!src || src_count <= 0)
                return NULL;

        hex_count = 2*src_count + 1;
        hex = al_malloc(hex_count);
        if (!hex)
                return NULL;

        if (bytes_buf_hex_buf(src, src_count, hex, hex_count)) {
                al_free(hex);
                return NULL;
        }

        hex[2*src_count] = '\0';
        return hex;
}

