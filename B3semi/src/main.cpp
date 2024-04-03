//
// Created by Hitoshi Sauji
//

#include "myImage.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <fstream>

// sh build_run.sh で実行
using namespace std;
int main()
{
    /*ここにmain関数の処理を記述する*/
    ImageMat color_img = read_image("img/cube.bmp");

    auto gray_img = color2gray(color_img);
    auto edge_img = extract_edge(gray_img);
    auto edge_color = make_color_edge_image(edge_img);
    // auto hough_img = hough_transform(edge_img[1], 80);
    write_image(edge_color[0], "result/road_hough_transformation.bmp");

    return 0;
}