// AUTHOR: ADAM RETANA
// DATE: 7/16/2023
// INSTITUTION DREXEL UNIVERSITY
// NOTES:
//		Seems to work well and only writes necassary bytes based on xdump output.


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void docompress(int, int, int, int, int, int);


int
main() {
	int width, height, max, n, pixel_grid_side, new_width, new_height, size, new_size, num_chars;
	char buf[30];

	n = scanf("P5 %d %d %d\n", &width, &height, &max);

	pixel_grid_side = (width / 300);

	new_width = width / pixel_grid_side;
	new_height = height / pixel_grid_side;

	size = width * height;
	new_size = new_width * new_height;

	num_chars = sprintf(buf, "P5\n%d %d\n%d\n", new_width, new_height, max); 
	write(1, buf, num_chars);

	docompress(0, size, new_size, max, pixel_grid_side, width);


	return 0;
}

void
docompress(int fd, int size, int new_size, int max, int grid_side, int width){
	int n, i, j, oct_buf_num, actual_bytes;
	char *p;
	char buf[size];
	char buf2[new_size];

	actual_bytes = 0;

	while(1){
		n = fread(buf, 1, size, stdin);

		if (n < 0) {
			perror("read");
			exit(1);
		}
		if(n == 0)
			break;

		p = buf2;
		for(i = 0; i < n; i += width) {
			for(j = 0; j < width; j++) {
				oct_buf_num = buf[i+j] & 0xff;
				if (j % grid_side == 0 && (i % (width * grid_side)) == 0 && oct_buf_num <= max) {
					*(p++) = oct_buf_num;
					actual_bytes++;
				}
			}
		}
		*(p++) = '\n';
		write(1, buf2, p - buf2);
	}
	close(fd);
}
