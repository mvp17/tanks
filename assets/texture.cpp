#include "../opengl.h"
#include "jpeglib.h"

using namespace std;

class Texture {
   public:
    unsigned char *image;
    int height, width;
    Texture(){};
    ~Texture(){};
    void readJPEG(const char *);
};

void Texture::readJPEG(const char *filename) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *infile;
    unsigned char **buffer;
    int i, j;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    if ((infile = fopen(filename, "rb")) == NULL) {
        printf("Unable to open file %s\n", filename);
        exit(1);
    }

    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_calc_output_dimensions(&cinfo);
    jpeg_start_decompress(&cinfo);

    width = cinfo.output_width;
    height = cinfo.output_height;

    image = (unsigned char *) malloc(cinfo.output_width * cinfo.output_height * cinfo.output_components);

    buffer = (unsigned char **)malloc(1 * sizeof(unsigned char **));
    buffer[0] = (unsigned char *)malloc(cinfo.output_width * cinfo.output_components);

    i = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);

        for (j = 0; j < cinfo.output_width * cinfo.output_components; j++) {
            image[i] = buffer[0][j];
            i++;
        }
    }

    free(buffer);
    jpeg_finish_decompress(&cinfo);
}