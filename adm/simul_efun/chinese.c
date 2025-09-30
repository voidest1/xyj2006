
string chinese_number(int i)
{
	return CHINESE_D->chinese_number(i);
}

string to_chinese(string str)
{
	return CHINESE_D->chinese(str);
}

int _is_cjk_ok(int cp) {
    /* 表意字符 */
    if ( (cp >= 0x4E00 && cp <= 0x9FFF)
      || (cp >= 0x3400 && cp <= 0x4DBF)
      || (cp >= 0xF900 && cp <= 0xFAFF)
      || (cp >= 0x20000 && cp <= 0x323AF) )   /* 扩展区 */
        return 1;

    /* CJK 标点、全角空格等 */
    if ( (cp >= 0x3000 && cp <= 0x303F)        /* CJK Symbols and Punctuation（含全角空格U+3000） */
      || (cp >= 0xFF00 && cp <= 0xFF65) )      /* 全角ASCII/半角片假名等，按需放宽 */
        return 1;

    return 0;
}

int is_chinese(string s) {
    int i, n, b0, b1, b2, b3, cp, len, seen;

    if (!stringp(s)) return 0;
    n = strlen(s);
    if (n <= 0) return 0;

    i = 0;
    seen = 0;
    while (i < n) {
        b0 = s[i] & 0xFF;
        if (b0 < 0x80) {                       /* ASCII：宽松版也不接受（若想接受空格，单独放开 0x20） */
            return 0;
        } else if (b0 < 0xE0) {                /* 2 字节 */
            if (i + 1 >= n) return 0;
            b1 = s[i+1] & 0xFF;
            if ((b1 & 0xC0) != 0x80) return 0;
            cp = ((b0 & 0x1F) << 6) | (b1 & 0x3F);
            len = 2;
        } else if (b0 < 0xF0) {                /* 3 字节 */
            if (i + 2 >= n) return 0;
            b1 = s[i+1] & 0xFF; b2 = s[i+2] & 0xFF;
            if ((b1 & 0xC0) != 0x80 || (b2 & 0xC0) != 0x80) return 0;
            cp = ((b0 & 0x0F) << 12) | ((b1 & 0x3F) << 6) | (b2 & 0x3F);
            len = 3;
        } else if (b0 < 0xF8) {                /* 4 字节 */
            if (i + 3 >= n) return 0;
            b1 = s[i+1] & 0xFF; b2 = s[i+2] & 0xFF; b3 = s[i+3] & 0xFF;
            if ((b1 & 0xC0) != 0x80 || (b2 & 0xC0) != 0x80 || (b3 & 0xC0) != 0x80) return 0;
            cp = ((b0 & 0x07) << 18) | ((b1 & 0x3F) << 12) | ((b2 & 0x3F) << 6) | (b3 & 0x3F);
            len = 4;
        } else {
            return 0;
        }

        if (!_is_cjk_ok(cp)) return 0;
        seen = 1;
        i += len;
    }
    return seen ? 1 : 0;
}