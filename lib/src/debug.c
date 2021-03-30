

void debug(char ch) {
   volatile char* ptr = (volatile char*)0x80000000;
   *ptr = ch;
}

