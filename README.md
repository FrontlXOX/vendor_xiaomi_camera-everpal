# proprietary_vendor_xiaomi_camera

Prebuilt stock MIUI Camera for custom ROM builds — **ported for Android 16**.

Extracted from evergo MIUI V14.0.2.0.TGBINXM package (refer `proprietary-files.txt` for version).

### Supported devices
* Xiaomi Redmi Note 11 5G (evergo)
* Xiaomi Redmi Note 11T 5G (evergo)
* Xiaomi Redmi Note 11S 5G (opal)
* Xiaomi POCO M4 Pro 5G (evergo)
* Xiaomi POCO M4 Pro 5G (evergreen)

---

### How to use

1. Clone this repo to `vendor/xiaomi/camera`:
   ```bash
   git clone <repo-url> vendor/xiaomi/camera
   ```

2. Inherit it from `device.mk` in your device tree:
   ```makefile
   # MIUI Camera
   $(call inherit-product-if-exists, vendor/xiaomi/camera/miuicamera.mk)
   ```

That's it. No other changes are needed in the device tree.

---

### Android 16 Changes (vs the Android 14 tree)

| File | Status | What changed |
|---|---|---|
| `Android.bp` | **Updated** | Added `enforce_uses_libs: false`; all 7 native `.so` libs moved to `cc_prebuilt_library_shared` modules |
| `Android.mk` | **Updated** | Symlink filter broadened from `everpal` to all 3 codenames; `include $(CLEAR_VARS)` added |
| `camera-vendor.mk` | **Updated** | All ELF `PRODUCT_COPY_FILES` removed; libs added to `PRODUCT_PACKAGES` instead |
| `miuicamera.mk` | **No change** | Already used `PRODUCT_SYSTEM_PROPERTIES` (correct for A16) |
| `BoardConfigVendor.mk` | **Updated** | Added `BUILD_BROKEN_ELF_PREBUILT_PRODUCT_COPY_FILES` and `BUILD_BROKEN_ENFORCE_USES_LIBRARIES` safety flags |
| `configs/permissions/privapp-permissions-miuicamera.xml` | **Updated** | Comprehensive permission list including Android 13/14/15/16 new permissions |
| `configs/permissions/default-permissions-miuicamera.xml` | **Updated** | Added `READ_MEDIA_IMAGES`, `READ_MEDIA_VIDEO`, `ACCESS_MEDIA_LOCATION` for Android 13+ storage model |
| `configs/permissions/miuicamera-hiddenapi-package-whitelist.xml` | **No change** | Still valid |
| `sepolicy/vendor/miuicamera.te` | **Updated** | Added GED node and ISP HAL access rules |
| `extract-files.sh` | **No change** | Extraction logic unchanged |
| `proprietary-files.txt` | **No change** | Same set of blobs |
| `setup-makefiles.sh` | **No change** | Generator script unchanged |

---

### Why ELF libs were moved out of `PRODUCT_COPY_FILES`

Android 15 introduced a hard build error when `.so` files appear in
`PRODUCT_COPY_FILES`. The fix is to declare them as
`cc_prebuilt_library_shared` modules in `Android.bp` and add their module
names to `PRODUCT_PACKAGES` in `camera-vendor.mk`. The `BoardConfigVendor.mk`
flag `BUILD_BROKEN_ELF_PREBUILT_PRODUCT_COPY_FILES := true` acts as a safety
net for any remaining downstream stragglers.

### Why `enforce_uses_libs: false`

MiuiCamera's `AndroidManifest.xml` declares `<uses-library>` tags for
MIUI-only framework jars (`miui-stat.jar`, `miuisdk.jar`, etc.) that do not
exist in AOSP. Without disabling this check the build fails during dexopt
with a "class loader context mismatch" error. The flag suppresses the check
entirely for this module.

### Caveats

The APK itself is compiled against Android 14 MIUI firmware. It will launch and
operate on Android 16 for basic camera functions. Advanced MIUI-specific
features (AI scene detection calling into MIUI framework, MIUI cloud backup,
etc.) may be silently no-ops or produce logcat errors — this is expected and
does not affect stability.

If you see AVC denials in logcat after flashing, add the missing rules to
`sepolicy/vendor/miuicamera.te` using `audit2allow`.
