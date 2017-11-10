// 정보보호개론 Assignment #1
// 2015318646 이수연

#include <stdio.h>
#include <string.h>

unsigned char cipher_text[5100], key[15], KEY_LENGTH;
unsigned int text_length;

int find_key_length(void);
int find_key_sequence(void);
float get_chi_square(int mod, unsigned char value);
void decrypt_text(void);

int main(void) {
	FILE *fpIn;
	int i;
	unsigned char ch;

	memset(cipher_text, 0, 5100);

	fpIn = fopen("hw1_input.txt", "r");
	for(i = 0; fscanf(fpIn, "%c", &ch) != EOF; ++i) {
		cipher_text[i] = ch;
	}
	text_length = i;
	fclose(fpIn);

	find_key_length();

	find_key_sequence();

	decrypt_text();

	return 0;
}

void decrypt_text(void) {
	FILE *fpOut;
	int i;

	fpOut = fopen("hw1_output.txt", "w");

	for(i = 0; i < KEY_LENGTH; i++) {
		fprintf(fpOut, "0x%02x ", key[i]);
	}	fprintf(fpOut, "\n");

	for(i = 0; i < text_length; i++) {
		fprintf(fpOut, "%c", cipher_text[i] ^ key[i%KEY_LENGTH]);
	}

	fclose(fpOut);
}

int find_key_length(void) {
	int i, j, k, cnt, length = 1;
	unsigned int frequency[256];
	float p[15], max = 0;

	memset(p, 0, sizeof(float) * 15);

	for(i = 1; i <= 10; i++) {
		for(j = 0; j < i; j++) {
			memset(frequency, 0, sizeof(int) * 256);
			cnt = 0;

			for(k = j; k < text_length; k += i) {
				frequency[cipher_text[k]]++;
				cnt++;
			}

			for(k = 0; k < 256; k++) {
				p[i] += ((float)(frequency[k] * (frequency[k] - 1)) / (cnt * (cnt - 1)));
			}
		}
		p[i] /= i;
	}

	for(i = 1; i <= 10; i++) {
		if(max <= p[i]) {
			max = p[i];
			length = i;
		}
		if((i < 10 && max >= p[i+1] && max >= 0.05) || (i == 1 && max >= 0.05))
			break;
	}

	KEY_LENGTH = length;

	return 0;
}

int find_key_sequence(void) {
	int i, max;
	unsigned char ch, key_tmp;
	float chi_square[256];

	for(i = 1; i <= KEY_LENGTH; i++) {
		for(ch = 0; ch < 256; ch++) {
			chi_square[ch] = get_chi_square(i, ch);
			if(ch == 255)
				break;
		}

		key_tmp = 0;
		for(ch = 0; ch < 256; ch++) {
			if(chi_square[ch] <= chi_square[key_tmp])
				key_tmp = ch;
			if(ch == 255)
				break;
		}

		key[i-1] = key_tmp;
	}
}

float get_chi_square(int mod, unsigned char value) {
	float expected_frq[26] = {0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015, 
		0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 
		0.05987, 0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150, 0.01974, 0.00074};
	int i, cnt = 0, frequency[256];
	float chi_square = 0;
	
	memset(frequency, 0, sizeof(int) * 256);
	for(i = mod - 1; i < text_length; i += KEY_LENGTH) {
		frequency[cipher_text[i] ^ value]++;
		if((cipher_text[i] ^ value >= 'a') && (cipher_text[i] ^ value) <= 'z')
			cnt++;
	}

	for(i = 'a'; i <= 'z'; i++) {
		chi_square += (((double)frequency[i] - (expected_frq[i-'a'] * cnt)) * 
				((double)frequency[i] - (expected_frq[i-'a'] * cnt)) / (expected_frq[i-'a'] * cnt));
	}

	if(chi_square != chi_square)	// checks to see if 'chi_square' is NaN
		return 10000000;

	return chi_square;
}
