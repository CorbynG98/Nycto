#include "system.h"
#include "pio.h"
#include "pacer.h"

static int previous_column = 0;

/** Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] = {
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/** Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] = {
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


static const uint8_t bitmap[] = {
    0x4F, 0x48, 0x7F, 0x09, 0x79
};


static void display_column (uint8_t row_pattern, uint8_t current_column)
{
    pio_output_high(cols[previous_column]);
    int i = 0;
    for (i = 0; i < 7; i++) {
        if ((row_pattern >> i) & 1) {
            pio_output_low (rows[i]);
        } else {
            pio_output_high (rows[i]);
        }
    }
    pio_output_low(cols[current_column]);
    previous_column = current_column;
}


int main (void)
{
    uint8_t current_column = 0;

    system_init ();
    pacer_init (500);

    /* TODO: Initialise LED matrix pins.  */
    pio_config_set(LEDMAT_COL1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL5_PIO, PIO_OUTPUT_HIGH);

    pio_config_set(LEDMAT_ROW1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW5_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW6_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW7_PIO, PIO_OUTPUT_HIGH);


    while (1) {
        pacer_wait ();

        display_column (bitmap[current_column], current_column);

        current_column++;

        if (current_column > (LEDMAT_COLS_NUM - 1)) {
            current_column = 0;
        }
    }
}
