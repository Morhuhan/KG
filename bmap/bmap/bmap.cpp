// 2018 ReVoL Primer Template
// bmap.cpp
#include "bmap.h"
// путь к файлу BMP
//#define PATH "..\\Debug\\ch01.bmp"      
//#define PATH "..\\Debug\\ch04.bmp"     
#define PATH "..\\Debug\\ch08.bmp"       
//#define PATH "..\\Debug\\ch24.bmp"      

// исследование BMP
void bitmap() {

    ///////////// Чтение файла

    FILE * f = fopen(PATH, "rb");

    if (!f) return;

    ///////////// Чтение заголовка файла

    tagBITMAPFILEHEADER bmfh;

    int res = fread((void*)&bmfh, sizeof(bmfh), 1, f);

    if (!res) return;

    //bfType bfSize bfReserved1 bfReserved2 bfOffBits
    //19778 94 0 0 62
    //19778 150 0 0 118
    //19778 1142 0 0 1078
    //19778 246 0 0 54

    ///////////// Чтение палитры из файла

    // Размер BITMAPINFO + Палитры
    int cbmi = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);

    // Создали динамический массив из BITMAPINFO, так как BITMAPINFO хранит только 1 элемент палитры
    BITMAPINFO* pbmi = (BITMAPINFO*)LocalAlloc(LPTR, cbmi);

    // Читаем палитру из файла
    res = fread((void*)pbmi, cbmi, 1, f);

    if (!res) return;

    //RGBQUAD* ppal = &pbmi->bmiColors[0];

    ///////////// Чтение набора пискелей

    // Массив писельного набора
    BYTE* dibs = (BYTE*)LocalAlloc(LPTR, pbmi->bmiHeader.biSizeImage);

    // Читаем пискельный набор
    res = fread((void*)dibs, pbmi->bmiHeader.biSizeImage, 1, f);

    //HDC CDC = GetDC(hWndMain);

    // Создаем картинку
    //HBMP = CreateDIBitmap(DCBMP, &pbmi->bmiHeader, CBM_INIT, dibs, pbmi, DIB_RGB_COLORS);

    SelectObject(DCBMP, HBMP);

    int BMPX = pbmi->bmiHeader.biWidth;
    int BMPY = pbmi->bmiHeader.biHeight;

    RGBQUAD* ppal = &pbmi->bmiColors[0];
    pbmi->bmiColors[0].rgbRed = 255;
    pbmi->bmiColors[0].rgbGreen = 255;
    pbmi->bmiColors[0].rgbBlue = 0;

    StretchDIBits(DCBMP, 0, 0, 200, 200, 0, 0, BMPX, BMPY, dibs, pbmi, DIB_RGB_COLORS, SRCCOPY);

    fclose(f);
}

// исследование метафайла
void metafile() {
}
