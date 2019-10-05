#include <stdio.h>

const unsigned char LEAD_MASK = 0xff; // 11111111
const unsigned char CONT_MASK = 0x3f; // 00111111
const unsigned UTF8_TEMPLATES[4] = {
    [0] = 0x00,      // 0xxxxxxx
    [1] = 0xc080,    // 110xxxxx 10xxxxxx
    [2] = 0xe08080,  // 1110xxxx 10xxxxxx 10xxxxxx
    [3] = 0xf0808080 // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
};

/* Get the integer UTF-8 encoding of a Unicode code point */
unsigned utf8_encode(unsigned point) {
    int offset;         // Byte offset
    unsigned encoded;   // Encoded integer
    int i;

    if (point < 0x80)
        offset = 0;
    else if (point < 0x800)
        offset = 1;
    else if (point < 0x10000)
        offset = 2;
    else if (point < 0x110000)
        offset = 3;
    else
        return 0;

    // Encode the leading byte
    encoded = UTF8_TEMPLATES[offset] + ((point << (2 * offset)) & (LEAD_MASK << (offset * 8)));

    // Encode the continuation bytes
    for (i = 0; i < offset; ++i) {
        encoded += (point << (2 * i)) & (CONT_MASK << (8 * i));
    }

    return encoded;
}

/* Convert a UTF-8 encode integer to a string */
char *utf8_string(unsigned encoded) {
    static char s[5];
    int i;
    int written = 0;
    char byte;

    for (i = 3; i >= 0; --i) {
        byte = (encoded >> (i * 8)) & 0xff;
        if (byte)
            s[written++] = byte;
    }

    s[written] = '\0';

    return s;
}

int main(int argc, char *argv[]) {
    int point;
    int i;

    if (argc == 1)
        return 1;

    for (i = 1; i < argc; ++i) {
        sscanf(argv[i], "%x", &point);
        printf("%s", utf8_string(utf8_encode(point)));
    }

    printf("\n");

    return 0;
}
