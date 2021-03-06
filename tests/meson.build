# Copyright © 2018, VideoLAN and dav1d authors
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#
# Build definition for the dav1d tests
#

# Leave subdir if tests are disabled
if not get_option('build_tests')
    subdir_done()
endif

if is_asm_enabled
    checkasm_sources = files('checkasm/checkasm.c')

    checkasm_tmpl_sources = files('checkasm/mc.c')

    checkasm_bitdepth_objs = []
    foreach bitdepth : dav1d_bitdepths
        checkasm_bitdepth_lib = static_library(
            'checkasm_bitdepth_@0@'.format(bitdepth),
            checkasm_tmpl_sources,
            include_directories: dav1d_inc_dirs,
            c_args: ['-DBITDEPTH=@0@'.format(bitdepth), stackalign_flag],
            install: false,
            build_by_default: false,
        )
        checkasm_bitdepth_objs += checkasm_bitdepth_lib.extract_all_objects()
    endforeach

    checkasm_nasm_objs = []
    if host_machine.cpu_family() == 'aarch64'
        checkasm_sources += files('checkasm/arm/checkasm_64.S')
    elif host_machine.cpu_family().startswith('arm')
        checkasm_sources += files('checkasm/arm/checkasm_32.S')
    elif host_machine.cpu_family().startswith('x86')
        checkasm_nasm_objs = nasm_gen.process(files('checkasm/x86/checkasm.asm'))
    endif

    checkasm = executable('checkasm', checkasm_sources, checkasm_nasm_objs,
        objects: [checkasm_bitdepth_objs],
        include_directories: dav1d_inc_dirs,
        link_with: libdav1d_private,
        c_args: [stackalign_flag, stackrealign_flag],
        build_by_default: false,
        )

    test('checkasm test', checkasm)
endif
