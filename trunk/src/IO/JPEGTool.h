#include <stdio.h>
#include <jpeglib/jpeglib.h>
#include <setjmp.h>

#include <iostream>
#include <fstream>
#include <vector>

#ifndef JPEGTOOLS
#define JPEGTOOLS

static void write_JPEG_file(char * filename, int quality, uint8_t* data)
{
	/* This struct contains the JPEG compression parameters and pointers to
	* working space (which is allocated as needed by the JPEG library).
	* It is possible to have several such structures, representing multiple
	* compression/decompression processes, in existence at once.  We refer
	* to any one struct (and its associated working data) as a "JPEG object".
	*/
	struct jpeg_compress_struct cinfo;
	/* This struct represents a JPEG error handler.  It is declared separately
	* because applications often want to supply a specialized error handler
	* (see the second half of this file for an example).  But here we just
	* take the easy way out and use the standard error handler, which will
	* print a message on stderr and call exit() if compression fails.
	* Note that this struct must live as long as the main JPEG parameter
	* struct, to avoid dangling-pointer problems.
	*/
	struct jpeg_error_mgr jerr;
	/* More stuff */
	FILE * outfile;		/* target file */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	JSAMPIMAGE image_pointer[1]; // IMAGE
	int row_stride;		/* physical row width in image buffer */

	/* Step 1: allocate and initialize JPEG compression object */

	/* We have to set up the error handler first, in case the initialization
	* step fails.  (Unlikely, but it could happen if you are out of memory.)
	* This routine fills in the contents of struct jerr, and returns jerr's
	* address which we place into the link field in cinfo.
	*/
	cinfo.err = jpeg_std_error(&jerr);
	/* Now we can initialize the JPEG compression object. */
	jpeg_create_compress(&cinfo);

	/* Step 2: specify data destination (eg, a file) */
	/* Note: steps 2 and 3 can be done in either order. */

	/* Here we use the library-supplied code to send compressed data to a
	* stdio stream.  You can also write your own code to do something else.
	* VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
	* requires it in order to write binary files.
	*/
	/*if ((outfile = fopen(filename, "wb")) == NULL) {
		fprintf(stderr, "can't open %s\n", filename);
		exit(1);
	}
	jpeg_stdio_dest(&cinfo, outfile);*/

	unsigned char *mem = NULL;
	unsigned long mem_size = 0;
	jpeg_mem_dest(&cinfo,&mem, &mem_size);

	/* Step 3: set parameters for compression */

	int image_width = 640;
	int image_height = 480;

	/* First we supply a description of the input image.
	* Four fields of the cinfo struct must be filled in:
	*/
	cinfo.image_width = image_width; 	/* image width and height, in pixels */
	cinfo.image_height = image_height;
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	/* Now use the library's routine to set default compression parameters.
	* (You must set at least cinfo.in_color_space before calling this,
	* since the defaults depend on the source color space.)
	*/
	jpeg_set_defaults(&cinfo);
	/* Now you can set any non-default parameters you wish to.
	* Here we just illustrate the use of quality (quantization table) scaling:
	*/
	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

	/* Step 4: Start compressor */

	/* TRUE ensures that we will write a complete interchange-JPEG file.
	* Pass TRUE unless you are very sure of what you're doing.
	*/
	jpeg_start_compress(&cinfo, TRUE);

	/* Step 5: while (scan lines remain to be written) */
	/*           jpeg_write_scanlines(...); */

	/* Here we use the library's state variable cinfo.next_scanline as the
	* loop counter, so that we don't have to keep track ourselves.
	* To keep things simple, we pass one scanline per call; you can pass
	* more if you wish, though.
	*/
	row_stride = image_width * 3;	/* JSAMPLEs per row in image_buffer */
	int t = 0;

	while (cinfo.next_scanline < cinfo.image_height) {
		t = cinfo.next_scanline * row_stride;
		//std::cout << t << " ROW STRIDE = " << row_stride << "WDITH: " << image_width << std::endl;

		row_pointer[0] = &data[t];
		(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
		std::cout << mem_size << std::endl;
	}
	/*image_pointer[0] = (JSAMPIMAGE)& data[0];
	(void)jpeg_write_raw_data(&cinfo, image_pointer[0], image_height);*/

	/* Step 6: Finish compression */

	jpeg_finish_compress(&cinfo);
	/* After finish_compress, we can close the output file. */
	//fclose(outfile);

	/* Step 7: release JPEG compression object */

	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_compress(&cinfo);

	/* And we're done! */
}

static size_t compressImage(char* buffer, size_t width, size_t height, unsigned char* outputBuffer, int quality){
	unsigned char* bits = (unsigned char*)buffer;
	unsigned char* outp = new unsigned char[60000];
	memset(outp, '0', (size_t)(60000));

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	JSAMPROW row_pointer[1];  //Pointer to JSAMPLE row[s]

	int row_stride = width * 3; //Physical row width in image buffer
	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_compress(&cinfo);

	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	unsigned long outlen = 0;

	jpeg_mem_dest(&cinfo, &outp, &outlen);

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, (boolean)true);
	jpeg_start_compress(&cinfo, (boolean)true);

	while (cinfo.next_scanline < cinfo.image_height){
		row_pointer[0] = &bits[cinfo.next_scanline * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	//std::cout << outlen << std::endl;
	memcpy(outputBuffer, outp, outlen);

	delete outp;

	return outlen;
}


static void decompressImage(unsigned char* input, size_t outlen, std::vector<uint8_t>& output){

	int i;
	unsigned char * bits;
	struct jpeg_decompress_struct dinfo;
	struct jpeg_error_mgr jerr;
	JSAMPARRAY buffer;
	int row_stride;     //Physical row width in image buffer

	dinfo.err = jpeg_std_error(&jerr);

	//Initlize data structures to read file
	jpeg_create_decompress(&dinfo);
	jpeg_mem_src(&dinfo, input, outlen);

	jpeg_read_header(&dinfo, TRUE);
	jpeg_start_decompress(&dinfo);

	unsigned int width = dinfo.output_width;
	unsigned int height = dinfo.output_height;

	row_stride = dinfo.output_width * dinfo.output_components;
	buffer = (*dinfo.mem->alloc_sarray)((j_common_ptr)&dinfo, JPOOL_IMAGE, row_stride, 1);

	if (dinfo.out_color_space == JCS_GRAYSCALE){
		int at = 0;
		bits = new unsigned char[width * height];
		output.resize(width * height );
		while (dinfo.output_scanline < dinfo.output_height){
			jpeg_read_scanlines(&dinfo, buffer, 1);
			for (i = 0; i<width; i++){
				bits[at++] = buffer[0][i];
			}
		}
		memcpy(&output[0], &bits[0], width * height);
	}
	else{
		int at = 0;
		bits = new unsigned char[width * height * 3];
		output.resize(width * height * 3);
		while (dinfo.output_scanline < dinfo.output_height){
			jpeg_read_scanlines(&dinfo, buffer, 1);
			for (i = 0; i<width; i++){
				bits[at++] = buffer[0][i * 3 + 0];
				bits[at++] = buffer[0][i * 3 + 1];
				bits[at++] = buffer[0][i * 3 + 2];
			}
		}
		memcpy(&output[0], &bits[0], width * height * 3);
	}

	jpeg_finish_decompress(&dinfo);
	jpeg_destroy_decompress(&dinfo);

	delete bits;
}

#endif
