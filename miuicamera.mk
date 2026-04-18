#
# Copyright (C) 2023 Paranoid Android
#
# SPDX-License-Identifier: Apache-2.0
#
# Android 16 notes:
#  - PRODUCT_SYSTEM_PROPERTIES already used here (correct - not the deprecated
#    PRODUCT_PROPERTY_OVERRIDES). No change needed for props.
#  - Permission XMLs are still copied via PRODUCT_COPY_FILES; this is correct
#    because they are plain XML files, not ELF binaries.
#  - Added privapp-permissions copy (was present before, kept as-is).
#  - BOARD_VENDOR_SEPOLICY_DIRS kept as-is; still valid in Android 16.

# Permissions
PRODUCT_COPY_FILES += \
    vendor/xiaomi/camera/configs/permissions/default-permissions-miuicamera.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/default-permissions/default-permissions-miuicamera.xml \
    vendor/xiaomi/camera/configs/permissions/miuicamera-hiddenapi-package-whitelist.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/sysconfig/miuicamera-hiddenapi-package-whitelist.xml \
    vendor/xiaomi/camera/configs/permissions/privapp-permissions-miuicamera.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/permissions/privapp-permissions-miuicamera.xml

# Properties
# ro.miui.notch        – tells MiuiCamera this device has a notch/punch-hole
# ro.product.mod_device – used by MIUI camera for feature flag lookup
# ro.com.google.lens.oem_camera_package – registers this app as the OEM camera
#                          for Google Lens handoff
PRODUCT_SYSTEM_PROPERTIES += \
    ro.com.google.lens.oem_camera_package=com.android.camera \
    ro.miui.notch=1 \
    ro.product.mod_device=evergo_in_global

# Sepolicy
BOARD_VENDOR_SEPOLICY_DIRS += \
    vendor/xiaomi/camera/sepolicy/vendor

$(call inherit-product, vendor/xiaomi/camera/camera-vendor.mk)
