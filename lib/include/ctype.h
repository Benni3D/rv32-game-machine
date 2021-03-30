#ifndef FILE_CTYPE_H
#define FILE_CTYPE_H

#define isinrange(min, max) (ch >= (min) && ch <= (max))

inline static int iscntrl(int ch) {
   return isinrange(0x00, 0x1f) || (ch == 0x7f);
}
inline static int isprint(int ch) {
   return isinrange(0x20, 0x7e);
}
inline static int isgraph(int ch) {
   return isinrange(0x21, 0x7e);
}
inline static int isspace(int ch) {
   return isinrange(0x09, 0x0d) || (ch == 0x20);
}
inline static int isblank(int ch) {
   return ch == 0x09 || ch == 0x20;
}
inline static int ispunct(int ch) {
   return isinrange(0x21, 0x2f) || isinrange(0x3a, 0x40) || isinrange(0x5b, 0x60) || isinrange(0x7b, 0x7e);
}

inline static int isdigit(int ch) {
   return isinrange(0x30, 0x39);
}
inline static int isxdigit(int ch) {
   return isdigit(ch) || isinrange(0x41, 0x46) || isinrange(0x61, 0x66);
}

inline static int isupper(int ch) {
   return isinrange(0x41, 0x5a);
}
inline static int islower(int ch) {
   return isinrange(0x61, 0x7a);
}
inline static int isalpha(int ch) {
   return isupper(ch) || islower(ch);
}
inline static int isalnum(int ch) {
   return isalpha(ch) || isdigit(ch);
}

inline static int toupper(int ch) {
   return islower(ch) ? ch - 0x20 : ch;
}
inline static int tolower(int ch) {
   return isupper(ch) ? ch + 0x20 : ch;
}


#undef isinrange

#endif /* FILE_CTYPE_H */
