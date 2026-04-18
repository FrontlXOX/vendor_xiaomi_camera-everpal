# Copyright (C) 2023 Paranoid Android
# SPDX-License-Identifier: Apache-2.0
#
# Android 16 notes:
#  BUILD_BROKEN_ELF_PREBUILT_PRODUCT_COPY_FILES is a SAFETY NET only.
#  We have already removed all ELF PRODUCT_COPY_FILES from camera-vendor.mk
#  and moved every .so into Android.bp cc_prebuilt_library_shared modules.
#  This flag is kept here as a fallback in case any downstream device tree
#  or ROM source re-adds an ELF copy-file that would otherwise hard-error.
#
#  BUILD_BROKEN_ENFORCE_USES_LIBRARIES suppresses the global uses-library
#  check that was made fatal in Android 12 and is still enforced in 16.
#  We handle this per-module via enforce_uses_libs:false in Android.bp,
#  but this board flag provides a second safety net.

# Relax ELF-in-PRODUCT_COPY_FILES from a hard error to a warning.
# Remove once all downstream trees are clean.
BUILD_BROKEN_ELF_PREBUILT_PRODUCT_COPY_FILES := true

# Suppress global uses-library enforcement (MiuiCamera references MIUI jars).
BUILD_BROKEN_ENFORCE_USES_LIBRARIES := true
