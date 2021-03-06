/*
 * Copyright © 2018, VideoLAN and dav1d authors
 * Copyright © 2018, Two Orioles, LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __DAV1D_SRC_TABLES_H__
#define __DAV1D_SRC_TABLES_H__

#include <stdint.h>

#include "common/intops.h"

#include "src/levels.h"

extern const uint8_t av1_al_part_ctx[2][N_BL_LEVELS][N_PARTITIONS];
extern const uint8_t /* enum BlockSize */
                     av1_block_sizes[N_BL_LEVELS][N_PARTITIONS][2];
// width, height (in 4px blocks), log2 versions of these two
extern const uint8_t av1_block_dimensions[N_BS_SIZES][4];
typedef struct TxfmInfo {
    // width, height (in 4px blocks), log2 of them, min/max of log2, sub, pad
    uint8_t w, h, lw, lh, min, max, sub, ctx;
} TxfmInfo;
extern const TxfmInfo av1_txfm_dimensions[N_RECT_TX_SIZES];
extern const uint8_t /* enum (Rect)TxfmSize */
                     av1_max_txfm_size_for_bs[N_BS_SIZES][4 /* y, 420, 422, 444 */];
extern const uint8_t /* enum TxfmType */
                     av1_txtp_from_uvmode[N_UV_INTRA_PRED_MODES];

extern const uint8_t /* enum InterPredMode */
                     av1_comp_inter_pred_modes[N_COMP_INTER_PRED_MODES][2];

extern const uint8_t av1_tx_type_count[N_TXTP_SETS];
extern const uint8_t /* enum TxfmType */
                     av1_tx_types_per_set[N_TXTP_SETS][N_TX_TYPES];
extern const uint8_t av1_tx_type_set_index[2][N_TXTP_SETS];

extern const uint8_t av1_filter_mode_to_y_mode[5];
extern const uint8_t av1_ymode_size_context[N_BS_SIZES];
extern const uint8_t av1_nz_map_ctx_offset[N_RECT_TX_SIZES][5][5];
extern const uint8_t /* enum TxClass */
                     av1_tx_type_class[N_TX_TYPES_PLUS_LL];
extern const uint8_t /* enum Filter2d */
                     av1_filter_2d[N_FILTERS /* h */][N_FILTERS /* v */];
extern const uint8_t /* enum FilterMode */ eve_av1_filter_dir[N_2D_FILTERS][2];
extern const uint8_t intra_mode_context[N_INTRA_PRED_MODES];
extern const uint8_t av1_wedge_ctx_lut[N_BS_SIZES];

extern const unsigned cfl_allowed_mask;
extern const unsigned wedge_allowed_mask;
extern const unsigned interintra_allowed_mask;

extern const WarpedMotionParams default_wm_params;

extern const int16_t sgr_params[16][4];
extern const int16_t sgr_x_by_xplus1[256];
extern const int16_t sgr_one_by_x[25];

extern const int8_t dav1d_mc_subpel_filters[5][15][8];
extern const int8_t dav1d_mc_warp_filter[][8];

#endif /* __DAV1D_SRC_TABLES_H__ */
