#include <iostream>
#include <fstream>
int main(){
    std::fstream file("NIC.bmp", std::ios::binary|std::ios::in|std::ios::out);
    
    if (!file)
    {
        std::cout<<"error"<<std::endl;
        exit(1);
    }
    uint8_t magic[2];
    file.read(reinterpret_cast<char*>(magic), 2);
    if (magic[0]=='B'&&magic[1]=='M')
    {
        std::cout<<"it is a bmp file...\n";
    }
    else
    {
        std::cout<<"it not a bmp file...\n";
    }
    //jump to postiion 30 in the file
    file.seekg(30, std::ios::beg);
    uint32_t compress;
    file.read(reinterpret_cast<char*>(&compress), 4);
    if (compress==0)
    {
        std::cout<<"it is not compressed \n";
    }
    else
    {
        std::cout<<"it def compressed...\n";
    }

    //jump to position that says when pixel array starts
    file.seekg(10, std::ios::beg);
    uint32_t offset;
    file.read(reinterpret_cast<char*>(&offset), 4);
    std::cout<<"the pixel array starts at byte: "<<offset<<std::endl;

    //find the width of the array
    file.seekg(18, std::ios::beg);
    uint32_t width;
    file.read(reinterpret_cast<char*>(&width), 4);
    std::cout<<"the width is: "<<width<<std::endl; 

    //override the first row of the array with a different color
    file.seekg(offset, std::ios::beg);
    uint8_t color[3]={202, 21, 219};
    for (unsigned int i = 0; i< width; i++)
    {
        file.write(reinterpret_cast<char*>(color), 3);
    }

    //flip lsb of last row in pizel array
    //go to size of raw data, then 
    //divide that size by number or rows to get amount of bytes per row
    file.seekg(34, std::ios::beg);
    uint32_t totalBytes;
    file.read(reinterpret_cast<char*>(&totalBytes), 4);
    long int bytesPerRow;
    bytesPerRow = totalBytes/width;
    std::cout<<"the amount of bytes in a row: "<<bytesPerRow<<std::endl;

    //need to advance from start of pizel array to start of last row
    file.seekg(-1*bytesPerRow, std::ios::end);
    uint8_t byte;
    for (int i=0; i<bytesPerRow; i++)
    {
        //read a byte
        file.read(reinterpret_cast<char*>(&byte), 1);
        //flip lsb
        byte = byte^1;
        //write the byte
        file.seekg(-1, std::ios::cur);
        file.write(reinterpret_cast<char*>(&byte), 1);
    }
    
    file.close();


    
    return 0;
}

