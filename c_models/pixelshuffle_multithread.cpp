#include <iostream>
#include <vector>
#include <thread>
#include <iomanip>
using namespace std;

#define DIM 8
#define CH 12
std::vector<std::vector<float>> final_channels(3, std::vector<float>(256)); //3x256
std::vector<std::vector<float>> flat_arrays(12, std::vector<float>(64)); // 12x64

float input_arr[1][12][8][8];
float final_arr[1][3][16][16];

void  flatten_array(int k)
{
    int index = 0;
    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            flat_arrays[k][index] = input_arr[0][k][i][j];
            index++;
        }
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

void process_4_channels(int start, int k) 
{
    int arr0 = start;
    int arr1 = start + 1;
    int arr2 = start + 2;
    int arr3 = start + 3;
    int i0 = 0; int i1 = 0; int i2 = 0; int i3 = 0;
    int channel_counter = 0;
    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            final_channels[k][channel_counter] = flat_arrays[arr0][i0];
            channel_counter++;
            final_channels[k][channel_counter] = flat_arrays[arr1][i1];
            channel_counter++;
            i0++;
            i1++;
        }
        for (int j = 0; j < DIM; j++)
        {
            final_channels[k][channel_counter] = flat_arrays[arr2][i2];
            channel_counter++;
            final_channels[k][channel_counter] = flat_arrays[arr3][i3];
            channel_counter++;
            i2++;
            i3++;
        }
    }
}

void pixelshuffle(float arr[1][12][8][8], float final_arr[1][3][16][16])
{
    int count = 0;

    std::vector<std::thread> ThreadVector;

    // flatten 12 channels throuth threading
    for (int i = 0; i < CH; i++)
    {
        std::thread t(flatten_array, i);
        ThreadVector.push_back(std::move(t));
    }
    for (auto &t : ThreadVector)
        t.join();


    count = 0;

    //combine channels
    std::vector<std::thread> ThreadVector2;
    for (int i = 0; i < 3; i++)
    {
        //process_4_channels(count, i);
        std::thread t(process_4_channels, count, i);
        ThreadVector2.push_back(std::move(t));
        count += 4;
    }
    for (auto &t : ThreadVector2)
        t.join();

    std::vector<float> final_flat = final_channels[0];
    final_flat.insert(final_flat.end(), final_channels[1].begin(), final_channels[1].end());
    final_flat.insert(final_flat.end(), final_channels[2].begin(), final_channels[2].end());


    //construct final arr
    count = 0;
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < (DIM * 2); k++)
            {
                for (int z = 0; z < (DIM * 2); z++)
                {
                    final_arr[i][j][k][z] = final_flat[count];
                    count ++;
                }
            }
        }
    }
    //print array
    /*
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
    */
}

int main(){
    int count = 0;
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
    return 0;
}