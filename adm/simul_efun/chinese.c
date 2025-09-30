
string chinese_number(int i)
{
	return CHINESE_D->chinese_number(i);
}

string to_chinese(string str)
{
	return CHINESE_D->chinese(str);
}

int is_chinese(string str)
{
int n = strlen(s);
    if (!stringp(s) || n == 0) return 0;

    int b0 = s[0] & 0xFF;

    // 3-byte: U+4E00..U+9FFF（基本汉字）、U+3400..U+4DBF（扩展A）、U+3000..U+303F（标点）
    if (b0 >= 0xE0 && b0 <= 0xEF && n >= 3) {
        int b1 = s[1] & 0xFF, b2 = s[2] & 0xFF;
        if ((b1 & 0xC0) == 0x80 && (b2 & 0xC0) == 0x80) {
            int cp = ((b0 & 0x0F) << 12) | ((b1 & 0x3F) << 6) | (b2 & 0x3F);
            return (cp >= 0x4E00 && cp <= 0x9FFF)   // CJK Unified Ideographs
                || (cp >= 0x3400 && cp <= 0x4DBF)   // CJK Ext-A
                || (cp >= 0x3000 && cp <= 0x303F)   // CJK 标点
                || (cp >= 0xF900 && cp <= 0xFAFF);  // 兼容表意
        }
    }

    // 4-byte: 扩展B~K等（U+20000 以上）
    if (b0 >= 0xF0 && b0 <= 0xF4 && n >= 4) {
        int b1 = s[1] & 0xFF, b2 = s[2] & 0xFF, b3 = s[3] & 0xFF;
        if ((b1 & 0xC0) == 0x80 && (b2 & 0xC0) == 0x80 && (b3 & 0xC0) == 0x80) {
            int cp = ((b0 & 0x07) << 18) | ((b1 & 0x3F) << 12) | ((b2 & 0x3F) << 6) | (b3 & 0x3F);
            return (cp >= 0x20000 && cp <= 0x2FA1F); // CJK Ext. B~K/兼容扩展（常用范围）
        }
    }

    return 0;
}
