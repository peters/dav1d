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
# Build definition for the dav1d library
#

# libdav1d source files
libdav1d_sources = files(
    'picture.c',
    'cpu.c',
    'data.c',
    'ref.c',
    'getbits.c',
    'obu.c',
    'decode.c',
    'cdf.c',
    'msac.c',
    'tables.c',
    'scan.c',
    'dequant_tables.c',
    'intra_edge.c',
    'lf_mask.c',
    'ref_mvs.c',
    'warpmv.c',
    'wedge.c',
    'qm.c',
)

# libdav1d bitdepth source files
# These files are compiled for each bitdepth with
# `BITDEPTH` defined to the currently built bitdepth.
libdav1d_tmpl_sources = files(
    'ipred.c',
    'itx.c',
    'ipred_prepare.c',
    'lf_apply.c',
    'loopfilter.c',
    'mc.c',
    'cdef_apply.c',
    'cdef.c',
    'lr_apply.c',
    'looprestoration.c',
    'recon.c'
)

# libdav1d entrypoint source files
# These source files contain library entry points and are
# built with the stack-realign flag set, where necessary.
libdav1d_entrypoints_sources = files(
    'lib.c',
    'thread_task.c'
)

# ASM specific sources
libdav1d_nasm_objs = []
if is_asm_enabled
    if (host_machine.cpu_family() == 'aarch64' or
        host_machine.cpu_family().startswith('arm'))

        libdav1d_sources += files(
            'arm/cpu.c',
        )
        libdav1d_tmpl_sources += files(
        )
        if host_machine.cpu_family() == 'aarch64'
            libdav1d_tmpl_sources += files(
            )
        elif host_machine.cpu_family().startswith('arm')
            libdav1d_tmpl_sources += files(
            )
        endif
    elif host_machine.cpu_family().startswith('x86')

        libdav1d_sources += files(
            'x86/cpu.c',
        )

        libdav1d_tmpl_sources += files(
            'x86/mc_init.c',
        )

        # NASM source files
        libdav1d_sources_asm = files(
            'x86/cpuid.asm',
            'x86/mc.asm',
        )

        # Compile the ASM sources with NASM
        libdav1d_nasm_objs = nasm_gen.process(libdav1d_sources_asm)
    endif
endif



#
# Windows .rc file
#

if host_machine.system() == 'windows'
    winmod = import('windows')
    rc_data = configuration_data()
    rc_data.set('VERSION_MAJOR', dav1d_version_major)
    rc_data.set('VERSION_MINOR', dav1d_version_minor)
    rc_data.set('VERSION_REVISION', dav1d_version_revision)
    rc_data.set('VERSION_EXTRA', '0')
    rc_data.set('COPYRIGHT_YEARS', '2018')

    rc_file = configure_file(
        input : 'dav1d.rc.in',
        output : 'dav1d.rc',
        configuration : rc_data
    )

    libdav1d_rc_obj = winmod.compile_resources(rc_file)
else
    libdav1d_rc_obj = []
endif




#
# Library definitions
#

# Helper library for dav1d entrypoints
libdav1d_entrypoints_objs = static_library('dav1d_entrypoint',
    libdav1d_entrypoints_sources,
    rev_target,

    include_directories : dav1d_inc_dirs,
    dependencies: [stdatomic_dependency],
    c_args : stackrealign_flag,
    install : false,
    build_by_default : false,
).extract_all_objects()

# Helper library for each bitdepth
libdav1d_bitdepth_objs = []
foreach bitdepth : dav1d_bitdepths
    libdav1d_bitdepth_objs += static_library(
        'dav1d_bitdepth_@0@'.format(bitdepth),
        libdav1d_tmpl_sources, config_h_target,
        include_directories: dav1d_inc_dirs,
        dependencies : [stdatomic_dependency],
        c_args : ['-DBITDEPTH=@0@'.format(bitdepth)] + stackalign_flag,
        install : false,
        build_by_default : false,
    ).extract_all_objects()
endforeach

# Static private helper library
# This is primarily needed to link with tests so that
# the tests can use all symbols, even the non-exported.
libdav1d_private = static_library('dav1d_private',
    libdav1d_sources,
    libdav1d_nasm_objs,
    libdav1d_rc_obj,

    objects : [
        libdav1d_bitdepth_objs,
        libdav1d_entrypoints_objs
        ],

    include_directories : dav1d_inc_dirs,
    dependencies : [stdatomic_dependency],
    c_args : [stackalign_flag],
    install : false,
    build_by_default : false,
)

# The final dav1d library
if host_machine.system() == 'windows'
    dav1d_soversion = ''
else
    dav1d_soversion = dav1d_version_major
endif

libdav1d = library('dav1d',
    version : meson.project_version(),
    link_whole : libdav1d_private,
    dependencies : thread_dependency,
    include_directories : dav1d_inc_dirs,
    soversion : dav1d_soversion,
    install : true,
)
