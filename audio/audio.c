#include "address_map_nios2.h"

/* globals */
#define BUF_SIZE 8000  // about 10 seconds of buffer (@ 8K samples/sec)
#define BUF_THRESHOLD 96 // 75% of 128 word buffer
#include <stdio.h>
/* function prototypes */
//void check_KEY0s(int *, int *, int *);
//void check_KEY1s(int *, int *, int *);
void audio(int r, int p);int fifospace;
int left_buffer[BUF_SIZE];
int right_buffer[BUF_SIZE];

/*******************************************************************************
 * This program performs the following:
 * 	1. records audio for 10 seconds when KEY[0] is pressed. LEDR[0] is lit
 *	   while recording.
 * 	2. plays the recorded audio when KEY[1] is pressed. LEDR[1] is lit while
 *	   playing.
 ******************************************************************************/
void audio(int r, int p) {
    /* Declare volatile pointers to I/O registers (volatile means that IO load
       and store instructions will be used to access these pointer locations,
       instead of regular memory loads and stores) */
    volatile int * red_LED_ptr = (int *)LED_BASE;
    volatile int * audio_ptr   = (int *)AUDIO_BASE;
    
    /* used for audio record/playback */
    
    int record = 0, play = 0, buffer_index = 0;
    

    /* read and echo audio data */
    record = r;
    play   = p;

    
        //check_KEY0s(&record, &play, &buffer_index);
        if (record) {
            *(audio_ptr) = 0x4;
            *(audio_ptr) = 0x0;
            buffer_index=0;
            *(red_LED_ptr) = 0x1; // turn on LEDR[0]
            fifospace =
                *(audio_ptr + 1); // read the audio port fifospace register
            if ((fifospace & 0x000000FF) > BUF_THRESHOLD) // check RARC
            {
                // store data until the the audio-in FIFO is empty or the buffer
                // is full
                while ((fifospace & 0x000000FF) && (buffer_index < BUF_SIZE)) {
                    left_buffer[buffer_index]  = *(audio_ptr + 2);
                    right_buffer[buffer_index] = *(audio_ptr + 3);
                    ++buffer_index;

                    if (buffer_index == BUF_SIZE) {
                        // done recording
                        record         = 0;
                        *(red_LED_ptr) = 0x0; // turn off LEDR
                    }
                    fifospace = *(audio_ptr +
                                  1); // read the audio port fifospace register
                }
            }
        } else if (play) {
            buffer_index=0;
            *(audio_ptr) = 0x8;
            *(audio_ptr) = 0x0;
            *(red_LED_ptr) = 0x2; // turn on LEDR_1
            fifospace =
                *(audio_ptr + 1); // read the audio port fifospace register
            if ((fifospace & 0x00FF0000) > BUF_THRESHOLD) // check WSRC
            {
                // output data until the buffer is empty or the audio-out FIFO
                // is full
                for(int i=0; i<BUF_SIZE; i++){
                    left_buffer[i]=10000;
                    right_buffer[i]=10000;
                }
                while ((fifospace & 0x00FF0000) && (buffer_index < BUF_SIZE)) {
                    *(audio_ptr + 2) = left_buffer[buffer_index];
                    *(audio_ptr + 3) = right_buffer[buffer_index];
                    ++buffer_index;

                    if (buffer_index == BUF_SIZE) {
                        // done playback
                        play           = 0;
                        *(red_LED_ptr) = 0x0; // turn off LEDR
                    }
                    fifospace = *(audio_ptr +
                                  1); // read the audio port fifospace register
                    //printf("%d",*(audio_ptr + 2));
                }
            }
        }
    
}

/****************************************************************************************
 * Subroutine to read KEYs
****************************************************************************************/
/*void check_KEY0s(int * KEY0, int * KEY1, int * counter) {
    volatile int * KEY_ptr   = (int *)KEY_BASE;
    volatile int * audio_ptr = (int *)AUDIO_BASE;
    int            KEY_value;

    KEY_value = *(KEY_ptr); // read the pushbutton KEY values
    while (KEY_value) { // Key 누름 확인
			while ( KEY_value) { // Key 뗌 확인
			}
    }
    if (KEY_value == 0x1) // check KEY0
    {
        // reset counter to start recording
        *counter = 0;
        // clear audio-in FIFO
        *(audio_ptr) = 0x4;
        *(audio_ptr) = 0x0;

        *KEY0 = 1;
    }
}
void check_KEY1s(int * KEY0, int * KEY1, int * counter){
    volatile int * audio_ptr = (int *)AUDIO_BASE;
    if (KEY1 !=0) // check KEY1
    {
        // reset counter to start playback
        *counter = 0;
        // clear audio-out FIFO
        *(audio_ptr) = 0x8;
        *(audio_ptr) = 0x0;

        *KEY1 = 1;
    }
}*/
