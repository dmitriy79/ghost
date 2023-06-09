packages:=boost libevent

qrencode_packages = qrencode
qt_packages = zlib


qt_linux_packages:=qt expat libxcb xcb_proto libXau xproto freetype fontconfig libxcb_util libxcb_util_render libxcb_util_keysyms libxcb_util_image libxcb_util_wm
qt_android_packages=qt

qt_darwin_packages=qt
qt_mingw32_packages=qt

bdb_packages=bdb
sqlite_packages=sqlite

zmq_packages=zeromq

upnp_packages=miniupnpc
natpmp_packages=libnatpmp

multiprocess_packages = libmultiprocess capnp
multiprocess_native_packages = native_libmultiprocess native_capnp

usdt_linux_packages=systemtap

darwin_native_packages = native_biplist native_ds_store native_mac_alias


ifneq ($(build_os),darwin)
darwin_native_packages += native_cctools native_libtapi native_libdmg-hfsplus zlib

ifeq ($(strip $(FORCE_USE_SYSTEM_CLANG)),)
darwin_native_packages+= native_clang
endif

endif
