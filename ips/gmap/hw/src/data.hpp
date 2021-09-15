/*
 * Fred hardware accelerator stub.
 *
 * Copyright (C) 2019, Marco Pagani, ReTiS Lab.
 * <marco.pag(at)outlook.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#ifndef DATA_HPP
#define DATA_HPP

const unsigned int gradient_map[256] = {
	0x532FC1, 0x532FC0, 0x5430BF, 0x5530BE, 0x5531BD, 0x5632BD, 0x5732BC, 0x5733BB,
	0x5834BA, 0x5934BA, 0x5935B9, 0x5A36B8, 0x5B36B7, 0x5B37B7, 0x5C38B6, 0x5D38B5,
	0x5D39B4, 0x5E3AB4, 0x5F3AB3, 0x5F3BB2, 0x603CB1, 0x613CB1, 0x613DB0, 0x623EAF,
	0x633EAE, 0x633FAE, 0x643FAD, 0x6540AC, 0x6541AB, 0x6641AB, 0x6742AA, 0x6743A9,
	0x6843A8, 0x6944A8, 0x6945A7, 0x6A45A6, 0x6B46A5, 0x6B47A5, 0x6C47A4, 0x6D48A3,
	0x6D49A2, 0x6E49A2, 0x6F4AA1, 0x6F4BA0, 0x704B9F, 0x714C9F, 0x714D9E, 0x724D9D,
	0x734E9C, 0x734E9C, 0x744F9B, 0x75509A, 0x755099, 0x765199, 0x775298, 0x775297,
	0x785396, 0x795496, 0x795495, 0x7A5594, 0x7B5693, 0x7B5693, 0x7C5792, 0x7D5891,
	0x7E5890, 0x7E598F, 0x7F5A8F, 0x805A8E, 0x805B8D, 0x815C8C, 0x825C8C, 0x825D8B,
	0x835D8A, 0x845E89, 0x845F89, 0x855F88, 0x866087, 0x866186, 0x876186, 0x886285,
	0x886384, 0x896383, 0x8A6483, 0x8A6582, 0x8B6581, 0x8C6680, 0x8C6780, 0x8D677F,
	0x8E687E, 0x8E697D, 0x8F697D, 0x906A7C, 0x906B7B, 0x916B7A, 0x926C7A, 0x926C79,
	0x936D78, 0x946E77, 0x946E77, 0x956F76, 0x967075, 0x967074, 0x977174, 0x987273,
	0x987272, 0x997371, 0x9A7471, 0x9A7470, 0x9B756F, 0x9C766E, 0x9C766E, 0x9D776D,
	0x9E786C, 0x9E786B, 0x9F796B, 0xA07A6A, 0xA07A69, 0xA17B68, 0xA27B68, 0xA27C67,
	0xA37D66, 0xA47D65, 0xA47E65, 0xA57F64, 0xA67F63, 0xA68062, 0xA78162, 0xA88161,
	0xA98260, 0xA9835F, 0xAA835E, 0xAB845E, 0xAB855D, 0xAC855C, 0xAD865B, 0xAD875B,
	0xAE875A, 0xAF8859, 0xAF8958, 0xB08958, 0xB18A57, 0xB18A56, 0xB28B55, 0xB38C55,
	0xB38C54, 0xB48D53, 0xB58E52, 0xB58E52, 0xB68F51, 0xB79050, 0xB7904F, 0xB8914F,
	0xB9924E, 0xB9924D, 0xBA934C, 0xBB944C, 0xBB944B, 0xBC954A, 0xBD9649, 0xBD9649,
	0xBE9748, 0xBF9847, 0xBF9846, 0xC09946, 0xC19945, 0xC19A44, 0xC29B43, 0xC39B43,
	0xC39C42, 0xC49D41, 0xC59D40, 0xC59E40, 0xC69F3F, 0xC79F3E, 0xC7A03D, 0xC8A13D,
	0xC9A13C, 0xC9A23B, 0xCAA33A, 0xCBA33A, 0xCBA439, 0xCCA538, 0xCDA537, 0xCDA637,
	0xCEA736, 0xCFA735, 0xCFA834, 0xD0A834, 0xD1A933, 0xD1AA32, 0xD2AA31, 0xD3AB31,
	0xD4AC30, 0xD4AC2F, 0xD5AD2E, 0xD6AE2D, 0xD6AE2D, 0xD7AF2C, 0xD8B02B, 0xD8B02A,
	0xD9B12A, 0xDAB229, 0xDAB228, 0xDBB327, 0xDCB427, 0xDCB426, 0xDDB525, 0xDEB624,
	0xDEB624, 0xDFB723, 0xE0B722, 0xE0B821, 0xE1B921, 0xE2B920, 0xE2BA1F, 0xE3BB1E,
	0xE4BB1E, 0xE4BC1D, 0xE5BD1C, 0xE6BD1B, 0xE6BE1B, 0xE7BF1A, 0xE8BF19, 0xE8C018,
	0xE9C118, 0xEAC117, 0xEAC216, 0xEBC315, 0xECC315, 0xECC414, 0xEDC513, 0xEEC512,
	0xEEC612, 0xEFC611, 0xF0C710, 0xF0C80F, 0xF1C80F, 0xF2C90E, 0xF2CA0D, 0xF3CA0C,
	0xF4CB0C, 0xF4CC0B, 0xF5CC0A, 0xF6CD09, 0xF6CE09, 0xF7CE08, 0xF8CF07, 0xF8D006,
	0xF9D006, 0xFAD105, 0xFAD204, 0xFBD203, 0xFCD303, 0xFCD402, 0xFDD401, 0xFED500
};

#endif /* DATA_HPP */