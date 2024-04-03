//
// Created by Hitoshi Sauji
//

#include "../myImage.hpp"

// 角度未定義
static const double NO_ANG = -1.0;

// エッジ抽出
/**
 * @param gray_img 1チャンネル画像
 * @return edge_data[0]: 1チャンネル エッジ角度数値データ、edge_data[1]: 1チャンネル エッジ強度数値データ
 */
std::array<ImageMatD, 2> extract_edge(const ImageMat &gray_image) // 例はprewitt
{

    ImageMatD edge_angle, edge_strength;
    double prewitt_row[3][3] = {{-1, 0, 1},
                                {-1, 0, 1},
                                {-1, 0, 1}};

    double prewitt_col[3][3] = {{-1, -1, -1},
                                {0, 0, 0},
                                {1, 1, 1}};

    // クラスの初期値を設定
    edge_angle.m_header = edge_strength.m_header = gray_image.m_header;
    edge_angle.m_width = edge_strength.m_width = gray_image.m_width;
    edge_angle.m_height = edge_strength.m_height = gray_image.m_height;
    edge_angle.m_ch = edge_strength.m_ch = gray_image.m_ch;
    edge_angle.m_length = edge_strength.m_length = gray_image.m_length;
    edge_angle.m_pixels.resize(edge_angle.m_length);
    edge_strength.m_pixels.resize(edge_strength.m_length);
    edge_angle.m_pixels.assign(edge_angle.m_length, NO_ANG); // 配列の初期化
    edge_strength.m_pixels.assign(edge_strength.m_length, 0);

    for (int y = 1; y < gray_image.m_height - 1; y++)
    {
        for (int x = 1; x < gray_image.m_width - 1; x++)
        {
            double dx, dy; // 各方向にフィルタをかけたときの差分
            dx = dy = 0;

            for (int col = -1; col <= 1; col++)
            {
                for (int row = -1; row <= 1; row++)
                {
                    dx += gray_image.m_pixels[(x + row) + (y + col) * gray_image.m_width] * prewitt_row[row + 1][col + 1];
                    dy += gray_image.m_pixels[(x + row) + (y + col) * gray_image.m_width] * prewitt_col[row + 1][col + 1];
                }
            }

            auto gradient_strength = sqrt((double)(dx * dx + dy * dy)); // エッジ強度

            int threshold = 80; // エッジ強度をしきい値にすることで画像を2値化
            if (gradient_strength >= threshold)
            {
                auto angle = ((double)atan2(dy, dx) + M_PI); // エッジ角度
                edge_angle.setPixel(x, y, 0, angle);
                edge_strength.setPixel(x, y, 0, 255);
            }
            else
            {
                edge_strength.setPixel(x, y, 0, 0);
            }
        }
    }

    // エッジ角度の正規化
    const auto max_angle = *max_element(edge_angle.m_pixels.begin(), edge_angle.m_pixels.end());
    const auto min_angle = *min_element(edge_angle.m_pixels.begin(), edge_angle.m_pixels.end());
    for (int y = 0; y < edge_angle.m_height; y++)
    {
        for (int x = 0; x < edge_angle.m_width; x++)
        {
            double norm = (edge_angle.getPixel(x, y, 0) - min_angle) / (max_angle - min_angle) * 255.0;
            edge_angle.setPixel(x, y, 0, norm);
        }
    }

    std::array<ImageMatD, 2> edge_data = {edge_angle, edge_strength};

    return edge_data;
}

std::array<ImageMat, 2> make_color_edge_image(const std::array<ImageMatD, 2> edge_data)
{
    // エッジ角度を色相としたHLS画像作成
    // 輝度
    ImageMat lightness, saturation;
    lightness = saturation = double2uint8t(edge_data[0]); // ヘッダ情報をコピーする

    lightness.m_pixels.assign(lightness.m_length, 0); // ピクセル値を初期化
    lightness.m_header = saturation.m_header = edge_data[0].m_header;
    for (int y = 0; y < lightness.m_height; y++)
    {
        for (int x = 0; x < lightness.m_width; x++)
        {
            lightness.setPixel(x, y, 0, (int)(edge_data[1].getPixel(x, y, 0) * 128)); // エッジ強度から計算, 正規化周り怪しい気がするから確認したい．
        }
    }

    // 彩度
    saturation.m_pixels.assign(saturation.m_length, 255);

    // double -> intへ変換
    auto angle_int = double2uint8t(edge_data[0]);
    auto strength_int = double2uint8t(edge_data[1]);

    // 3つの1チャンネル画像を1つの3チャンネル画像に統合
    auto merge_angle = merge_Channel(angle_int, lightness, saturation);
    auto merge_strength = merge_Channel(strength_int, lightness, saturation);

    // HLS色空間->RGB色空間
    merge_angle = hls2bgr(merge_angle);
    merge_strength = hls2bgr(merge_strength);

    std::array<ImageMat, 2> color_edge_data = {merge_angle, merge_strength};

    return color_edge_data;
}