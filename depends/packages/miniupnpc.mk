package=miniupnpc
$(package)_version=2.2.8
$(package)_download_path=http://miniupnp.free.fr/files
$(package)_file_name=$(package)-$($(package)_version).tar.gz
$(package)_sha256_hash=05b929679091b9921b6b6c1f25e39e4c8d1f4d46c8feb55a412aa697aee03a93

define $(package)_set_vars
$(package)_build_opts=CC="$($(package)_cc)"
$(package)_build_opts_darwin=LIBTOOL="$($(package)_libtool)"
$(package)_build_opts_mingw32=-f Makefile.mingw
ifeq ($(host_os),mingw32)
$(package)_lib_file=libminiupnpc.a
else
$(package)_lib_file=build/libminiupnpc.a
endif
$(package)_build_env+=CFLAGS="$($(package)_cflags) $($(package)_cppflags)" AR="$($(package)_ar)"
endef

define $(package)_preprocess_cmds
  mkdir -p dll build && \
  sed -e 's|MINIUPNPC_VERSION_STRING \"version\"|MINIUPNPC_VERSION_STRING \"$($(package)_version)\"|' -e 's|OS/version|$(host)|' miniupnpcstrings.h.in > miniupnpcstrings.h && \
  cp miniupnpcstrings.h build/ && \
  sed -i.old "s|miniupnpcstrings.h: miniupnpcstrings.h.in wingenminiupnpcstrings|miniupnpcstrings.h: miniupnpcstrings.h.in|" Makefile.mingw
endef

define $(package)_build_cmds
	$(MAKE) $($(package)_lib_file) $($(package)_build_opts)
endef

define $(package)_stage_cmds
	mkdir -p $($(package)_staging_prefix_dir)/include/miniupnpc $($(package)_staging_prefix_dir)/lib &&\
	install include/*.h miniupnpcstrings.h $($(package)_staging_prefix_dir)/include/miniupnpc &&\
	install $($(package)_lib_file) $($(package)_staging_prefix_dir)/lib
endef
