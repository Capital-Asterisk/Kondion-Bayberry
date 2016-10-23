#define KTT2_HEADER_PIXEL(data,pixel) {\
pixel[0] = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4)); \
pixel[1] = ((((data[1] - 33) & 0xF) << 4) | ((data[2] - 33) >> 2)); \
pixel[2] = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33))); \
data += 4; \
}

static char *ktt2_data =
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
