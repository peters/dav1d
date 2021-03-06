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

#ifndef __DAV1D_DATA_H__
#define __DAV1D_DATA_H__

#include <stddef.h>
#include <stdint.h>

#include "common.h"

typedef struct Dav1dData {
    uint8_t *data; ///< data pointer
    size_t sz; ///< data size
    struct Dav1dRef *ref; ///< allocation origin
} Dav1dData;

/**
 * Allocate data.
 */
DAV1D_API int dav1d_data_create(Dav1dData *data, size_t sz);

/**
 * Wrap an existing data packet.
 * $ptr/$sz are the pointer and size of the data to be wrapped.
 * $free_callback/$user_data are a function and function argument to be
 * called when we release our last reference to this data. In this callback,
 * $data will be the $ptr argument to this function, and $user_data will be
 * the $user_data input argument to this function.
 */
DAV1D_API int dav1d_data_wrap(Dav1dData *data, uint8_t *ptr, size_t sz,
                              void (*free_callback)(uint8_t *data, void *user_data),
                              void *user_data);

/**
 * Free data.
 */
DAV1D_API void dav1d_data_unref(Dav1dData *buf);

#endif /* __DAV1D_DATA_H__ */
