#include <cmath>
#include <algorithm> // Include <algorithm> for min and max functions
using namespace std;
struct RGB {
    unsigned char Red;
    unsigned char Blue;
    unsigned char Green;
};

void Sobel_algorithm(int32_t height , int32_t width, RGB** image )
{
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Create a new buffer to store the result
    RGB** result = new RGB*[height];
    for(int i = 0; i < height; ++i)
        result[i] = new RGB[width];

    // Iterate over the image pixels in a single loop
    for (int y = 1; y < height-1 ; y++) {
        for (int x = 1; x < width-1 ; x++) {
            // Initialize the gradient components for each color channel
            int gx_Red = 0;
            int gx_Blue = 0;
            int gx_Green = 0;
            
            int gy_Red = 0;
            int gy_Blue = 0;
            int gy_Green = 0;

            // Apply the Sobel filter kernel to the neighboring pixels
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int pixelX = x + j;
                    int pixelY = y + i;
                    
                    if (pixelX >= 0 && pixelX < width && pixelY >= 0 && pixelY < height) {
                        int weight_x = gx[i + 1][j + 1];
                        gx_Red += weight_x * image[pixelY][pixelX].Red;
                        gx_Blue += weight_x * image[pixelY][pixelX].Blue;
                        gx_Green += weight_x * image[pixelY][pixelX].Green;

                        int weight_y = gy[i + 1][j + 1];
                        gy_Red += weight_y * image[pixelY][pixelX].Red;
                        gy_Blue += weight_y * image[pixelY][pixelX].Blue;
                        gy_Green += weight_y * image[pixelY][pixelX].Green;
                    }
                }
            }
            
            // Calculate the absolute value of the gradient components using abs function
            int total_Red = sqrt(pow(gx_Red,2)+ pow(gy_Red,2));
            int total_Blue = sqrt(pow(gx_Blue,2)+ pow(gy_Blue,2));
            int total_Green = sqrt(pow(gx_Green,2)+ pow(gy_Green,2));

            // Clamp the color values between 0 and 255 using min and max functions
            total_Red = min(255, max(0, total_Red));
            total_Green = min(255, max(0, total_Green));
            total_Blue = min(255, max(0, total_Blue));

            // Update the color values of the current pixel in the result buffer
            result[y][x].Red = total_Red;
            result[y][x].Green = total_Green;
            result[y][x].Blue = total_Blue;
        }
    }

    // Copy the result buffer back to the original image
    for (int y = 1; y < height-1 ; y++) {
        for (int x = 1; x < width-1 ; x++) {
            image[y][x] = result[y][x];
        }
    }

    // Don't forget to delete the result buffer when you're done!
    for(int i = 0; i < height; ++i)
        delete [] result[i];
    delete [] result;
}
