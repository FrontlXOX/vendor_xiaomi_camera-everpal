# vendor_xiaomi_camera-everpal

> HyperOS Leica Camera v6 vendor tree for MediaTek Dimensity 810 (MT6833) — Android 16 AOSP

Prebuilt Leica Camera v6 (`com.android.camera` v6.0.001240.1) extracted from **HyperOS 3.0**
(`OS3.0.10.0.VNQCNXM_15.0`, Redmi Note 13 5G / `gold`) and ported to Android 16 AOSP
(LineageOS 23.0 / AlphaDroid base) on the Xiaomi Dimensity 810 device family.

This tree ships the fully patched APK alongside the MT6833 ISP/JNI native library stubs,
SELinux policies, linker configuration, and permission XMLs required for a clean build-time
integration — **no KernelSU module or manual flashing required**.

---

## Supported Devices

| Device | Codename | SoC |
| :--- | :---: | :---: |
| Xiaomi POCO M4 Pro 5G | `everpal` | MT6833P |
| Xiaomi Redmi Note 11S 5G | `everpal` | MT6833P |
| Xiaomi Redmi Note 11 5G | `evergo` | MT6833 |
| Xiaomi Redmi Note 11T 5G | `evergo` | MT6833 |
| Xiaomi Redmi Note 11S 5G (Global) | `opal` | MT6833 |
| Xiaomi POCO M4 Pro 5G (Global) | `evergreen` | MT6833 |

---

## What's Included

### APK — `proprietary/system/priv-app/MiuiCamera/MiuiCamera.apk`

HyperOS Leica Camera v6.0.001240.1, patched for Android 16 AOSP with the following fixes:

| Fix | Detail |
| :--- | :--- |
| **ABI packaging** | Native libs repackaged into `lib/arm64-v8a/` (was `lib/arm64/`) |
| **StreamConfigurationMap reflection** | `ba.c.O(I)` diverted to public `SCALER_STREAM_CONFIGURATION_MAP` getter — prevents `NullPointerException` during viewfinder init on Android 16 |
| **Uncompressed media assets** | All `.mp4`, `.ogg`, `.mp3`, `.wav`, `resources.arsc`, and native libs stored as `ZIP_STORED` (method 0), zipaligned to 4 bytes — prevents `AssetManager FileNotFoundException` on Android 16 |
| **Vendor tags null-safety** | `ba.p1` (`onCaptureCompleted`) and `com.android.camera.b$b` guarded against null `ICustomCaptureResult` on AOSP — prevents crash on photo capture |
| **AudioParaManger stubs** | Stub classes `android.media.AudioParaManger`, `AudioParaManger$TuneListener`, `AudioParaManger$EventListener` injected into `classes6.dex`; `c0/a.isSupportAiAudioNew()` hardcoded to `false` — prevents `NoClassDefFoundError` on AOSP |
| **EIS operating-mode bypass** | `b3/a.getOperatingMode()` forced to return `0xf010` (standard) instead of `0x8004` (EIS-pure) — eliminates `camerahalserver` SIGSEGV in `TuningMgrImp::tuningMgrWriteRegs` and `P2::StreamingProcessor` |
| **EIS full chain off** | `VideoModule.isEisOn()`, `UserRecordSetting.l()`, and `MediaRecorderCreator.e` all hardcoded off — prevents Video mode viewfinder blur and HAL tombstones |

Signed with `v1/v2/v3` signature schemes. Tracked via Git LFS.

### Native Libraries — `proprietary/system/lib64/`

| Library | Purpose |
| :--- | :--- |
| `libcamera_algoup_jni.xiaomi.so` | MT6833 ISP algorithm upscaling JNI |
| `libcamera_ispinterface_jni.xiaomi.so` | MT6833 ISP interface JNI |
| `libcamera_mianode_jni.xiaomi.so` | MiaNode postprocessing pipeline JNI |
| `libmtkisp_metadata_sys.so` | MTK ISP metadata system library |
| `libged_kpi.so` | Mali GED KPI performance counters |
| `libged_sys.so` | Mali GED system interface |
| `vendor.mediatek.hardware.camera.isphal@1.0.so` | MTK ISP HAL HIDL interface |

### Linker Shim — `shims/libsdk_sr/`

`libsdk_sr_shim.so` interposes `sr_*` symbol calls from `libmialgoengine.so` to prevent
`CL_INVALID_BINARY` crashes when the device lacks the matching OpenCL binary cache.

### Configs

| File | Destination | Purpose |
| :--- | :--- | :--- |
| `configs/permissions/privapp-permissions-miuicamera.xml` | `system/etc/permissions/` | Privileged permission grants |
| `configs/permissions/default-permissions-miuicamera.xml` | `system/etc/default-permissions/` | Default runtime permission grants |
| `configs/permissions/miuicamera-hiddenapi-package-whitelist.xml` | `system/etc/sysconfig/` | Hidden API whitelist |
| `configs/linker/public.libraries-xiaomi.txt` | `system/etc/` | Exposes Xiaomi JNI libs to app linker namespace |

---

## Integration

### Prerequisites

In your device tree's `device.mk`, add:

```makefile
$(call inherit-product-if-exists, vendor/xiaomi/camera/miuicamera.mk)
```

### System Properties Set

```
ro.com.google.lens.oem_camera_package=com.android.camera
ro.miui.notch=1
ro.product.mod_device=evergo_in_global
```

Also add to `vendor.prop` in your device tree:

```
persist.vendor.camera.privapp.list=com.android.camera
```

### SELinux

Vendor sepolicy is loaded automatically via:

```makefile
BOARD_VENDOR_SEPOLICY_DIRS += vendor/xiaomi/camera/sepolicy/vendor
```

Grants: `priv_app → same_process_hal_file`, `hal_misys_hwservice`, `hal_campostproc_hwservice`, `vendor_camera_prop`.

### BoardConfig

```makefile
# Included via BoardConfigVendor.mk
BUILD_BROKEN_ELF_PREBUILT_PRODUCT_COPY_FILES := true
BUILD_BROKEN_ENFORCE_USES_LIBRARIES := true
```

### PRODUCT_PACKAGES Installed

```
MiuiCamera
libcamera_algoup_jni.xiaomi
libcamera_ispinterface_jni.xiaomi
libcamera_mianode_jni.xiaomi
libged_kpi  libged_sys  libmtkisp_metadata_sys
vendor.mediatek.hardware.camera.isphal@1.0_system
libsdk_sr  libsdk_sr_shim
```

---

## Build Notes

- **Android 16 (AOSP / LineageOS 23.0):** Fully verified.
- **ELF prebuilts:** All `.so` files are declared as `cc_prebuilt_library_shared` in `Android.bp` — no `PRODUCT_COPY_FILES` for ELF binaries (compliant with Android 15+ enforcement).
- **Dexpreopt:** Disabled for `MiuiCamera` (`dex_preopt: { enabled: false }`) — the APK is pre-signed and pre-zipaligned.
- **Lib symlinks:** `Android.mk` creates `lib/arm64/` → `system/lib64/` symlinks inside the installed APK directory for legacy JNI loader compatibility.
- **libutils_binder fix:** If your ROM build fails on `libutils_binder_impl_defaults_nodeps_no_apex`, add `-DANDROID_UTILS_CALLSTACK_ENABLED=0` to its `cflags` in `system/core/libutils/binder/Android.bp`.

---

## Credits

- **EverpalTweaks port & Android 16 fixes:** [FrontlXOX](https://github.com/FrontlXOX)
- **Original tree base:** [himanshuksr0007](https://github.com/himanshuksr0007) / [mmtrt](https://gitlab.com/mmtrt)
- **Upstream device tree:** [Addster09](https://github.com/Addster09) / [xiaomi-mt6833-dev](https://github.com/xiaomi-mt6833-dev)

---

## License

```
SPDX-License-Identifier: Apache-2.0
```
