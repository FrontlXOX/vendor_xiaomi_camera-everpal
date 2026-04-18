# Copyright (C) 2023 Paranoid Android
# SPDX-License-Identifier: Apache-2.0
#
# Android 16 notes:
#  - PRODUCT_COPY_FILES for ELF (.so) files is REMOVED.
#    Android 15+ enforces BUILD_BROKEN_ELF_PREBUILT_PRODUCT_COPY_FILES and
#    will error (or silently drop them) when .so files are copy-filed directly.
#    All native libs are now declared as cc_prebuilt_library_shared in
#    Android.bp and pulled in via PRODUCT_PACKAGES below.
#  - PRODUCT_SOONG_NAMESPACES retained as-is; still required.

PRODUCT_SOONG_NAMESPACES += \
    vendor/xiaomi/camera

# All prebuilt modules declared in Android.bp
PRODUCT_PACKAGES += \
    MiuiCamera \
    libcamera_algoup_jni.xiaomi \
    libcamera_ispinterface_jni.xiaomi \
    libcamera_mianode_jni.xiaomi \
    libged_kpi \
    libged_sys \
    libgui-xiaomi \
    libmtkisp_metadata_sys \
    vendor.mediatek.hardware.camera.isphal@1.0
