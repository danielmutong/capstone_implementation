#include <iostream>
#include <vector>
#include <thread>
#include <iomanip>
using namespace std;

#define DIM 8
#define CH 12
#define SCALE 2
#define BATCH_SIZE 1

std::vector<std::vector<float>> final_channels(3, std::vector<float>(256)); //3x256

float input_arr[BATCH_SIZE][CH][DIM][DIM];
float final_arr[BATCH_SIZE][CH / (SCALE * SCALE)][DIM * SCALE][DIM * SCALE];

void print_output_array()
{
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < (DIM * 2); k++)
            {
                for (int z = 0; z < (DIM * 2); z++)
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
void print_vector(vector <float> & x )
{
    for (int i = 0; i < x.size(); i++)
    {
        cout << to_string(x[i]) << " " ;
    }
    cout << endl;
}

void process_4_channels(int start, int index) 
{
    int arr0 = start;
    int arr1 = start + 1;
    int arr2 = start + 2;
    int arr3 = start + 3;
    int channel_counter = 0;

    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            final_channels[index][channel_counter] = input_arr[0][arr0][i][j];
            channel_counter++;
            final_channels[index][channel_counter] = input_arr[0][arr1][i][j];
            channel_counter++;
        }
        for (int j = 0; j < DIM; j++)
        {
            final_channels[index][channel_counter] = input_arr[0][arr2][i][j];
            channel_counter++;
            final_channels[index][channel_counter] = input_arr[0][arr3][i][j];
            channel_counter++;
        }
    }
}

void construct_final_array(vector <float> & final_flat)
{
    int count = 0;
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < (DIM * 2); k++)
            {
                for (int z = 0; z < (DIM * 2); z++)
                {
                    final_arr[i][j][k][z] = final_flat[count];
                    count++;
                }
            }
        }
    }
}

void pixelshuffle(float arr[1][12][8][8], float final_arr[1][3][16][16])
{
    int count = 0;

    //reduce to 3 channels
    std::vector<std::thread> ThreadVector;
    for (int i = 0; i < 3; i++)
    {
        std::thread t(process_4_channels, count, i);
        ThreadVector.push_back(std::move(t));
        count += 4;
    }
    for (auto &t : ThreadVector)
        t.join();

    //combine channels into single flat array
    std::vector<float> final_flat = final_channels[0];
    final_flat.insert(final_flat.end(), final_channels[1].begin(), final_channels[1].end());
    final_flat.insert(final_flat.end(), final_channels[2].begin(), final_channels[2].end());

    //construct final arr
    construct_final_array(final_flat);
}

int main(){
    int count = 0;

    //initialize array
    for (int i0 = 0; i0 < 1; i0++)
    {
        for (int i1 = 0; i1 < 12; i1++)
        {
            for (int i2 = 0; i2 < DIM; i2++)
            {
                for (int i3 = 0; i3 < DIM; i3++)
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