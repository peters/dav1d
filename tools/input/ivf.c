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

#include "config.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "common/intops.h"

#include "input/demuxer.h"

typedef struct DemuxerPriv {
    FILE *f;
} IvfInputContext;

static int ivf_open(IvfInputContext *const c, const char *const file,
                    unsigned fps[2], unsigned *const num_frames)
{
    int res;
    uint8_t hdr[32];

    memset(c, 0, sizeof(*c));
    if (!(c->f = fopen(file, "rb"))) {
        fprintf(stderr, "Failed to open %s: %s\n", file, strerror(errno));
        return -1;
    } else if ((res = fread(hdr, 32, 1, c->f)) != 1) {
        fprintf(stderr, "Failed to read stream header: %s\n", strerror(errno));
        fclose(c->f);
        return -1;
    } else if (memcmp(hdr, "DKIF", 4)) {
        fprintf(stderr, "%s is not an IVF file [tag=%4s|0x%02x%02x%02x%02x]\n",
                file, hdr, hdr[0], hdr[1], hdr[2], hdr[3]);
        fclose(c->f);
        return -1;
    } else if (memcmp(&hdr[8], "AV01", 4)) {
        fprintf(stderr, "%s is not an AV1 file [tag=%4s|0x%02x%02x%02x%02x]\n",
                file, &hdr[8], hdr[8], hdr[9], hdr[10], hdr[11]);
        fclose(c->f);
        return -1;
    }

    fps[0] = rl32(&hdr[16]);
    fps[1] = rl32(&hdr[20]);
    const unsigned duration = rl32(&hdr[24]);
    uint8_t data[4];
    for (*num_frames = 0;; (*num_frames)++) {
        if ((res = fread(data, 4, 1, c->f)) != 1)
            break; // EOF
        fseek(c->f, rl32(data) + 8, SEEK_CUR);
    }
    fps[0] *= *num_frames;
    fps[1] *= duration;
    fseek(c->f, 32, SEEK_SET);

    return 0;
}

static int ivf_read(IvfInputContext *const c, Dav1dData *const buf) {
    uint8_t data[4];
    int res;

    if ((res = fread(data, 4, 1, c->f)) != 1)
        return -1; // EOF
    fseek(c->f, 8, SEEK_CUR); // skip timestamp
    const ptrdiff_t sz = rl32(data);
    dav1d_data_create(buf, sz);
    if ((res = fread(buf->data, sz, 1, c->f)) != 1)
        fprintf(stderr, "Failed to read frame data: %s\n", strerror(errno));

    return 0;
}

static void ivf_close(IvfInputContext *const c) {
    fclose(c->f);
}

const Demuxer ivf_demuxer = {
    .priv_data_size = sizeof(IvfInputContext),
    .name = "ivf",
    .extension = "ivf",
    .open = ivf_open,
    .read = ivf_read,
    .close = ivf_close,
};
