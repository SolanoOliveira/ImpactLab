#include <iostream> 
#include <fstream>

using namespace std;

int main() {
    ofstream image;
    image.open("giraffe_p3.ppm");

    int cor = 255;

    if (image.is_open()) {
        image << "P3" << endl;         
        image << "250 250" << endl;     
        image << "255 " << endl;          
    }

    //across the image
    for(int y = 0; y<250; y++){
        if(y > 125){
           cor = 0; 
        }
     for(int x = 0; x<250; x++){
        image << cor <<  " " << cor << " " << cor << endl;
    }   
    }

    image.close();

    return 0;
}
