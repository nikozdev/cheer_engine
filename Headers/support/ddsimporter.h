#pragma once
void loaddds(const char *path) {
	FILE* f;
	unsigned char header[124];
	unsigned char headerr;
	char tipo[4];
	if ((f = fopen(path, "rb")) != 0) {

		fread(tipo, 1, 4, f);
		if ((tipo[0] == 'D') && (tipo[1] == 'D') && (tipo[2] == 'S')) {
			fread(header, 1, 124, f);


			printf(" GUARDAMI %d", sizeof(header));
			unsigned int height = *(unsigned int*)&header[8];//trasforma il byte in un puntatore unsigned int* e poi in un numero normale attraverso il * davanti
			unsigned int width = *(unsigned int*)&header[12];
			unsigned int linearsize = *(unsigned int*)&header[16];
			unsigned int size = width * height * 3;
			unsigned int nmipmaps = *(unsigned int*)&header[24];;
			unsigned int blocksize = 0;//DDS e' fatto cosi purtroppo
			//preso l'header ora prendiamo il tipo

			unsigned int formato = 0;
			char* tip;

			tip = (char*)&header[80];

			printf(" GUARDAMI2 %s", tip);

			if (strcmp(tip, "DXT1") == 0) {
				formato = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				blocksize = 8;
			}
			else if (strcmp(tip, "DXT3") == 0) {
				formato = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				blocksize = 16;
			}
			else if (strcmp(tip, "DXT5") == 0) {
				formato = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				blocksize = 16;
			}
			else if ((strcmp(tip, "DXT5") != 0) && (strcmp(tip, "DXT3") != 0) && (strcmp(tip, "DXT1") != 0)) {

				printf("file non supportato"); fclose(f); 
			}//gli altri non li supporta
			unsigned char* data = nullptr;
			unsigned int a = 0;
			if (nmipmaps > 1) {
				a = linearsize * 2;
			}
			else { a = linearsize; }


			fseek(f, 0, SEEK_END);
			a = ftell(f);

			fseek(f, 128, SEEK_SET);

			data = (unsigned char*)malloc(a * sizeof(unsigned char));
			fread(data, 1, a, f);	printf("O %s", data);
			/*	fseek(f,0,SEEK_END);
				long a=0;
				a=ftell(f);

				fseek(f, 0, SEEK_SET);
				a = ftell(f);

				fread(data, 1, a, f);
				printf("O %s", data);
				//unsigned char data=fread()*/

			unsigned int w = width;
			unsigned int h = height;
			unsigned int offset = 0;

			//cominciamo a creare la texture vera e propria
			GLuint t = 0;
			glGenTextures(1, &t);
			printf("ttttt%d  ", t);
			glBindTexture(GL_TEXTURE_2D, t);
			printf("aaaaa %d  ", t);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			unsigned int dimens = 0;
			//mettiamo le mipmap
			for (int num = 0; num < nmipmaps; num++) {
				//le mipmap sono tante texture una la meta di dimensione dell'altra quindi:
				if ((w != 0) || (h != 0)) {
					printf("W e H %d %d ", blocksize, h);
					dimens = (((w + 3) / 4) * ((h + 3) / 4)) * blocksize;	printf("W e H %d %d ", dimens, h);
					glCompressedTexImage2D(GL_TEXTURE_2D, num, formato, w, h, 0, dimens, data + offset);
					offset = offset + dimens;
					w = w / 2;
					h = h / 2;
				}
				else { nmipmaps--; }
			}
		}
	}
	else { printf("impossibile aprire il file"); }
	 //forse dopo devo modificare le UV mettendo che le v sono v-1,forse
}