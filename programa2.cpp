#include <iostream> 
#include <fstream>

using namespace std;

int main() {

 ifstream image;

 ofstream new_image;

 image.open(flor.ppm);
 new_image.open(flor_blue.ppm);

 string type = "", width = "", height = "", RGB = "";

 image >> type;
 image >> width << "" << height << endl;
 image >> RGB;
 
 string red = "", green ="", blue ="";

 int int_red = 0, int green = 0, int blue = 0;

 //continue here 


}