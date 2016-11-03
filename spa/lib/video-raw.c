/* Simple Plugin API
 * Copyright (C) 2016 Wim Taymans <wim.taymans@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <spa/video/raw.h>
#include <spa/video/format.h>

#include "props.h"

static const uint32_t format_values[] = {
  SPA_VIDEO_FORMAT_UNKNOWN,
  SPA_VIDEO_FORMAT_ENCODED,
  SPA_VIDEO_FORMAT_I420,
  SPA_VIDEO_FORMAT_YV12,
  SPA_VIDEO_FORMAT_YUY2,
  SPA_VIDEO_FORMAT_UYVY,
  SPA_VIDEO_FORMAT_AYUV,
  SPA_VIDEO_FORMAT_RGBx,
  SPA_VIDEO_FORMAT_BGRx,
  SPA_VIDEO_FORMAT_xRGB,
  SPA_VIDEO_FORMAT_xBGR,
  SPA_VIDEO_FORMAT_RGBA,
  SPA_VIDEO_FORMAT_BGRA,
  SPA_VIDEO_FORMAT_ARGB,
  SPA_VIDEO_FORMAT_ABGR,
  SPA_VIDEO_FORMAT_RGB,
  SPA_VIDEO_FORMAT_BGR,
  SPA_VIDEO_FORMAT_Y41B,
  SPA_VIDEO_FORMAT_Y42B,
  SPA_VIDEO_FORMAT_YVYU,
  SPA_VIDEO_FORMAT_Y444,
  SPA_VIDEO_FORMAT_v210,
  SPA_VIDEO_FORMAT_v216,
  SPA_VIDEO_FORMAT_NV12,
  SPA_VIDEO_FORMAT_NV21,
  SPA_VIDEO_FORMAT_GRAY8,
  SPA_VIDEO_FORMAT_GRAY16_BE,
  SPA_VIDEO_FORMAT_GRAY16_LE,
  SPA_VIDEO_FORMAT_v308,
  SPA_VIDEO_FORMAT_RGB16,
  SPA_VIDEO_FORMAT_BGR16,
  SPA_VIDEO_FORMAT_RGB15,
  SPA_VIDEO_FORMAT_BGR15,
  SPA_VIDEO_FORMAT_UYVP,
  SPA_VIDEO_FORMAT_A420,
  SPA_VIDEO_FORMAT_RGB8P,
  SPA_VIDEO_FORMAT_YUV9,
  SPA_VIDEO_FORMAT_YVU9,
  SPA_VIDEO_FORMAT_IYU1,
  SPA_VIDEO_FORMAT_ARGB64,
  SPA_VIDEO_FORMAT_AYUV64,
  SPA_VIDEO_FORMAT_r210,
  SPA_VIDEO_FORMAT_I420_10BE,
  SPA_VIDEO_FORMAT_I420_10LE,
  SPA_VIDEO_FORMAT_I422_10BE,
  SPA_VIDEO_FORMAT_I422_10LE,
  SPA_VIDEO_FORMAT_Y444_10BE,
  SPA_VIDEO_FORMAT_Y444_10LE,
  SPA_VIDEO_FORMAT_GBR,
  SPA_VIDEO_FORMAT_GBR_10BE,
  SPA_VIDEO_FORMAT_GBR_10LE,
  SPA_VIDEO_FORMAT_NV16,
  SPA_VIDEO_FORMAT_NV24,
  SPA_VIDEO_FORMAT_NV12_64Z32,
  SPA_VIDEO_FORMAT_A420_10BE,
  SPA_VIDEO_FORMAT_A420_10LE,
  SPA_VIDEO_FORMAT_A422_10BE,
  SPA_VIDEO_FORMAT_A422_10LE,
  SPA_VIDEO_FORMAT_A444_10BE,
  SPA_VIDEO_FORMAT_A444_10LE,
  SPA_VIDEO_FORMAT_NV61,
  SPA_VIDEO_FORMAT_P010_10BE,
  SPA_VIDEO_FORMAT_P010_10LE,
  SPA_VIDEO_FORMAT_IYU2,
};

static const SpaPropRangeInfo format_range[] = {
  { "ENCODED", { sizeof (uint32_t), &format_values[1] } },
  { "I420", { sizeof (uint32_t), &format_values[2] } },
  { "YV12", { sizeof (uint32_t), &format_values[3] } },
  { "YUY2", { sizeof (uint32_t), &format_values[4] } },
  { "UYVY", { sizeof (uint32_t), &format_values[5] } },
  { "AYUV", { sizeof (uint32_t), &format_values[6] } },
  { "RGBx", { sizeof (uint32_t), &format_values[7] } },
  { "BGRx", { sizeof (uint32_t), &format_values[8] } },
  { "xRGB", { sizeof (uint32_t), &format_values[9] } },
  { "xBGR", { sizeof (uint32_t), &format_values[10] } },
  { "RGBA", { sizeof (uint32_t), &format_values[11] } },
  { "BGRA", { sizeof (uint32_t), &format_values[12] } },
  { "ARGB", { sizeof (uint32_t), &format_values[13] } },
  { "ABGR", { sizeof (uint32_t), &format_values[14] } },
  { "RGB", { sizeof (uint32_t), &format_values[15] } },
  { "BGR", { sizeof (uint32_t), &format_values[16] } },
  { "Y41B", { sizeof (uint32_t), &format_values[17] } },
  { "Y42B", { sizeof (uint32_t), &format_values[18] } },
  { "YVYU", { sizeof (uint32_t), &format_values[19] } },
  { "Y444", { sizeof (uint32_t), &format_values[20] } },
  { "v210", { sizeof (uint32_t), &format_values[21] } },
  { "v216", { sizeof (uint32_t), &format_values[22] } },
  { "NV12", { sizeof (uint32_t), &format_values[23] } },
  { "NV21", { sizeof (uint32_t), &format_values[24] } },
  { "GRAY8", { sizeof (uint32_t), &format_values[25] } },
  { "GRAY16_BE", { sizeof (uint32_t), &format_values[26] } },
  { "GRAY16_LE", { sizeof (uint32_t), &format_values[27] } },
  { "v308", { sizeof (uint32_t), &format_values[28] } },
  { "RGB16", { sizeof (uint32_t), &format_values[29] } },
  { "BGR16", { sizeof (uint32_t), &format_values[30] } },
  { "RGB15", { sizeof (uint32_t), &format_values[31] } },
  { "BGR15", { sizeof (uint32_t), &format_values[32] } },
  { "UYVP", { sizeof (uint32_t), &format_values[33] } },
  { "A420", { sizeof (uint32_t), &format_values[34] } },
  { "RGB8P", { sizeof (uint32_t), &format_values[35] } },
  { "YUV9", { sizeof (uint32_t), &format_values[36] } },
  { "YVU9", { sizeof (uint32_t), &format_values[37] } },
  { "IYU1", { sizeof (uint32_t), &format_values[38] } },
  { "ARGB64", { sizeof (uint32_t), &format_values[39] } },
  { "AYUV64", { sizeof (uint32_t), &format_values[40] } },
  { "r210", { sizeof (uint32_t), &format_values[41] } },
  { "I420_10BE", { sizeof (uint32_t), &format_values[42] } },
  { "I420_10LE", { sizeof (uint32_t), &format_values[43] } },
  { "I422_10BE", { sizeof (uint32_t), &format_values[44] } },
  { "I422_10LE", { sizeof (uint32_t), &format_values[45] } },
  { "I444_10BE", { sizeof (uint32_t), &format_values[46] } },
  { "I444_10LE", { sizeof (uint32_t), &format_values[47] } },
  { "GBR", { sizeof (uint32_t), &format_values[48] } },
  { "GBR_10BE", { sizeof (uint32_t), &format_values[49] } },
  { "GBR_10LE", { sizeof (uint32_t), &format_values[50] } },
  { "NV16", { sizeof (uint32_t), &format_values[51] } },
  { "NV24", { sizeof (uint32_t), &format_values[52] } },
  { "NV12_64Z32", { sizeof (uint32_t), &format_values[53] } },
  { "A420_10BE", { sizeof (uint32_t), &format_values[54] } },
  { "A420_10LE", { sizeof (uint32_t), &format_values[55] } },
  { "A422_10BE", { sizeof (uint32_t), &format_values[56] } },
  { "A422_10LE", { sizeof (uint32_t), &format_values[57] } },
  { "A444_10BE", { sizeof (uint32_t), &format_values[58] } },
  { "A444_10LE", { sizeof (uint32_t), &format_values[59] } },
  { "NV61", { sizeof (uint32_t), &format_values[60] } },
  { "P010_10BE", { sizeof (uint32_t), &format_values[61] } },
  { "P010_10LE", { sizeof (uint32_t), &format_values[62] } },
  { "IYU2", { sizeof (uint32_t), &format_values[63] } },
};

static const SpaRectangle min_size = { 1, 1 };
static const SpaRectangle max_size = { UINT32_MAX, UINT32_MAX };

static const SpaPropRangeInfo size_range[] = {
  { "min", { sizeof (SpaRectangle), &min_size } },
  { "max", { sizeof (SpaRectangle), &max_size } },
};

static const uint32_t interlace_modes[] = {
  SPA_VIDEO_INTERLACE_MODE_PROGRESSIVE,
  SPA_VIDEO_INTERLACE_MODE_INTERLEAVED,
  SPA_VIDEO_INTERLACE_MODE_MIXED,
  SPA_VIDEO_INTERLACE_MODE_FIELDS
};

static const SpaPropRangeInfo interlace_mode_range[] = {
  { "progressive", { sizeof (uint32_t), &interlace_modes[0] } },
  { "interleaved", { sizeof (uint32_t), &interlace_modes[1] } },
  { "mixed", { sizeof (uint32_t), &interlace_modes[2] } },
  { "fields", { sizeof (uint32_t), &interlace_modes[3] } },
};


static const uint32_t multiview_modes[] = {
  SPA_VIDEO_MULTIVIEW_MODE_NONE,
  SPA_VIDEO_MULTIVIEW_MODE_MONO,
  SPA_VIDEO_MULTIVIEW_MODE_LEFT,
  SPA_VIDEO_MULTIVIEW_MODE_RIGHT,
  SPA_VIDEO_MULTIVIEW_MODE_SIDE_BY_SIDE,
  SPA_VIDEO_MULTIVIEW_MODE_SIDE_BY_SIDE_QUINCUNX,
  SPA_VIDEO_MULTIVIEW_MODE_COLUMN_INTERLEAVED,
  SPA_VIDEO_MULTIVIEW_MODE_ROW_INTERLEAVED,
  SPA_VIDEO_MULTIVIEW_MODE_TOP_BOTTOM,
  SPA_VIDEO_MULTIVIEW_MODE_CHECKERBOARD,
  SPA_VIDEO_MULTIVIEW_MODE_FRAME_BY_FRAME,
  SPA_VIDEO_MULTIVIEW_MODE_MULTIVIEW_FRAME_BY_FRAME,
  SPA_VIDEO_MULTIVIEW_MODE_SEPARATED
};

static const SpaPropRangeInfo multiview_mode_range[] = {
  { "mono", { sizeof (uint32_t), &multiview_modes[1] } },
  { "left", { sizeof (uint32_t), &multiview_modes[2] } },
  { "right", { sizeof (uint32_t), &multiview_modes[3] } },
  { "side-by-side", { sizeof (uint32_t), &multiview_modes[4] } },
  { "side-by-side-quincunx", { sizeof (uint32_t), &multiview_modes[5] } },
  { "column-interleaved", { sizeof (uint32_t), &multiview_modes[6] } },
  { "row-interleaved", { sizeof (uint32_t), &multiview_modes[7] } },
  { "top-bottom", { sizeof (uint32_t), &multiview_modes[8] } },
  { "checkerboard", { sizeof (uint32_t), &multiview_modes[9] } },
  { "frame-by-frame", { sizeof (uint32_t), &multiview_modes[10] } },
  { "multiview-frame-by-frame", { sizeof (uint32_t), &multiview_modes[11] } },
  { "separated", { sizeof (uint32_t), &multiview_modes[12] } },
};

static const uint32_t multiview_flags[] = {
  SPA_VIDEO_MULTIVIEW_FLAGS_NONE,
  SPA_VIDEO_MULTIVIEW_FLAGS_RIGHT_VIEW_FIRST,
  SPA_VIDEO_MULTIVIEW_FLAGS_LEFT_FLIPPED,
  SPA_VIDEO_MULTIVIEW_FLAGS_LEFT_FLOPPED,
  SPA_VIDEO_MULTIVIEW_FLAGS_RIGHT_FLIPPED,
  SPA_VIDEO_MULTIVIEW_FLAGS_RIGHT_FLOPPED,
  SPA_VIDEO_MULTIVIEW_FLAGS_HALF_ASPECT,
  SPA_VIDEO_MULTIVIEW_FLAGS_MIXED_MONO,
};

static const SpaPropRangeInfo multiview_flags_range[] = {
  { "none", { sizeof (uint32_t), &multiview_flags[0] } },
  { "right-view-first", { sizeof (uint32_t), &multiview_flags[1] } },
  { "left-flipped", { sizeof (uint32_t), &multiview_flags[2] } },
  { "left-flopped", { sizeof (uint32_t), &multiview_flags[3] } },
  { "right-flipped", { sizeof (uint32_t), &multiview_flags[4] } },
  { "right-flopped", { sizeof (uint32_t), &multiview_flags[5] } },
  { "half-aspect", { sizeof (uint32_t), &multiview_flags[6] } },
  { "mixed-mono", { sizeof (uint32_t), &multiview_flags[7] } },
};

static const uint32_t chroma_sites[] = {
  SPA_VIDEO_CHROMA_SITE_UNKNOWN,
  SPA_VIDEO_CHROMA_SITE_NONE,
  SPA_VIDEO_CHROMA_SITE_H_COSITED,
  SPA_VIDEO_CHROMA_SITE_V_COSITED,
  SPA_VIDEO_CHROMA_SITE_ALT_LINE,
};

static const SpaPropRangeInfo chroma_site_range[] = {
  { "unknown", { sizeof (uint32_t), &chroma_sites[0] } },
  { "none", { sizeof (uint32_t), &chroma_sites[1] } },
  { "h-cosited", { sizeof (uint32_t), &chroma_sites[2] } },
  { "v-cosited", { sizeof (uint32_t), &chroma_sites[3] } },
  { "alt-line", { sizeof (uint32_t), &chroma_sites[4] } }
};

static const uint32_t color_ranges[] = {
  SPA_VIDEO_COLOR_RANGE_UNKNOWN,
  SPA_VIDEO_COLOR_RANGE_0_255,
  SPA_VIDEO_COLOR_RANGE_16_235
};

static const SpaPropRangeInfo color_range_range[] = {
  { "unknown", { sizeof (uint32_t), &color_ranges[0] } },
  { "0_255", { sizeof (uint32_t), &color_ranges[1] } },
  { "16_235", { sizeof (uint32_t), &color_ranges[2] } },
};

static const uint32_t color_matrices[] = {
  SPA_VIDEO_COLOR_MATRIX_UNKNOWN,
  SPA_VIDEO_COLOR_MATRIX_RGB,
  SPA_VIDEO_COLOR_MATRIX_FCC,
  SPA_VIDEO_COLOR_MATRIX_BT709,
  SPA_VIDEO_COLOR_MATRIX_BT601,
  SPA_VIDEO_COLOR_MATRIX_SMPTE240M,
  SPA_VIDEO_COLOR_MATRIX_BT2020
};

static const SpaPropRangeInfo color_matrix_range[] = {
  { "unknown", { sizeof (uint32_t), &color_matrices[0] } },
  { "rgb", { sizeof (uint32_t), &color_matrices[1] } },
  { "fcc", { sizeof (uint32_t), &color_matrices[2] } },
  { "bt709", { sizeof (uint32_t), &color_matrices[3] } },
  { "bt601", { sizeof (uint32_t), &color_matrices[4] } },
  { "smpte240m", { sizeof (uint32_t), &color_matrices[5] } },
  { "bt2020", { sizeof (uint32_t), &color_matrices[6] } },
};

static const uint32_t transfer_functions[] = {
  SPA_VIDEO_TRANSFER_UNKNOWN,
  SPA_VIDEO_TRANSFER_GAMMA10,
  SPA_VIDEO_TRANSFER_GAMMA18,
  SPA_VIDEO_TRANSFER_GAMMA20,
  SPA_VIDEO_TRANSFER_GAMMA22,
  SPA_VIDEO_TRANSFER_BT709,
  SPA_VIDEO_TRANSFER_SMPTE240M,
  SPA_VIDEO_TRANSFER_SRGB,
  SPA_VIDEO_TRANSFER_GAMMA28,
  SPA_VIDEO_TRANSFER_LOG100,
  SPA_VIDEO_TRANSFER_LOG316,
  SPA_VIDEO_TRANSFER_BT2020_12,
  SPA_VIDEO_TRANSFER_ADOBERGB
};

static const SpaPropRangeInfo transfer_function_range[] = {
  { "unknown", { sizeof (uint32_t), &transfer_functions[0] } },
  { "gamma10", { sizeof (uint32_t), &transfer_functions[1] } },
  { "gamma18", { sizeof (uint32_t), &transfer_functions[2] } },
  { "gamma20", { sizeof (uint32_t), &transfer_functions[3] } },
  { "gamma22", { sizeof (uint32_t), &transfer_functions[4] } },
  { "bt709", { sizeof (uint32_t), &transfer_functions[5] } },
  { "smpte240m", { sizeof (uint32_t), &transfer_functions[6] } },
  { "srgb", { sizeof (uint32_t), &transfer_functions[7] } },
  { "gamma28", { sizeof (uint32_t), &transfer_functions[8] } },
  { "log100", { sizeof (uint32_t), &transfer_functions[9] } },
  { "log316", { sizeof (uint32_t), &transfer_functions[10] } },
  { "bt2020_12", { sizeof (uint32_t), &transfer_functions[11] } },
  { "adobergb", { sizeof (uint32_t), &transfer_functions[12] } },
};

static const uint32_t color_primaries[] = {
  SPA_VIDEO_COLOR_PRIMARIES_UNKNOWN,
  SPA_VIDEO_COLOR_PRIMARIES_BT709,
  SPA_VIDEO_COLOR_PRIMARIES_BT470M,
  SPA_VIDEO_COLOR_PRIMARIES_BT470BG,
  SPA_VIDEO_COLOR_PRIMARIES_SMPTE170M,
  SPA_VIDEO_COLOR_PRIMARIES_SMPTE240M,
  SPA_VIDEO_COLOR_PRIMARIES_FILM,
  SPA_VIDEO_COLOR_PRIMARIES_BT2020,
  SPA_VIDEO_COLOR_PRIMARIES_ADOBERGB
};

static const SpaPropRangeInfo color_primaries_range[] = {
  { "unknown", { sizeof (uint32_t), &color_primaries[0] } },
  { "bt709", { sizeof (uint32_t), &color_primaries[1] } },
  { "bt470M", { sizeof (uint32_t), &color_primaries[2] } },
  { "bt470BG", { sizeof (uint32_t), &color_primaries[3] } },
  { "smpte170m", { sizeof (uint32_t), &color_primaries[4] } },
  { "smpte240m", { sizeof (uint32_t), &color_primaries[5] } },
  { "film", { sizeof (uint32_t), &color_primaries[6] } },
  { "bt2020", { sizeof (uint32_t), &color_primaries[7] } },
  { "adobergb", { sizeof (uint32_t), &color_primaries[8] } },
};

static const uint32_t min_uint32 = 1;
static const uint32_t max_uint32 = UINT32_MAX;

static const SpaPropRangeInfo uint32_range[] = {
  { "min", { sizeof (uint32_t), &min_uint32 } },
  { "max", { sizeof (uint32_t), &max_uint32 } },
};

static const SpaFraction min_framerate = { 0, 1 };
static const SpaFraction max_framerate = { UINT32_MAX, 1 };

static const SpaPropRangeInfo framerate_range[] = {
  { "min", { sizeof (SpaFraction), &min_framerate } },
  { "max", { sizeof (SpaFraction), &max_framerate } },
};

static const SpaPropInfo format_prop_info[] =
{
  { SPA_PROP_ID_VIDEO_INFO,         0,
                                    "info",
                                    SPA_PROP_FLAG_READWRITE | SPA_PROP_FLAG_OPTIONAL | SPA_PROP_FLAG_INFO,
                                    SPA_PROP_TYPE_POINTER, sizeof (SpaVideoInfoRaw),
                                    SPA_PROP_RANGE_TYPE_NONE, 0, NULL,
                                    NULL },
  { SPA_PROP_ID_VIDEO_FORMAT,       0,
                                    "format",
                                    SPA_PROP_FLAG_READWRITE,
                                    SPA_PROP_TYPE_UINT32, sizeof (uint32_t),
                                    SPA_PROP_RANGE_TYPE_ENUM, SPA_N_ELEMENTS (format_range), format_range,
                                    NULL },
  { SPA_PROP_ID_VIDEO_SIZE,         0,
                                    "size",
                                    SPA_PROP_FLAG_READWRITE,
                                    SPA_PROP_TYPE_RECTANGLE, sizeof (SpaRectangle),
                                    SPA_PROP_RANGE_TYPE_MIN_MAX, 2, size_range,
                                    NULL },
  { SPA_PROP_ID_VIDEO_FRAMERATE,    0,
                                    "framerate",
                                    SPA_PROP_FLAG_READWRITE,
                                    SPA_PROP_TYPE_FRACTION, sizeof (SpaFraction),
                                    SPA_PROP_RANGE_TYPE_MIN_MAX, 2, framerate_range,
                                    NULL },
  { SPA_PROP_ID_VIDEO_MAX_FRAMERATE, 0,
                                    "max-framerate",
                                    SPA_PROP_FLAG_READWRITE | SPA_PROP_FLAG_OPTIONAL,
                                    SPA_PROP_TYPE_FRACTION, sizeof (SpaFraction),
                                    SPA_PROP_RANGE_TYPE_MIN_MAX, 2, framerate_range,
                                    NULL },
  { SPA_PROP_ID_VIDEO_VIEWS,        0,
                                    "views",
                                    SPA_PROP_FLAG_READWRITE | SPA_PROP_FLAG_OPTIONAL,
                                    SPA_PROP_TYPE_UINT32, sizeof (uint32_t),
                                    SPA_PROP_RANGE_TYPE_MIN_MAX, 2, uint32_range,
                                    NULL },
  { SPA_PROP_ID_VIDEO_INTERLACE_MODE, 0,
                                    "interlace-mode",
                                    SPA_PROP_FLAG_READWRITE | SPA_PROP_FLAG_OPTIONAL,
                                    SPA_PROP_TYPE_UINT32, sizeof (uint32_t),
                                    SPA_PROP_RANGE_TYPE_ENUM, SPA_N_ELEMENTS (interlace_mode_range), interlace_mode_range,
                                    NULL },
  { SPA_PROP_ID_VIDEO_PIXEL_ASPECT_RATIO, 0,
                                    "pixel-aspect-ratio",
                                    SPA_PROP_FLAG_READWRITE | SPA_PROP_FLAG_OPTIONAL,
                                    SPA_PROP_TYPE_FRACTION, sizeof (SpaFraction),
                                    SPA_PROP_RANGE_TYPE_MIN_MAX, 2, framerate_range,
                                    NULL },
  { SPA_PROP_ID_VIDEO_MULTIVIEW_MODE, 0,
                                    "multiview-mode",
                                    SPA_PROP_FLAG_READWRITE | SPA_PROP_FLAG_OPTIONAL,
                                    SPA_PROP_TYPE_UINT32, sizeof (uint32_t),
                                    SPA_PROP_RANGE_TYPE_ENUM, SPA_N_ELEMENTS (multiview_mode_range), multiview_mode_range,
                                    NULL },
  { SPA_PROP_ID_VIDEO_MULTIVIEW_FLAGS, 0,
                                    "multiview-flags",
                                    SPA_PROP_FLAG_READWRITE | SPA_PROP_FLAG_OPTIONAL,
                                    SPA_PROP_TYPE_UINT32, sizeof (uint32_t),
                                    SPA_PROP_RANGE_TYPE_FLAGS, SPA_N_ELEMENTS (multiview_flags_range), multiview_flags_range,
                                    NULL },
  { SPA_PROP_ID_VIDEO_CHROMA_SITE,  0,
                                    "chroma-site",
                                    SPA_PROP_FLAG_READWRITE | SPA_PROP_FLAG_OPTIONAL,
                                    SPA_PROP_TYPE_UINT32, sizeof (uint32_t),
                                    SPA_PROP_RANGE_TYPE_FLAGS, SPA_N_ELEMENTS (chroma_site_range), chroma_site_range,
                                    NULL },
  { SPA_PROP_ID_VIDEO_COLOR_RANGE,  0,
                                    "color-range",
                                    SPA_PROP_FLAG_READWRITE | SPA_PROP_FLAG_OPTIONAL,
                                    SPA_PROP_TYPE_UINT32, sizeof (uint32_t),
                                    SPA_PROP_RANGE_TYPE_ENUM, SPA_N_ELEMENTS (color_range_range), color_range_range,
                                    NULL },
  { SPA_PROP_ID_VIDEO_COLOR_MATRIX,  0,
                                    "color-matrix",
                                    SPA_PROP_FLAG_READWRITE | SPA_PROP_FLAG_OPTIONAL,
                                    SPA_PROP_TYPE_UINT32, sizeof (uint32_t),
                                    SPA_PROP_RANGE_TYPE_ENUM, SPA_N_ELEMENTS (color_matrix_range), color_matrix_range,
                                    NULL },
  { SPA_PROP_ID_VIDEO_TRANSFER_FUNCTION,  0,
                                    "transfer-function",
                                    SPA_PROP_FLAG_READWRITE | SPA_PROP_FLAG_OPTIONAL,
                                    SPA_PROP_TYPE_UINT32, sizeof (uint32_t),
                                    SPA_PROP_RANGE_TYPE_ENUM, SPA_N_ELEMENTS (transfer_function_range), transfer_function_range,
                                    NULL },
  { SPA_PROP_ID_VIDEO_COLOR_PRIMARIES,  0,
                                    "color-primaries",
                                    SPA_PROP_FLAG_READWRITE | SPA_PROP_FLAG_OPTIONAL,
                                    SPA_PROP_TYPE_UINT32, sizeof (uint32_t),
                                    SPA_PROP_RANGE_TYPE_ENUM, SPA_N_ELEMENTS (color_primaries_range), color_primaries_range,
                                    NULL },
};

SpaResult
spa_prop_info_fill_video (SpaPropInfo    *info,
                          SpaPropIdVideo  id,
                          size_t          offset)
{
  int i;

  if (info == NULL)
    return SPA_RESULT_INVALID_ARGUMENTS;

  i = id - SPA_PROP_ID_MEDIA_CUSTOM_START;

  if (i < 0 || i >= SPA_N_ELEMENTS (format_prop_info))
    return SPA_RESULT_INVALID_PROPERTY_INDEX;

  memcpy (info, &format_prop_info[i], sizeof (SpaPropInfo));
  info->offset = offset - sizeof (SpaFormat) + sizeof (SpaProps);

  return SPA_RESULT_OK;
}

SpaResult
spa_format_video_init (SpaMediaType     type,
                       SpaMediaSubType  subtype,
                       SpaFormatVideo  *format)
{
  SpaPropInfo *prop_info = NULL;
  unsigned int n_prop_info = 0;
  int i;

  if (type != SPA_MEDIA_TYPE_VIDEO)
    return SPA_RESULT_INVALID_ARGUMENTS;

  switch (subtype) {
    case SPA_MEDIA_SUBTYPE_RAW:
    {
      static SpaPropInfo raw_prop_info[] = {
        { SPA_PROP_ID_VIDEO_INFO,               offsetof (SpaFormatVideo, info.raw) },
        { SPA_PROP_ID_VIDEO_FORMAT,             offsetof (SpaFormatVideo, info.raw.format) },
        { SPA_PROP_ID_VIDEO_SIZE,               offsetof (SpaFormatVideo, info.raw.size) },
        { SPA_PROP_ID_VIDEO_FRAMERATE,          offsetof (SpaFormatVideo, info.raw.framerate) },
        { SPA_PROP_ID_VIDEO_MAX_FRAMERATE,      offsetof (SpaFormatVideo, info.raw.max_framerate) },
        { SPA_PROP_ID_VIDEO_VIEWS,              offsetof (SpaFormatVideo, info.raw.views) },
        { SPA_PROP_ID_VIDEO_INTERLACE_MODE,     offsetof (SpaFormatVideo, info.raw.interlace_mode) },
        { SPA_PROP_ID_VIDEO_PIXEL_ASPECT_RATIO, offsetof (SpaFormatVideo, info.raw.pixel_aspect_ratio) },
        { SPA_PROP_ID_VIDEO_MULTIVIEW_MODE,     offsetof (SpaFormatVideo, info.raw.multiview_mode) },
        { SPA_PROP_ID_VIDEO_MULTIVIEW_FLAGS,    offsetof (SpaFormatVideo, info.raw.multiview_flags) },
        { SPA_PROP_ID_VIDEO_CHROMA_SITE,        offsetof (SpaFormatVideo, info.raw.chroma_site) },
        { SPA_PROP_ID_VIDEO_COLOR_RANGE,        offsetof (SpaFormatVideo, info.raw.color_range) },
        { SPA_PROP_ID_VIDEO_COLOR_MATRIX,       offsetof (SpaFormatVideo, info.raw.color_matrix) },
        { SPA_PROP_ID_VIDEO_TRANSFER_FUNCTION,  offsetof (SpaFormatVideo, info.raw.transfer_function) },
        { SPA_PROP_ID_VIDEO_COLOR_PRIMARIES,    offsetof (SpaFormatVideo, info.raw.color_primaries) },
      };
      static const SpaVideoInfoRaw default_raw_info = {
        SPA_VIDEO_FORMAT_UNKNOWN,
        { 320, 240 },
        { 25, 1 },
        { 25, 1 },
        1,
        SPA_VIDEO_INTERLACE_MODE_PROGRESSIVE,
        { 1, 1},
        SPA_VIDEO_MULTIVIEW_MODE_MONO,
        SPA_VIDEO_MULTIVIEW_FLAGS_NONE,
        SPA_VIDEO_CHROMA_SITE_UNKNOWN,
        SPA_VIDEO_COLOR_RANGE_UNKNOWN,
        SPA_VIDEO_COLOR_MATRIX_UNKNOWN,
        SPA_VIDEO_TRANSFER_UNKNOWN,
        SPA_VIDEO_COLOR_PRIMARIES_UNKNOWN
      };
      prop_info = raw_prop_info;
      n_prop_info = SPA_N_ELEMENTS (raw_prop_info);
      format->format.props.unset_mask = (1 << 15)-2;
      format->info.raw = default_raw_info;
      break;
    }

    case SPA_MEDIA_SUBTYPE_H264:
    {
      static SpaPropInfo h264_prop_info[] = {
        { SPA_PROP_ID_VIDEO_INFO,               offsetof (SpaFormatVideo, info.h264) },
        { SPA_PROP_ID_VIDEO_SIZE,               offsetof (SpaFormatVideo, info.h264.size) },
        { SPA_PROP_ID_VIDEO_FRAMERATE,          offsetof (SpaFormatVideo, info.h264.framerate) },
        { SPA_PROP_ID_VIDEO_MAX_FRAMERATE,      offsetof (SpaFormatVideo, info.h264.max_framerate) },
      };
      static const SpaVideoInfoH264 default_h264_info = {
        { 320, 240 },
        { 25, 1 },
        { 25, 1 }
      };
      prop_info = h264_prop_info;
      n_prop_info = SPA_N_ELEMENTS (h264_prop_info);
      format->format.props.unset_mask = (1 << 4)-2;
      format->info.h264 = default_h264_info;
      break;
    }

    case SPA_MEDIA_SUBTYPE_MJPG:
    {
      static SpaPropInfo mjpg_prop_info[] = {
        { SPA_PROP_ID_VIDEO_INFO,               offsetof (SpaFormatVideo, info.mjpg) },
        { SPA_PROP_ID_VIDEO_SIZE,               offsetof (SpaFormatVideo, info.mjpg.size) },
        { SPA_PROP_ID_VIDEO_FRAMERATE,          offsetof (SpaFormatVideo, info.mjpg.framerate) },
        { SPA_PROP_ID_VIDEO_MAX_FRAMERATE,      offsetof (SpaFormatVideo, info.mjpg.max_framerate) },
      };
      static const SpaVideoInfoMJPG default_mjpg_info = {
        { 320, 240 },
        { 25, 1 },
        { 25, 1 }
      };
      prop_info = mjpg_prop_info;
      n_prop_info = SPA_N_ELEMENTS (mjpg_prop_info);
      format->format.props.unset_mask = (1 << 4)-2;
      format->info.mjpg = default_mjpg_info;
      break;
    }

    case SPA_MEDIA_SUBTYPE_DV:
    case SPA_MEDIA_SUBTYPE_MPEGTS:
    case SPA_MEDIA_SUBTYPE_H263:
    case SPA_MEDIA_SUBTYPE_MPEG1:
    case SPA_MEDIA_SUBTYPE_MPEG2:
    case SPA_MEDIA_SUBTYPE_MPEG4:
    case SPA_MEDIA_SUBTYPE_XVID:
    case SPA_MEDIA_SUBTYPE_VC1:
    case SPA_MEDIA_SUBTYPE_VP8:
    case SPA_MEDIA_SUBTYPE_VP9:
    case SPA_MEDIA_SUBTYPE_JPEG:
    case SPA_MEDIA_SUBTYPE_BAYER:
      return SPA_RESULT_NOT_IMPLEMENTED;

    default:
      return SPA_RESULT_INVALID_ARGUMENTS;
  }

  if (prop_info && prop_info[0].name == NULL) {
    for (i = 0; i < n_prop_info; i++)
      spa_prop_info_fill_video (&prop_info[i],
                                prop_info[i].id,
                                prop_info[i].offset);
  }

  format->format.media_type = type;
  format->format.media_subtype = subtype;
  format->format.props.n_prop_info = n_prop_info;
  format->format.props.prop_info = prop_info;

  return SPA_RESULT_OK;
}

SpaResult
spa_format_video_parse (const SpaFormat *format,
                        SpaFormatVideo  *vformat)
{
  SpaPropValue value;
  const SpaProps *props;
  SpaResult res;
  unsigned int idx;

  if ((void *)format == (void *)vformat)
    return SPA_RESULT_OK;

  if (format->media_type != SPA_MEDIA_TYPE_VIDEO)
    return SPA_RESULT_INVALID_MEDIA_TYPE;

  if ((res = spa_format_video_init (format->media_type,
                                    format->media_subtype,
                                    vformat)) < 0)
    return res;

  props = &format->props;
  idx = spa_props_index_for_id (props, SPA_PROP_ID_VIDEO_INFO);
  if ((res = spa_props_get_value (props, idx, &value)) < 0)
    goto fallback;

  if (props->prop_info[idx].type != SPA_PROP_TYPE_POINTER)
    goto fallback;

  memcpy (&vformat->info, value.value, SPA_MIN (value.size, sizeof (vformat->info)));
  vformat->format.props.unset_mask = props->unset_mask;

  return SPA_RESULT_OK;

fallback:
  res = spa_props_copy_values (props, &vformat->format.props);

  return res;
}
