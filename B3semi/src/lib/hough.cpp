//
// Created by Hitoshi Sauji.
//

#include "../myImage.hpp"

// hough変換の流れ
// color2gray->extraction_edge->double2uint8t(edge_strength)->gray2color(edge_strength)->hough(edge_strength)

/**
 * @param edge_strength エッジ強度のデータ
 * @param threshold エッジ強度のしきい値
 * @return hough_img[0]: 直線検出済み画像、hough_img[1]: 可視化済み投票空間画像
 */
std::array<ImageMat, 2> hough_transform(const ImageMatD &edge_strength, const int &threshold)
{
    ImageMatI hough_space; // 投票処理が入るのでuint8_tだと255以上に対応できない
    hough_space.m_header = edge_strength.m_header;
    hough_space.m_width = 180;
    hough_space.m_height = 2 * (int)sqrt(std::pow(edge_strength.m_width, 2.0) + std::pow(edge_strength.m_height, 2.0));
    hough_space.m_ch = 3;
    hough_space.m_length = hough_space.m_height * hough_space.m_width * hough_space.m_ch;
    hough_space.m_pixels.resize(hough_space.m_length);
    hough_space.m_pixels.assign(hough_space.m_length, 0); // 配列の初期化
    std::cout << "hough_space... width: " << hough_space.m_width << ", height: " << hough_space.m_height << std::endl;

    for (int y = 0; y < edge_strength.m_height; y++)
    {
        for (int x = 0; x < edge_strength.m_width; x++)
        {
            if ((int)edge_strength.getPixel(x, y, 0) > threshold)
                hough_space = vote_hough_space(hough_space, x, y);
        }
    }

    // hough空間の投票結果を画像として出力できる形式に変える
    ImageMat hough_space_img = output_hough_space(hough_space);

    // クラスの型とチャンネル数を変える
    ImageMat edge_strength_uint = double2uint8t(edge_strength);
    edge_strength_uint = gray2color(edge_strength_uint);

    // 投票で決定した点に直線を引く
    // auto hough_line_img = draw_line_using_parametric_variable(hough_space, edge_strength_uint);
    auto hough_line_img = draw_line(hough_space, edge_strength_uint);

    // リスト形式にして返すための処理
    std::array<ImageMat, 2> hough_img;
    hough_img[0] = hough_line_img;
    hough_img[1] = hough_space_img;
    return hough_img;
}

// hough空間に対する投票
ImageMatI vote_hough_space(const ImageMatI &hough_space, const int &x, const int &y)
{
    ImageMatI after_vote_hough_space = hough_space;

    for (int theta = 0; theta < 180; theta++)
    {
        double radian = theta * M_PI / 180.0;
        auto rho = (int)(x * cos(radian) + y * sin(radian));
        rho += hough_space.m_height / 2; // rhoを正に戻す処理
        const auto vote = after_vote_hough_space.getPixel(theta, rho, 0) + 1;
        after_vote_hough_space.setPixel(theta, rho, 0, vote);
        after_vote_hough_space.setPixel(theta, rho, 1, vote);
        after_vote_hough_space.setPixel(theta, rho, 2, vote);
    }

    return after_vote_hough_space;
}

// houth空間中の投票値の大小を画像に表示する
ImageMat output_hough_space(const ImageMatI &hough_space_img)
{
    ImageMat output_img;
    output_img.m_header = hough_space_img.m_header;
    output_img.m_height = hough_space_img.m_height;
    output_img.m_width = hough_space_img.m_width;
    output_img.m_length = hough_space_img.m_height * hough_space_img.m_width * hough_space_img.m_ch;
    output_img.m_pixels.resize(output_img.m_length);

    auto max_vote = *std::max_element(hough_space_img.m_pixels.begin(), hough_space_img.m_pixels.end());
    std::cout << "max_vote: " << max_vote << std::endl;

    for (int y = 0; y < hough_space_img.m_height; y++) // 正規化して再セット、0~255に収める
    {
        for (int x = 0; x < hough_space_img.m_width; x++)
        {
            output_img.setPixel(x, y, 0, (uint8_t)((double)(hough_space_img.getPixel(x, y, 0)) / (double)(max_vote) * 255));
            output_img.setPixel(x, y, 1, (uint8_t)((double)(hough_space_img.getPixel(x, y, 1)) / (double)(max_vote) * 255));
            output_img.setPixel(x, y, 2, (uint8_t)((double)(hough_space_img.getPixel(x, y, 2)) / (double)(max_vote) * 255));
        }
    }
    return output_img;
}

// 直線を入力画像に引いた画像を作り、返す
ImageMat draw_line(const ImageMatI &hough_image, const ImageMat &edge_strength)
{
    ImageMat line_image = edge_strength;
    auto max_vote = *std::max_element(hough_image.m_pixels.begin(), hough_image.m_pixels.end());

    for (int hough_y = 0; hough_y < hough_image.m_height; hough_y++) // hough空間内の探索
    {
        for (int hough_x = 0; hough_x < hough_image.m_width; hough_x++)
        {
            int theta = hough_x;
            int rho = hough_y - hough_image.m_height / 2; // y座標のオフセットの分を引く
            double radian = theta * M_PI / 180.0;

            if (hough_image.getPixel(hough_x, hough_y, 0) <= max_vote * 0.7) // 投票数が一定数以下
                continue;

            // 直線を描く処理
            int bi_x, bi_y;
            int new_index;
            if (10 < theta && theta < 170)
            {
                for (bi_x = 0; bi_x < edge_strength.m_width; bi_x++)
                {
                    double t = 1 - cos(radian);
                    bi_y = (int)((double)((-cos(radian) * bi_x + rho) / sin(radian)));
                    new_index = (bi_x + bi_y * edge_strength.m_width) * 3;

                    line_image.setPixel(bi_x, bi_y, 0, 0);
                    line_image.setPixel(bi_x, bi_y, 1, 0);
                    line_image.setPixel(bi_x, bi_y, 2, 255);
                }
            }
            else if (theta == 0)
            {
                for (bi_y = 0; bi_y < edge_strength.m_height; bi_y++)
                {
                    bi_x = rho;
                    new_index = (bi_x + bi_y * edge_strength.m_width) * 3;

                    line_image.setPixel(bi_x, bi_y, 0, 0);
                    line_image.setPixel(bi_x, bi_y, 1, 0);
                    line_image.setPixel(bi_x, bi_y, 2, 255);
                }
            }
            else
            {
                for (bi_y = 0; bi_y < edge_strength.m_height; bi_y++)
                {
                    bi_x = (int)((double)((-sin(radian) * bi_y + rho) / cos(radian)));
                    new_index = (bi_x + bi_y * edge_strength.m_width) * 3;

                    line_image.setPixel(bi_x, bi_y, 0, 0);
                    line_image.setPixel(bi_x, bi_y, 1, 0);
                    line_image.setPixel(bi_x, bi_y, 2, 255);
                }
            }
        }
    }

    return line_image;
}

ImageMat draw_line_using_parametric_variable(const ImageMatI &hough_image, const ImageMat &edge_strength)
{
    ImageMat line_image = edge_strength;
    auto max_vote = *std::max_element(hough_image.m_pixels.begin(), hough_image.m_pixels.end());

    for (int hough_y = 0; hough_y < hough_image.m_height; hough_y++) // hough空間内の探索
    {
        for (int hough_x = 0; hough_x < hough_image.m_width; hough_x++)
        {
            int theta = hough_x;
            int rho = hough_y - hough_image.m_height / 2; // y座標のオフセットの分を引く
            double radian = theta * M_PI / 180.0;

            if (hough_image.getPixel(hough_x, hough_y, 0) <= max_vote * 0.7) // 投票数が一定数以下
                continue;

            if (!is_max_value_in_filter(hough_image, hough_x, hough_y, 5, 5)) // hough空間内のフィルタ内で極大値か？を調べる
                continue;

            // 直線上のある点の座標(x0, y0)を求める
            double x0, y0;
            x0 = rho * cos(radian);
            y0 = rho * sin(radian);

            // 直行するベクトルを求める
            //  vec_x = sin(theta);vec_y = -cos(theta);でもよい, thetaを90°回転させた時の三角関数の関係を利用
            double vec_x, vec_y;
            vec_x = -sin(radian);
            vec_y = cos(radian);

            // 媒介変数の最大値を定義
            int max_t = hough_image.m_height / 2;

            // tの範囲を求める
            int bi_x, bi_y;
            for (int t = -max_t; t <= max_t; t++)
            {
                bi_x = x0 + vec_x * t;
                bi_y = y0 + vec_y * t;

                // 値が画像空間内に収まっていれば直線を描画
                if (bi_y >= 0 && bi_y < edge_strength.m_height)
                {
                    if (bi_x >= 0 && bi_x < edge_strength.m_width)
                    {
                        line_image.setPixel(bi_x, bi_y, 0, 0);
                        line_image.setPixel(bi_x, bi_y, 1, 0);
                        line_image.setPixel(bi_x, bi_y, 2, 255);
                    }
                }
            }
        }
    }

    return line_image;
}

bool is_max_value_in_filter(const ImageMatI &hough_image, int hough_x, int hough_y, int theta_range, int rho_range)
{
    int theta, rho;
    int max_value = -1;

    for (int y = hough_y - rho_range; y <= hough_y + rho_range; y++)
    {
        for (int x = hough_x - theta_range; x <= hough_x + theta_range; x++)
        {
            if (x < 0 || x >= 180)
            {
                theta = (x + 180) % 180;
                rho = (hough_image.m_height) - y;
            }
            else
            {
                rho = y;
                theta = x;
            }

            // rhoがhough空間内に収まっているか？
            if (rho < 0 || rho > hough_image.m_height)
                continue;

            int pixel_value = hough_image.getPixel(theta, rho, 0);
            if (pixel_value > max_value)
                max_value = pixel_value;
        }
    }
    return hough_image.getPixel(hough_x, hough_y, 0) >= max_value;
}