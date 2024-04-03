//
// Created by Hitoshi Sauji.
//

#include "../myImage.hpp"

// 画像の2値化処理
/**
 * @param gray_img 1チャンネル画像
 * @param threshold しきい値
 * @return 2値化画像
 */
ImageMat binarize(const ImageMat &gray_img, const int &threshold)
{
    ImageMat binarize_img = gray_img;

    int black = 0;
    int white = 255;

    for (int i = 0; i < gray_img.m_length; i++)
    {
        if (gray_img.m_pixels[i] >= threshold)
            binarize_img.m_pixels[i] = white;
        else
            binarize_img.m_pixels[i] = black;
    }

    return binarize_img;
}

// 膨張処理
/**
 * @param binarize_img 2値化処理済み1チャンネル画像
 * @return 画像処理済1チャンネル画像
 */
ImageMat dilation(const ImageMat &binarize_img)
{
    ImageMat dilate_img = binarize_img;
    std::vector<int> pixels(9);
    int black = 0;
    int white = 255;

    // 処理を記述 中心が白い画素なら周囲8近傍をすべて白にする
    // 周囲1pxに対する処理を忘れずに

    for (int y = 0; y < binarize_img.m_height; y++)
    {
        for (int x = 0; x < binarize_img.m_width; x++)
        {
            pixels[0] = binarize_img.m_pixels[((x - 1) + (y - 1) * binarize_img.m_width)];
            pixels[1] = binarize_img.m_pixels[(x + (y - 1) * binarize_img.m_width)];
            pixels[2] = binarize_img.m_pixels[((x + 1) + (y - 1) * binarize_img.m_width)];
            pixels[3] = binarize_img.m_pixels[((x - 1) + y * binarize_img.m_width)];
            pixels[4] = binarize_img.m_pixels[(x + y * binarize_img.m_width)];
            pixels[5] = binarize_img.m_pixels[((x + 1) + y * binarize_img.m_width)];
            pixels[6] = binarize_img.m_pixels[((x - 1) + (y + 1) * binarize_img.m_width)];
            pixels[7] = binarize_img.m_pixels[(x + (y + 1) * binarize_img.m_width)];
            pixels[8] = binarize_img.m_pixels[((x + 1) + (y + 1) * binarize_img.m_width)];
            if (x == 0)
            {
                if (y == 0)
                { // 左下
                    if (pixels[5] || pixels[7] || pixels[8] == white)
                    { // <- ここ因果関係が逆で, 中心が白いなら周りを白くする，が正しいけど処理結果的には合ってるのでまあよし
                        dilate_img.m_pixels[x + y * dilate_img.m_width] = white;
                    }
                }
                else if (y == binarize_img.m_height - 1)
                { // 左上
                    if (pixels[1] || pixels[2] || pixels[5] == white)
                    {
                        dilate_img.m_pixels[x + y * dilate_img.m_width] = white;
                    }
                }
                else
                { // 左端
                    if (pixels[1] || pixels[2] || pixels[5] || pixels[7] || pixels[8] == white)
                    {
                        dilate_img.m_pixels[x + y * dilate_img.m_width] = white;
                    }
                }
            }
            else if (x == binarize_img.m_width - 1)
            {
                if (y == 0)
                { // 右下
                    if (pixels[3] || pixels[6] || pixels[7] == white)
                    {
                        dilate_img.m_pixels[x + y * dilate_img.m_width] = white;
                    }
                }
                else if (y == binarize_img.m_height - 1)
                { // 右上
                    if (pixels[0] || pixels[1] || pixels[3] == white)
                    {
                        dilate_img.m_pixels[x + y * dilate_img.m_width] = white;
                    }
                }
                else
                { // 右端
                    if (pixels[0] || pixels[1] || pixels[3] || pixels[6] || pixels[7] == white)
                    {
                        dilate_img.m_pixels[x + y * dilate_img.m_width] = white;
                    }
                }
            }
            else if (y == binarize_img.m_height - 1)
            { // 上端
                if (pixels[0] || pixels[1] || pixels[2] || pixels[3] || pixels[5] == white)
                {
                    dilate_img.m_pixels[x + y * dilate_img.m_width] = white;
                }
            }
            else if (y == 0)
            { // 下端
                if (pixels[3] || pixels[5] || pixels[6] || pixels[7] || pixels[8] == white)
                {
                    dilate_img.m_pixels[x + y * dilate_img.m_width] = white;
                }
            }
            else
            {
                if (pixels[0] || pixels[1] || pixels[2] || pixels[3] || pixels[4] || pixels[5] || pixels[6] || pixels[7] || pixels[8] == white)
                {
                    dilate_img.m_pixels[x + y * dilate_img.m_width] = white;
                }
            }
        }
    }
    return dilate_img;
}

// 収縮
/**
 * @param binarize_img 2値化処理済み1チャンネル画像
 * @return 画像処理済1チャンネル画像
 */
ImageMat erosion(const ImageMat &binarize_img)
{
    ImageMat erode_img = binarize_img;
    std::vector<int> pixels(9);
    int black = 0;
    int white = 255;

    // 処理を記述
    // 周囲1pxに対する処理を忘れずに

    for (int y = 0; y < binarize_img.m_height; y++)
    {
        for (int x = 0; x < binarize_img.m_width; x++)
        {
            pixels[0] = binarize_img.m_pixels[((x - 1) + (y - 1) * binarize_img.m_width)];
            pixels[1] = binarize_img.m_pixels[(x + (y - 1) * binarize_img.m_width)];
            pixels[2] = binarize_img.m_pixels[((x + 1) + (y - 1) * binarize_img.m_width)];
            pixels[3] = binarize_img.m_pixels[((x - 1) + y * binarize_img.m_width)];
            pixels[4] = binarize_img.m_pixels[(x + y * binarize_img.m_width)];
            pixels[5] = binarize_img.m_pixels[((x + 1) + y * binarize_img.m_width)];
            pixels[6] = binarize_img.m_pixels[((x - 1) + (y + 1) * binarize_img.m_width)];
            pixels[7] = binarize_img.m_pixels[(x + (y + 1) * binarize_img.m_width)];
            pixels[8] = binarize_img.m_pixels[((x + 1) + (y + 1) * binarize_img.m_width)];
            if (x == 0)
            {
                if (y == 0)
                { // 左下
                    if (pixels[5] || pixels[7] || pixels[8] == black)
                    {
                        erode_img.m_pixels[x + y * erode_img.m_width] = black;
                    }
                }
                else if (y == binarize_img.m_height - 1)
                { // 左上
                    if (pixels[1] || pixels[2] || pixels[5] == black)
                    {
                        erode_img.m_pixels[x + y * erode_img.m_width] = black;
                    }
                }
                else
                { // 左端
                    if (pixels[1] || pixels[2] || pixels[5] || pixels[7] || pixels[8] == black)
                    {
                        erode_img.m_pixels[x + y * erode_img.m_width] = black;
                    }
                }
            }
            else if (x == binarize_img.m_width - 1)
            {
                if (y == 0)
                { // 右下
                    if (pixels[3] || pixels[6] || pixels[7] == black)
                    {
                        erode_img.m_pixels[x + y * erode_img.m_width] = black;
                    }
                }
                else if (y == binarize_img.m_height - 1)
                { // 右上
                    if (pixels[0] || pixels[1] || pixels[3] == black)
                    {
                        erode_img.m_pixels[x + y * erode_img.m_width] = black;
                    }
                }
                else
                { // 右端
                    if (pixels[0] || pixels[1] || pixels[3] || pixels[6] || pixels[7] == black)
                    {
                        erode_img.m_pixels[x + y * erode_img.m_width] = black;
                    }
                }
            }
            else if (y == binarize_img.m_height - 1)
            { // 上端
                if (pixels[0] || pixels[1] || pixels[2] || pixels[3] || pixels[5] == black)
                {
                    erode_img.m_pixels[x + y * erode_img.m_width] = black;
                }
            }
            else if (y == 0)
            { // 下端
                if (pixels[3] || pixels[5] || pixels[6] || pixels[7] || pixels[8] == black)
                {
                    erode_img.m_pixels[x + y * erode_img.m_width] = black;
                }
            }
            else
            {
                if (pixels[0] || pixels[1] || pixels[2] || pixels[3] || pixels[4] || pixels[5] || pixels[6] || pixels[7] || pixels[8] == black)
                {
                    erode_img.m_pixels[x + y * erode_img.m_width] = black;
                }
            }
        }
    }
    return erode_img;
}