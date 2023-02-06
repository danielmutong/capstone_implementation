#include <iostream>
#include <vector>
#include <thread>
#include <iomanip>
using namespace std;

#define W 4
#define H 4
#define CH 12
#define SCALE 2
#define BATCH_SIZE 1

float input_arr[BATCH_SIZE][CH][W][H];
float final_arr[BATCH_SIZE][CH / (SCALE * SCALE)][W * SCALE][H * SCALE];

void print_output_array()
{
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < (W * 2); k++)
            {
                for (int z = 0; z < (H * 2); z++)
                {
                    cout << std::setprecision(1) << std::fixed << final_arr[i][j][k][z] << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
        cout << endl;
    }
}

void print_vector(vector<float> &x)
{
    for (int i = 0; i < x.size(); i++)
    {
        cout << to_string(x[i]) << " ";
    }
    cout << endl;
}

//alternate processing even and odd rows
void process_rows(int base, int index, int row_parity, int channel_number)
{
    int arr = base + channel_number;
    int channel_counter = 0;
    int row_counter = 0;
    int channel_number_reset = 0;

    if (row_parity == 0)
        row_counter = 0;
    else
        row_counter = 1;
    if (channel_number == 0 || channel_number == 2)
        channel_number_reset = 0;
    else
        channel_number_reset = 1;

    channel_counter = channel_number_reset;
    for (int i = 0; i < W; i++)
    {
        for (int j = 0; j < H; j++)
        {
            final_arr[0][index][row_counter][channel_counter] = input_arr[0][arr][i][j];
            channel_counter += 2;
        }
        channel_counter = channel_number_reset;
        row_counter += 2;
    }
}

//for each final channel, 
void process_4_channels(int base, int index)
{
    // process a seperate channel each
    process_rows(base, index, 0, 0);
    process_rows(base, index, 0, 1);
    process_rows(base, index, 1, 2);
    process_rows(base, index, 1, 3);
}

void pixelshuffle(float arr[1][12][4][4], float final_arr[1][3][8][8])
{
    int count = 0;

    // reduce to 3 channels
    for (int i = 0; i < 3; i++)
    {
        process_4_channels(count, i);
        count += 4;
    }
}

int main()
{
    int count = 0;

    // initialize array
    for (int i0 = 0; i0 < 1; i0++)
    {
        for (int i1 = 0; i1 < 12; i1++)
        {
            for (int i2 = 0; i2 < W; i2++)
            {
                for (int i3 = 0; i3 < H; i3++)
                {
                    input_arr[i0][i1][i2][i3] = count;
                    count++;
                }
            }
        }
    }
    pixelshuffle(input_arr, final_arr);
    // print array
    print_output_array();
    return 0;
}