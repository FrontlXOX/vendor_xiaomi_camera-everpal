# Copyright (C) 2023 Paranoid Android
# SPDX-License-Identifier: Apache-2.0
#
# Android 16 notes:
#  - LOCAL_MODULE_TAGS := optional is still valid but eng/debug are not.
#  - $(hide) is still supported; no change needed.
#  - Symlink block kept: MiuiCamera loads its JNI libs from a relative
#    lib/arm64/ path inside the APK install dir, so the symlinks are still
#    required even though the .so files are now declared as
#    cc_prebuilt_library_shared modules in Android.bp.
#  - Filter expanded to all supported codenames (evergo, opal, evergreen).

LOCAL_PATH := $(call my-dir)

ifneq ($(filter everpal evergo opal evergreen,$(TARGET_DEVICE)),)

include $(CLEAR_VARS)

CAMERA_LIBRARIES := \
    libcamera_algoup_jni.xiaomi.so \
    libcamera_mianode_jni.xiaomi.so

CAMERA_SYMLINKS := $(addprefix \
    $(TARGET_OUT_APPS_PRIVILEGED)/MiuiCamera/lib/arm64/,\
    $(notdir $(CAMERA_LIBRARIES)))

$(CAMERA_SYMLINKS):
	@echo "MiuiCamera lib link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /system/lib64/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(CAMERA_SYMLINKS)

endif
