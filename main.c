#include <stdio.h>
#include <sodium.h>
#include <string.h>
#include <time.h>
#include "monocypher.h"
#include "micro_aes.h"
#include "stinky_aes_128_gcm_encrypt.h"


void stinky_aes(unsigned char* message, unsigned long message_len, unsigned char* ciphertext){
   
    unsigned char key[16];
    unsigned char nonce[12];

    randombytes_buf(key, sizeof key);
    randombytes_buf(nonce, sizeof nonce);

    temp_AES_GCM(ciphertext, message, message_len, NULL, 0, key, nonce);    
}
void micro_aes(unsigned char* message, unsigned long message_len, unsigned char* ciphertext){
   
    unsigned char key[16];
    unsigned char nonce[12];

    randombytes_buf(key, sizeof key);
    randombytes_buf(nonce, sizeof nonce);

    AES_GCM_encrypt(key,     
                        nonce,   
                        NULL,      
                        0,  
                        message,      
                        message_len,  
                        ciphertext);          

}
void libsodium_aes(unsigned char* message, unsigned long message_len, unsigned char* ciphertext){

    unsigned char nonce[crypto_aead_aes256gcm_NPUBBYTES];
    unsigned char key[crypto_aead_aes256gcm_KEYBYTES];

    crypto_aead_aes256gcm_keygen(key);
    randombytes_buf(nonce, sizeof nonce);

    unsigned long long ciphertext_len = 0;

    crypto_aead_aes256gcm_encrypt(ciphertext, &ciphertext_len,
                                          message, message_len,
                                          NULL, 0,
                                          NULL, nonce, key);                                     
     
}
void libsodium_chacha20poly1305(unsigned char* message, unsigned long message_len, unsigned char* ciphertext){
    unsigned char nonce[crypto_aead_chacha20poly1305_IETF_NPUBBYTES];
    unsigned char key[crypto_aead_chacha20poly1305_IETF_KEYBYTES];

    crypto_aead_chacha20poly1305_ietf_keygen(key);
    randombytes_buf(nonce, sizeof nonce);

    unsigned long long ciphertext_len = 0;

    crypto_aead_chacha20poly1305_ietf_encrypt(ciphertext, &ciphertext_len,
                                          message, message_len,
                                          NULL, 0,
                                          NULL, nonce, key);
}
void monocypher_chacha20poly1305(unsigned char* message, unsigned long message_len, unsigned char* ciphertext){
    unsigned char nonce[crypto_aead_chacha20poly1305_IETF_NPUBBYTES];
    unsigned char key[crypto_aead_chacha20poly1305_IETF_KEYBYTES];

    crypto_aead_chacha20poly1305_ietf_keygen(key);
    randombytes_buf(nonce, sizeof nonce);

    crypto_aead_ctx ctx = {0};
    crypto_aead_init_ietf(&ctx, key, nonce);

    unsigned char mac[16];

    crypto_aead_write(&ctx, ciphertext, mac, NULL, 0, message, message_len);
}

int main(){   
    if (sodium_init() < 0) {
        printf("libsoidum did not init. PANIC!\n");
    }
    if (crypto_aead_aes256gcm_is_available() == 0) {
        printf("Not available on this CPU\n");
        abort(); /* Not available on this CPU */
    }


    unsigned long long message_len = (long) 1 * 1000 * 1000 * 1000;
    // unsigned long message_len = 1 * 1000 * 1000 * 100; // micro_aes
    unsigned char* message = malloc(message_len);
    unsigned char* ciphertext = malloc(message_len + 100);
    unsigned long long ciphertext_len;
    
    if(message == NULL){
        printf("malloc failed! message\n");
    }
    if(ciphertext == NULL){
        printf("malloc failed! ciphertext\n");
    }

    printf("Each test encrypts %0.2fGBs of data.\n", (double)message_len / 1000000000.0);

    float avg_time = 0;
    int times_ran = 10;
    for(int i = 0; i < times_ran; i++){
        printf("Running test [%d/%d]: ", i+1, times_ran);

        memset(message, i, message_len);
        float start_time = (float)clock()/CLOCKS_PER_SEC;


        libsodium_aes(message, message_len, ciphertext);

        libsodium_chacha20poly1305(message, message_len, ciphertext);

        // monocypher_chacha20poly1305(message, message_len, ciphertext);
        
        // micro_aes(message, message_len, ciphertext);

        // stinky_aes(message, message_len, ciphertext);


        float end_time = (float)clock()/CLOCKS_PER_SEC;
        float dif_time = end_time - start_time; 

        printf("%fs\n",dif_time);
        avg_time += dif_time;
    }
    
    avg_time /= times_ran;
    printf("==========\navg_time %f\n==========\n",avg_time);

    free(message);
    free(ciphertext);

    return 0;
}

