#include "bmp.h"
#include <stdlib.h>

int main(int argc, char * argv[]) {
	char * error = NULL;
	FILE * fp = fopen("6x6_24bit.bmp", "r");
	BMPImage * bmp = read_bmp(fp, &error);
	fclose(fp);
	fp = fopen("6x6_24bit_copy.bmp", "w");
	if(write_bmp(fp, bmp, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	free_bmp(bmp);
	
	fp = fopen("green_earth.bmp", "r");
	bmp = read_bmp(fp, &error);
	fclose(fp);
	fp = fopen("green_earth_copy.bmp", "w");
	if(write_bmp(fp, bmp, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	free_bmp(bmp);

	fp = fopen("airplane16.bmp", "r");
	bmp = read_bmp(fp, &error);
	fclose(fp);
	fp = fopen("airplane16_copy.bmp", "w");
	if(write_bmp(fp, bmp, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	free_bmp(bmp);
	
	fp = fopen("6x6_24bit.bmp", "r");
	bmp = read_bmp(fp, &error);
	BMPImage * crop = crop_bmp(bmp, 1, 1, 5, 5, &error);
	fclose(fp);
	fp = fopen("6x6_24bit_crop.bmp", "w");
	if(write_bmp(fp, crop, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	if(bmp != NULL) {
		free_bmp(bmp);
	}
	if(crop != NULL) {
		free_bmp(crop);
	}

	fp = fopen("airplane.bmp", "r");
	bmp = read_bmp(fp, &error);
	crop = crop_bmp(bmp, 110, 1, 500, 407, &error);
	fclose(fp);
	fp = fopen("airplane_crop.bmp", "w");
	if(crop == NULL || write_bmp(fp, crop, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	if(bmp != NULL) {
		free_bmp(bmp);
	}
	if(crop != NULL) {
		free_bmp(crop);
	}

	fp = fopen("airplane16.bmp", "r");
	bmp = read_bmp(fp, &error);
	crop = crop_bmp(bmp, 1, 1, 500, 100, &error);
	fclose(fp);
	fp = fopen("airplane16_crop.bmp", "w");
	if(crop == NULL || write_bmp(fp, crop, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	if(bmp != NULL) {
		free_bmp(bmp);
	}
	if(crop != NULL) {
		free_bmp(crop);
	}
      
	fp = fopen("airplane16.bmp", "r");
	bmp = read_bmp(fp, &error);
	crop = crop_bmp(bmp, 110, 1, 500, 407, &error);
	fclose(fp);
	fp = fopen("airplane16_crop1.bmp", "w");
	if(crop == NULL || write_bmp(fp, crop, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	if(bmp != NULL) {
		free_bmp(bmp);
	}
	if(crop != NULL) {
		free_bmp(crop);
	}
      
	fp = fopen("6x6_24bit.bmp", "r");
	bmp = read_bmp(fp, &error);
	crop = crop_bmp(bmp, 11, 1, 3, 3, &error);
	fclose(fp);
	fp = fopen("6x6_24bit_crop.bmp", "w");
	if(crop == NULL || write_bmp(fp, crop, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	if(bmp != NULL) {
		free_bmp(bmp);
	}
	if(crop != NULL) {
		free_bmp(crop);
	}

	fp = fopen("6x6_24bit.bmp", "r");
	bmp = read_bmp(fp, &error);
	crop = crop_bmp(bmp, 1, 11, 3, 3, &error);
	fclose(fp);
	fp = fopen("6x6_24bit_crop.bmp", "w");
	if(crop == NULL || write_bmp(fp, crop, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	if(bmp != NULL) {
		free_bmp(bmp);
	}
	if(crop != NULL) {
		free_bmp(crop);
	}

	fp = fopen("6x6_24bit.bmp", "r");
	bmp = read_bmp(fp, &error);
	crop = crop_bmp(bmp, 1, 1, 31, 3, &error);
	fclose(fp);
	fp = fopen("6x6_24bit_crop.bmp", "w");
	if(crop == NULL || write_bmp(fp, crop, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	if(bmp != NULL) {
		free_bmp(bmp);
	}
	if(crop != NULL) {
		free_bmp(crop);
	}

	fp = fopen("6x6_24bit.bmp", "r");
	bmp = read_bmp(fp, &error);
	crop = crop_bmp(bmp, 1, 1, 1, 31, &error);
	fclose(fp);
	fp = fopen("6x6_24bit_crop.bmp", "w");
	if(crop == NULL || write_bmp(fp, crop, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	if(bmp != NULL) {
		free_bmp(bmp);
	}
	if(crop != NULL) {
		free_bmp(crop);
	}

	fp = fopen("corrupt_green_earthh.bmp", "r");
	bmp = read_bmp(fp, &error);
	crop = crop_bmp(bmp, 110, 1, 500, 407, &error);
	fclose(fp);
	fp = fopen("corrupt_green_earthh_copy.bmp", "w");
	if(crop == NULL || write_bmp(fp, crop, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	if(bmp != NULL) {
		free_bmp(bmp);
	}
	if(crop != NULL) {
		free_bmp(crop);
	}

	fp = fopen("corrupt_purduetraini.bmp", "r");
	bmp = read_bmp(fp, &error);
	crop = crop_bmp(bmp, 110, 1, 500, 407, &error);
	fclose(fp);
	fp = fopen("corrupt_purduetraini_copy.bmp", "w");
	if(crop == NULL || write_bmp(fp, crop, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	if(bmp != NULL) {
		free_bmp(bmp);
	}
	if(crop != NULL) {
		free_bmp(crop);
	}
	
	fp = fopen("car16.bmp", "r");
	bmp = read_bmp(fp, &error);
	crop = crop_bmp(bmp, 1, 1, 200, 200, &error);
	fclose(fp);
	fp = fopen("car16_crop.bmp", "w");
	if(crop == NULL || write_bmp(fp, crop, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	if(bmp != NULL) {
		free_bmp(bmp);
	}
	if(crop != NULL) {
		free_bmp(crop);
	}

	fp = fopen("corrupt_carw.bmp", "r");
	bmp = read_bmp(fp, &error);
	crop = crop_bmp(bmp, 1, 1, 200, 200, &error);
	fclose(fp);
	fp = fopen("corrupt_carw_copy.bmp", "w");
	if(crop == NULL || write_bmp(fp, crop, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	if(bmp != NULL) {
		free_bmp(bmp);
	}
	if(crop != NULL) {
		free_bmp(crop);
	}

	fp = fopen("corrupt_green_earthh.bmp", "r");
	bmp = read_bmp(fp, &error);
	crop = crop_bmp(bmp, 1, 1, 200, 200, &error);
	fclose(fp);
	fp = fopen("corrupt_green_earthh_copy.bmp", "w");
	if(crop == NULL || write_bmp(fp, crop, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	if(bmp != NULL) {
		free_bmp(bmp);
	}
	if(crop != NULL) {
		free_bmp(crop);
	}

	fp = fopen("corrupt_purduetrainw.bmp", "r");
	bmp = read_bmp(fp, &error);
	crop = crop_bmp(bmp, 110, 1, 500, 407, &error);
	fclose(fp);
	fp = fopen("corrupt_purduetrainw_copy.bmp", "w");
	if(crop == NULL || write_bmp(fp, crop, &error) == false) {
		printf("%s\n", error);
	}
	fclose(fp);
	if(bmp != NULL) {
		free_bmp(bmp);
	}
	if(crop != NULL) {
		free_bmp(crop);
	}

	free(error);
	return EXIT_SUCCESS;
}
