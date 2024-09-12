#include <iostream> 
#include <fstream>

using namespace std;

int main() {
    ofstream image;
    image.open("giraffe_p3.ppm");

    int R = 255;
    int G = 0;
    int B = 0;

    if (image.is_open()) {
        image << "P3" << endl;         
        image << "250 250" << endl;     
        image << "255 " << endl;          
    }

    //across the image
    for(int y = 0; y<250; y++){
        if(y == 41){
           R = 255;
           G = 140;
           B = 0; 
        }
        else if(y == 82){
           R = 255;
           G = 237;
           B = 0; 
        }else if(y == 123){
             R = 0;
           G = 128;
           B = 38; 
        }
        else if(y == 164){
             R = 0;
           G = 76;
           B = 255; 
        }
         else if(y == 205){
             R = 115;
           G = 41;
           B = 130; 
        }
     for(int x = 0; x<250; x++){
        image << R <<  " " << G << " " << B << endl;
    }   
    }

    image.close();

    return 0;
}
