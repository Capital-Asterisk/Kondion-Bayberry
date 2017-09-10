#define KTT2_HEADER_PIXEL(data,pixel) {\
pixel[0] = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4)); \
pixel[1] = ((((data[1] - 33) & 0xF) << 4) | ((data[2] - 33) >> 2)); \
pixel[2] = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33))); \
data += 4; \
}

static char* ktt2_data =
    "'BI;'BI;``%!``%!'BI;'BI;``%!``%!'BI;'BI;``%!``%!'BI;'BI;``%!``%!"
    "'BI;'BI;``%!``%!'BI;'BI;``%!``%!'BI;'BI;``%!``%!'BI;'BI;``%!``%!"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0``%!"
    "``%!$Q]0'BI;(BY?%R-4$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0(BY?(BY?"
    "$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "``%!$Q]0(BY?'BI;$Q]0%R-4'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0%R-4'BI;'BI;"
    "%R-4$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "'BI;'BI;%R-4$Q]0'BI;(BY?$Q]0%R-4'BI;(BY?$Q]0%R-4'BI;(BY?$Q]0$Q]0"
    "(BY?'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0``%!"
    "'BI;'BI;%R-4$Q]0'BI;(BY?$Q]0%R-4'BI;(BY?$Q]0%R-4'BI;(BY?$Q]0$Q]0"
    "(BY?'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0``%!"
    "``%!$Q]0'BI;(BY?%R-4$Q]0'BI;'BI;%R-4$Q]0(BY?'BI;$Q]0$Q]0(BY?(BY?"
    "$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "``%!$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0``%!"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0``%!"
    "``%!$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "``%!$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "``%!``%!``%!``%!``%!$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0"
    "``%!``%!``%!``%!``%!'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0``%!"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;````````````````````````````"
    "``%!``%!``%!``%!``%!````````$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0``%!"
    "``%!$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;````$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "``%!``%!``%!``%!``%!$Q]0'BI;````$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "``%!$Q]0'BI;'BI;$Q]0$Q]0'BI;````$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "``%!``%!``%!``%!``%!$Q]0'BI;'BI;````$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0````'BI;'BI;$Q]0````````````````````"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0````'BI;$Q]0$Q]0'BI;'BI;$Q]0``%!"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0````'BI;$Q]0````````````````````"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0````'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0``%!"
    "``%!$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0````````````````````````````"
    "````````````````````````````'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "``%!$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;````````````````````"
    "$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0````````````````````"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0``%!"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0``%!"
    "``%!$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "``%!$Q]0'BI;'BI;$Q]0$Q]0````````````````````'BI;````````````````"
    "$Q]0````````````$Q]0````````````````````'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0````'BI;$Q]0$Q]0````'BI;$Q]0$Q]0"
    "'BI;````$Q]0$Q]0'BI;'BI;$Q]0````'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0``%!"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0````'BI;$Q]0$Q]0````````````$Q]0"
    "'BI;````````````'BI;'BI;$Q]0````'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0``%!"
    "``%!$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;````$Q]0'BI;'BI;````$Q]0'BI;'BI;"
    "$Q]0$Q]0'BI;````$Q]0$Q]0'BI;````$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "``%!$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;````$Q]0'BI;'BI;````````````````"
    "$Q]0````````````$Q]0$Q]0'BI;````$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0``%!"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0"
    "'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0``%!"
    "``%!$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;$Q]0$Q]0'BI;'BI;"
    "``%!``%!'BI;'BI;``%!``%!'BI;'BI;``%!``%!'BI;'BI;``%!``%!'BI;'BI;"
    "``%!``%!'BI;'BI;``%!``%!'BI;'BI;``%!``%!'BI;'BI;``%!``%!'BI;'BI;"
    "";

// Can be fed directly into gltex2d, but takes more space
static char* kondismol_data =
    "\x71\xe3\x3c\xfa\x2f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x8a\x24"
    "\x22\x22\x48\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x82\x24\x22\x22"
    "\x88\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xf9\xef\x3c\x23\xc8\x80"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x88\x24\x22\x22\x28\x80\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x71\xc4\x22\x22\x28\x80\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x04\x22\x22\x28\x80\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x04\x3c\xfb\xcf\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\xe1"
    "\xe3\x3c\x91\x8b\xbe\x22\x20\x03\x00\x00\x00\x00\x00\x00\x80\xa0\x14\x49"
    "\x5b\x54\x60\x42\x10\x05\x00\x00\x00\x00\x00\x00\x80\xa0\x14\x4a\x35\x20"
    "\x50\x42\x10\x09\x00\x00\x00\x00\x00\x00\xf3\xa0\xef\x4a\x31\x53\xc8\x82"
    "\x0a\x52\x00\x00\x00\x00\x00\x00\x8c\xb1\x04\x4a\x31\x8c\x44\x42\x11\xa9"
    "\x00\x00\x00\x00\x00\x00\xf3\xae\xf4\x4a\x31\x8c\x7e\x42\x10\x05\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x00\x42\x10\x03\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x04\x00\x00\x00\x00\x22\x20\x00\x00\x00\x00\x00\x00\x00"
    "\x03\xfc\xe7\xb9\x1e\x89\x19\x26\x56\x2e\x00\x00\x00\x00\x00\x00\x04\x63"
    "\x18\xc5\x01\x89\x25\x24\x56\x31\x00\x00\x00\x00\x00\x00\x04\x63\x08\xc1"
    "\x0f\x89\x05\x44\x56\x31\x00\x00\x00\x00\x00\x00\x03\xe3\x08\xfd\xd1\x89"
    "\x05\x84\x56\x31\x00\x00\x00\x00\x00\x00\x00\x63\x18\xc5\x11\x88\x01\x44"
    "\x56\x31\x00\x00\x00\x00\x00\x00\x03\xbc\xe7\xb9\x0e\xf1\x05\x24\x6b\xce"
    "\x00\x00\x00\x00\x00\x00\x10\x20\x00\x80\x80\x80\x01\x04\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x20\x20\x00\x80\x40\x80\x01\x04\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x83\xe3\xe2\x38\x91\x89\x3e\xe1\x38\x1f\x00\x00\x00\x00\x00\x00"
    "\x85\xa4\x12\x45\x5b\x89\x20\x81\x08\x00\x00\x00\x00\x00\x00\x00\x84\xa8"
    "\x12\x46\x35\x51\x10\x82\x08\x00\x00\x00\x00\x00\x00\x00\xf4\xbc\xe2\x46"
    "\x31\x21\x08\x82\x08\x00\x00\x00\x00\x00\x00\x00\x8c\xa3\x02\x46\x31\x52"
    "\x84\x84\x08\x00\x00\x00\x00\x00\x00\x00\x8c\xa3\x02\x46\x31\x8c\x42\x84"
    "\x0a\x20\x00\x00\x00\x00\x00\x00\x8c\xa3\x02\x46\x31\x8c\x42\x88\x09\x40"
    "\x00\x00\x00\x00\x00\x00\xf3\x3c\xff\xc6\x31\x8c\x7e\xe8\x38\x80\x00\x00"
    "\x00\x00\x00\x00\x74\x7c\xff\x7e\x0e\x8f\xd9\x1f\xc6\x2e\x00\x00\x00\x00"
    "\x00\x00\x84\x63\x08\xc2\x11\x89\x25\x28\x46\x31\x00\x00\x00\x00\x00\x00"
    "\xbc\x63\x08\xc2\x11\x89\x25\x48\x46\x31\x00\x00\x00\x00\x00\x00\xbf\xfd"
    "\x08\xf3\x93\xf9\x05\x88\x46\x71\x00\x00\x00\x00\x00\x00\x8c\x63\x08\xc2"
    "\x10\x89\x05\x48\x46\xb1\x00\x00\x00\x00\x00\x00\xbc\x63\x08\xc2\x10\x89"
    "\x05\x28\x57\x31\x00\x00\x00\x00\x00\x00\x8a\xa3\x08\xc2\x11\x89\x05\x18"
    "\x6e\x31\x00\x00\x00\x00\x00\x00\x71\x3c\xff\x7f\xee\x8f\xff\x18\x46\x2e"
    "\x00\x00\x00\x00\x00\x00\x73\xbf\xe1\x79\xc8\x73\x80\x01\x01\x04\x00\x00"
    "\x00\x00\x00\x00\xc9\x10\x11\x06\x28\x8c\x40\x82\x00\x80\x00\x00\x00\x00"
    "\x00\x00\xc9\x08\x11\x06\x24\x88\x48\x44\x38\x44\x00\x00\x00\x00\x00\x00"
    "\xa9\x04\xef\xfb\xc4\x73\xc0\x08\x00\x22\x00\x00\x00\x00\x00\x00\xa9\x02"
    "\x19\x42\x02\x8c\x40\x04\x38\x41\x00\x00\x00\x00\x00\x00\x99\x02\x19\x42"
    "\x02\x8c\x48\x42\x00\x91\x00\x00\x00\x00\x00\x00\x99\x22\x15\x42\x31\x8c"
    "\x40\x01\x01\x11\x00\x00\x00\x00\x00\x00\x73\x1d\xe5\x7d\xdf\x73\x80\x00"
    "\x00\x0e\x00\x00\x00\x00\x00\x00\xf9\x00\xa2\x4d\xa0\x12\x00\x06\x00\x88"
    "\x00\x00\x00\x00\x00\x00\x88\x00\xa7\x4e\x40\x21\x00\x42\x00\x08\x00\x00"
    "\x00\x00\x00\x00\x01\x01\xf1\x22\xa0\x40\x94\x40\x00\x04\x00\x00\x00\x00"
    "\x00\x00\x03\x80\xa7\x11\x00\x40\x89\xf0\x7c\x04\x00\x00\x00\x00\x00\x00"
    "\x03\x80\xa4\x0a\x80\x40\x9c\x40\x00\x02\x00\x00\x00\x00\x00\x00\x03\x81"
    "\xf7\x66\x40\x40\x88\x40\x00\x02\x00\x00\x00\x00\x00\x00\x03\x94\xa2\x66"
    "\x44\x21\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x03\x94\xa2\x01\x84\x12"
    "\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff"
    "\xff\xff\x00\x00\x00\x00\x00\x00\x8c\x63\x18\xc6\x31\x8c\x63\x18\xc6\x31"
    "\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00"
    "\x00\x00\x00\x00\x8c\x63\x18\xc6\x31\xff\xff\xff\xff\xff\x00\x00\x00\x00"
    "\x00\x00\x8c\x63\x1f\xff\xff\x8c\x63\x1f\xff\xff\x00\x00\x00\x00\x00\x00"
    "\x8c\x7f\xf8\xc7\xff\x8c\x7f\xf8\xc7\xff\x00\x00\x00\x00\x00\x00\x8f\xe3"
    "\xf8\xfe\x3f\x8f\xe3\xf8\xfe\x3f\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff"
    "\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff"
    "\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x8c\x63\x18\xc6\x31\x8c\x63\x18"
    "\xc6\x31\x00\x00\x00\x00\x00\x00\x8c\x63\x18\xc6\x31\x8c\x63\x18\xc6\x31"
    "\x00\x00\x00\x00\x00\x00\x8c\x63\x18\xc6\x31\xff\xff\xff\xff\xff\x00\x00"
    "\x00\x00\x00\x00\x8c\x63\x1f\xff\xff\x8c\x63\x1f\xff\xff\x00\x00\x00\x00"
    "\x00\x00\x8c\x7f\xf8\xc7\xff\x8c\x7f\xf8\xc7\xff\x00\x00\x00\x00\x00\x00"
    "\x8f\xe3\xf8\xfe\x3f\x8f\xe3\xf8\xfe\x3f\x00\x00\x00\x00\x00\x00\xff\xff"
    "\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00";


